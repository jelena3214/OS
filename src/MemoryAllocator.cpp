//
// Created by os on 6/18/22.
//

#include "../h/MemoryAllocator.hpp"
MemoryAllocator* MemoryAllocator::instance = nullptr;


void *MemoryAllocator::allocate(size_t size) {
    //uint64 p = (uint64)(freeMemHead);
    //printInteger((uint64)freeMemHead);
    if(freeMemHead == nullptr){
        printString("Nema slobodnog prostora\n");
        return nullptr; //no free space anymore
    }
    printString("Broj freeMemHead slobodnih blokova: \n");
    printInteger((uint64)(freeMemHead->numOfBlocks));
    __putc('\n');
    for(FreeMem* cur = freeMemHead; cur != nullptr; cur = cur->next){
        if(cur->numOfBlocks >= size){
            size_t execBlocks = cur->numOfBlocks - size;
            if(execBlocks){
                FreeMem* newFragment = (FreeMem*)((char*)cur+size*MEM_BLOCK_SIZE);
                if(cur->prev)cur->prev->next = cur->next;
                else freeMemHead = cur->next;
                if(cur->next)cur->next->prev = newFragment;
                newFragment->prev = cur->prev;
                newFragment->next = cur->next;
                newFragment->numOfBlocks = execBlocks;
            }
            MemBlock* newMemBlock = (MemBlock*)((char*)cur);
            newMemBlock->numOfBlocks = size;
            if(allocatedMemHead){
                MemBlock* p = allocatedMemHead;
                for( ;p->next != nullptr; p = p->next);
                p->next = newMemBlock;
                newMemBlock->prev = p;
                newMemBlock->next = nullptr;
            }else{
                allocatedMemHead = newMemBlock;
                allocatedMemHead->prev = allocatedMemHead->next = nullptr;
            }
            return newMemBlock;
        }
    }
    return nullptr; //if there is not any fitting block
}

int MemoryAllocator::deallocate(void *block) {
    MemBlock* del = (MemBlock*)block;
    FreeMem* cur = nullptr;
    printString("Adresa alociranog bloka: \n");
    printInteger((uint64)allocatedMemHead);
    __putc('\n');
    if(!allocatedMemHead){
        return 0;
    }
    if(!freeMemHead || (char*)block < (char*)freeMemHead){
        cur = 0;
    }else{
        for(cur = freeMemHead; cur->next && (char*)block > (char*)cur->next; cur = cur->next);
    }

    if(cur && (char*)cur+cur->numOfBlocks*MEM_BLOCK_SIZE == (char*)block){
        cur->numOfBlocks += del->numOfBlocks;
        if(cur->next && (char*)cur+cur->numOfBlocks*MEM_BLOCK_SIZE == (char*)cur->next){
            cur->numOfBlocks += cur->next->numOfBlocks;
            cur->next = cur->next->next;
            if(cur->next)cur->next->prev = cur;
        }
        updateMemBlocks(block);
        return 1;
    }else{
        FreeMem* nextSegment = cur?cur->next:freeMemHead;
        if(nextSegment && (char*)block + del->numOfBlocks*MEM_BLOCK_SIZE == (char*)nextSegment){
            FreeMem* newSegment = (FreeMem*)block;
            newSegment->numOfBlocks = nextSegment->numOfBlocks + del->numOfBlocks;
            newSegment->prev = nextSegment->prev;
            newSegment->next = nextSegment->next;
            if(nextSegment->next)nextSegment->next->prev = newSegment;
            if(nextSegment->prev)nextSegment->prev->next = newSegment;
            else freeMemHead = newSegment;
            updateMemBlocks(block);
            return 1;
        }
    }

    FreeMem* newSegment = (FreeMem*)block;
    newSegment->numOfBlocks = del->numOfBlocks;
    newSegment->prev = cur;
    if(cur)newSegment->next = cur->next;
    else newSegment->next = freeMemHead;
    if(newSegment->next)newSegment->next->prev = newSegment;
    if(cur)cur->next = newSegment;
    else freeMemHead = newSegment;
    return 1;
}

void MemoryAllocator::updateMemBlocks(void *del) {
    MemBlock* block = (MemBlock*)del;
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