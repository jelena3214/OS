//
// Created by os on 6/18/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

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
        //freeMemHead->prev = freeMemHead->next = nullptr;
        //freeMemHead->numOfBlocks = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR)/MEM_BLOCK_SIZE;
    }
    static FreeMem* freeMemHead;
    static MemBlock* allocatedMemHead;

    void updateMemBlocks(void* del);
public:
    static MemoryAllocator& getInstance(){
        static MemoryAllocator instance;
        return instance;
    }
    MemoryAllocator(MemoryAllocator const& ) = delete;
    void operator=(MemoryAllocator const&) = delete;
    //size number of blocks
    void* allocate(size_t size);
    void deallocate(void *block);
};

using FreeMem = MemoryAllocator::FreeMem;
using MemBlock = MemoryAllocator::MemBlock;

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
