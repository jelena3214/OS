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
    MemoryAllocator() = default;
    static MemoryAllocator instance;
    static FreeMem* freeMemHead;
    static MemBlock* allocatedMemHead;
public:



    static MemoryAllocator& getInstance(){
        return instance;
    }
};

using FreeMem = MemoryAllocator::FreeMem;
using MemBlock = MemoryAllocator::MemBlock;

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
