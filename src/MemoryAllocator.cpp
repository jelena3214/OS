//
// Created by os on 6/18/22.
//

#include "../h/MemoryAllocator.hpp"
MemoryAllocator* MemoryAllocator::instance = nullptr;

//size neka bude u bajtovima
void *MemoryAllocator::allocate(size_t size) {
    size_t blockNum = (size + headerSize)/MEM_BLOCK_SIZE + ((size + headerSize)%MEM_BLOCK_SIZE == 0?0:1);
    printString("Potreban prostor\n");
    printInteger(blockNum);
    __putc('\n');
    if(freeMemHead == nullptr){
        printString("Nema slobodnog prostora\n");
        return nullptr; //no free space anymore
    }
    printString("Broj freeMemHead slobodnih blokova: \n");
    printInteger((uint64)(freeMemHead->numOfBlocks));
    __putc('\n');
    for(Block* cur = freeMemHead; cur != nullptr; cur = cur->next){
        printString("ETO\n");
        printInteger(cur->numOfBlocks);
        __putc('\n');
        if(cur->numOfBlocks >= blockNum){
            printString("Mesto");
            __putc('\n');
            size_t execBlocks = cur->numOfBlocks - blockNum;
            printString("EXEC: ");
            printInteger(execBlocks);
            __putc('\n');
            if(execBlocks){
                Block* newFragment = (Block*)((char*)cur+blockNum*MEM_BLOCK_SIZE);
                if(cur->prev)cur->prev->next = cur->next;
                else freeMemHead = newFragment;
                if(cur->next)cur->next->prev = newFragment;
                newFragment->prev = cur->prev;
                newFragment->next = cur->next;
                newFragment->numOfBlocks = execBlocks;
            }else{
                if(cur->prev)cur->prev->next = cur->next;
                else freeMemHead = cur->next;
                if(cur->next)cur->next->prev = cur->prev;
            }
            Block* newMemBlock = (Block*)(cur);
            newMemBlock->numOfBlocks = blockNum;
            if(allocatedMemHead){
                Block* p = allocatedMemHead;
                for( ;p->next != nullptr; p = p->next);
                p->next = newMemBlock;
                newMemBlock->prev = p;
                newMemBlock->next = nullptr;
            }else {
                allocatedMemHead = newMemBlock;
                allocatedMemHead->prev = allocatedMemHead->next = nullptr;
            }
            return (void*)((char*)newMemBlock + headerSize);
        }
    }
    return nullptr; //if there is not any fitting block
}

int MemoryAllocator::deallocate(void *block) {
    Block* cur = nullptr;
    size_t startAddr = (size_t)HEAP_START_ADDR + ((sizeof(MemoryAllocator) + headerSize)/MEM_BLOCK_SIZE + ((sizeof(MemoryAllocator) + headerSize)%MEM_BLOCK_SIZE == 0?0:1)) * MEM_BLOCK_SIZE;
    if(!allocatedMemHead || ((size_t)block < startAddr || (size_t)block > (size_t)HEAP_END_ADDR) ){
        return 0;
    }

    Block* del = (Block*)((char*)block - headerSize);
    if(!freeMemHead || (char*)block - headerSize < (char*)freeMemHead){
        cur = 0;
    }else{
        for(cur = freeMemHead; cur->next && (char*)block - headerSize > (char*)cur->next; cur = cur->next);
    }
    updateMemBlocks((char*)block - headerSize);

    if(cur && (char*)cur+cur->numOfBlocks*MEM_BLOCK_SIZE == ((char*)block - headerSize)){
        cur->numOfBlocks += del->numOfBlocks;
        printString("BROJ UVECAN: ");
        printInteger(cur->numOfBlocks);
        __putc('\n');
        if(cur->next && (char*)cur+cur->numOfBlocks*MEM_BLOCK_SIZE == (char*)cur->next){
            cur->numOfBlocks += cur->next->numOfBlocks;
            cur->next = cur->next->next;
            if(cur->next)cur->next->prev = cur;
            printString("USAO U OVO\n");
        }
        printString("BROJ OSLOBODJENIH: ");
        printInteger(del->numOfBlocks);
        __putc('\n');
        joinFreeSpace();
        return 1;
    }else{
        Block* nextSegment = cur?cur->next:freeMemHead;
        if(nextSegment && (char*)block + del->numOfBlocks*MEM_BLOCK_SIZE == (char*)nextSegment){
            Block* newSegment = (Block*)((char*)block - headerSize);
            newSegment->numOfBlocks = nextSegment->numOfBlocks + del->numOfBlocks;
            newSegment->prev = nextSegment->prev;
            newSegment->next = nextSegment->next;
            if(nextSegment->next)nextSegment->next->prev = newSegment;
            if(nextSegment->prev)nextSegment->prev->next = newSegment;
            else freeMemHead = newSegment;
            printString("OVDE JE\n");
            printString("BROJ OSLOBODJENIH: ");
            printInteger(newSegment->numOfBlocks);
            __putc('\n');
            joinFreeSpace();
            return 1;
        }
    }
    Block* newSegment = (Block*)((char*)block - headerSize);
    newSegment->numOfBlocks = del->numOfBlocks;
    printString("BROJ OSLOBODJENIH: ");
    printInteger(newSegment->numOfBlocks);
    __putc('\n');
    newSegment->prev = cur;
    if(cur)newSegment->next = cur->next;
    else newSegment->next = freeMemHead;
    if(newSegment->next)newSegment->next->prev = newSegment;
    if(cur)cur->next = newSegment;
    else freeMemHead = newSegment;
    joinFreeSpace();
    return 1;
}

void MemoryAllocator::updateMemBlocks(void *del) {
    Block* block = (Block*)del;
    if(allocatedMemHead == block){
        allocatedMemHead = block->next;
    }
    if(block->next != nullptr){
        block->next->prev = block->prev;
    }
    if(block->prev != nullptr){
        block->prev->next = block->next;
    }
}

void MemoryAllocator::joinFreeSpace() {
    Block* cur = freeMemHead;
    if(!cur)return;
    while(1){
        if(cur->next && (char*)cur+cur->numOfBlocks*MEM_BLOCK_SIZE == (char*)cur->next){
            cur->numOfBlocks += cur->next->numOfBlocks;
            cur->next = cur->next->next;
            if(cur->next)cur->next->prev = cur;
        }else{
            if(cur->next == nullptr)break;
            cur = cur->next;
        }
    }
}

void MemoryAllocator::ispisAlloc() {
    printString("ISPIS ALOC\n");
    Block* cur = nullptr;
    for(cur = allocatedMemHead; cur; cur = cur->next){
        printInteger(cur->numOfBlocks);
        __putc('\n');
    }
}

void MemoryAllocator::ispisFree() {
    printString("ISPIS FRE\n");
    Block* cur = nullptr;
    for(cur = freeMemHead; cur; cur = cur->next){
        printInteger(cur->numOfBlocks);
        __putc('\n');
    }
}
