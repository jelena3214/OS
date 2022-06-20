//
// Created by os on 6/18/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"
#include "../lib/console.h"
#include "../h/print.hpp"

//kako njega da alociramo sa new?

class MemoryAllocator {
public:
    struct FreeMem{
        FreeMem* next, *prev;
        size_t numOfBlocks;
    };
    struct MemBlock{
        MemBlock* prev,*next;
        size_t numOfBlocks;
    };
private:
    MemoryAllocator(){
        void* newB = (void*) HEAP_START_ADDR;
        allocatedMemHead = static_cast<MemBlock *>(newB);
        //freeMemHead->prev = freeMemHead->next = nullptr;
        //freeMemHead->numOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR)/MEM_BLOCK_SIZE;
    }
    static MemoryAllocator* instance;
    FreeMem* freeMemHead;
    MemBlock* allocatedMemHead;
    static void initialize(){
        __putc('k');
        //instance =  reinterpret_cast<MemoryAllocator*>(const_cast<void*>(HEAP_START_ADDR));
        MemBlock* newB = reinterpret_cast<MemBlock*>(const_cast<void*>(HEAP_START_ADDR));
        newB->next = newB->prev = nullptr;
        newB->numOfBlocks = sizeof(MemoryAllocator)/MEM_BLOCK_SIZE + (sizeof(MemoryAllocator)%MEM_BLOCK_SIZE == 0?0:1);
        instance = reinterpret_cast<MemoryAllocator*>(newB);
        instance->freeMemHead = reinterpret_cast<FreeMem *>((char *) HEAP_START_ADDR + newB->numOfBlocks * MEM_BLOCK_SIZE);
        instance->freeMemHead->prev = instance->freeMemHead->next = nullptr;
        instance->freeMemHead->numOfBlocks = ((char*)HEAP_END_ADDR - (char*)instance->freeMemHead)/MEM_BLOCK_SIZE;
        instance->allocatedMemHead = nullptr;

        __putc('P');
        printInteger((uint64)(instance));
        __putc('\n');
    }
    void updateMemBlocks(void* del);
public:
    static MemoryAllocator* getInstance(){
        if(instance == nullptr){
            initialize();
        }
        return instance;
    }
    MemoryAllocator(MemoryAllocator const& ) = delete;
    void operator=(MemoryAllocator const&) = delete;
    //size number of blocks
    void* allocate(size_t size);
    int deallocate(void *block);
};

using FreeMem = MemoryAllocator::FreeMem;
using MemBlock = MemoryAllocator::MemBlock;

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
