//
// Created by os on 6/18/22.
//

#include "../h/MemoryAllocator.hpp"

//size neka bude u bajtovima
void *MemoryAllocator::allocate(size_t size) {
    size_t blockNum = (size + headerSize) / MEM_BLOCK_SIZE + ((size + headerSize) % MEM_BLOCK_SIZE == 0 ? 0 : 1);

    if (freeMemHead == nullptr) {
        return nullptr; //nema slobodnog prostora
    }

    for (Block *cur = freeMemHead; cur != nullptr; cur = cur->next) {

        if (cur->numOfBlocks >= blockNum) {
            size_t execBlocks = cur->numOfBlocks - blockNum;

            if (execBlocks) {
                Block *newFragment = (Block *) ((char *) cur + blockNum * MEM_BLOCK_SIZE);
                if (cur->prev)cur->prev->next = newFragment;
                else freeMemHead = newFragment;
                if (cur->next)cur->next->prev = newFragment;
                newFragment->prev = cur->prev;
                newFragment->next = cur->next;
                newFragment->numOfBlocks = execBlocks;
            } else {
                if (cur->prev)cur->prev->next = cur->next;
                else freeMemHead = cur->next;
                if (cur->next)cur->next->prev = cur->prev;
            }
            Block *newMemBlock = (Block *) (cur);
            newMemBlock->numOfBlocks = blockNum;
            if (allocatedMemHead) {
                Block *p = allocatedMemHead;
                for (; p->next && (char *) p->next < (char *) newMemBlock; p = p->next);
                newMemBlock->next = p->next;
                newMemBlock->prev = p;
                if (p->next)p->next->prev = newMemBlock;
                p->next = newMemBlock;
            } else {
                allocatedMemHead = newMemBlock;
                allocatedMemHead->prev = allocatedMemHead->next = nullptr;
            }
            return (void *) ((char *) newMemBlock + headerSize);
        }
    }

    return nullptr;
}


void MemoryAllocator::updateMemBlocks(void *del) {
    Block *block = (Block *) del;
    if (allocatedMemHead == block) {
        allocatedMemHead = block->next;
    }
    if (block->next != nullptr) {
        block->next->prev = block->prev;
    }
    if (block->prev != nullptr) {
        block->prev->next = block->next;
    }
}


void MemoryAllocator::ispisAlloc() {
    //printString("ISPIS ALOC\n");
    Block *cur = nullptr;
    for (cur = allocatedMemHead; cur; cur = cur->next) {
        //printInteger(cur->numOfBlocks);
        //__putc('\n');
    }
}

void MemoryAllocator::ispisFree() {
    //printString("ISPIS FRE\n");
    Block *cur = nullptr;
    for (cur = freeMemHead; cur; cur = cur->next) {
        //printInteger(cur->numOfBlocks);
        //__putc('\n');
    }
}

int MemoryAllocator::deallocate(void *block) {
    Block *cur = nullptr;
    size_t startAddr = (size_t) HEAP_START_ADDR;
    if (!allocatedMemHead ||
        ((size_t) block - headerSize < startAddr || (size_t) block - headerSize > (size_t) HEAP_END_ADDR)) {
        return -1; // adresa ne pripada opsegu za alokaciju
    }
    if (!freeMemHead || (char *) block - headerSize < (char *) freeMemHead) {
        cur = 0;
    } else {
        for (cur = freeMemHead; cur->next && (char *) block - headerSize > (char *) cur->next; cur = cur->next);
    }
    updateMemBlocks((char *) block - headerSize);

    Block *newSeg = (Block *) ((char *) block - headerSize);
    newSeg->prev = cur;
    if (cur)newSeg->next = cur->next;
    else newSeg->next = freeMemHead;
    if (newSeg->next)newSeg->next->prev = newSeg;
    if (cur)cur->next = newSeg;
    else freeMemHead = newSeg;

    tryToJoin(newSeg);
    tryToJoin(cur);
    return 0; //success
}

int MemoryAllocator::tryToJoin(Block *cur) {
    if (!cur)return 0;
    if (cur->next && (char *) cur + cur->numOfBlocks * MEM_BLOCK_SIZE == (char *) cur->next) {
        cur->numOfBlocks += cur->next->numOfBlocks;
        cur->next = cur->next->next;
        if (cur->next)cur->next->prev = cur;
        return 1;
    } else {
        return 0;
    }

}
