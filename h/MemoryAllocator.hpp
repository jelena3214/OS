//
// Created by os on 6/18/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"


class MemoryAllocator {
public:
    struct Block {
        Block *prev, *next;
        size_t numOfBlocks;
    };

    static const size_t headerSize = sizeof(Block); //sizeof(MemBlock) == sizeof(FreeMem)
private:
    MemoryAllocator() {
        freeMemHead = reinterpret_cast<Block *>(const_cast<void *>(HEAP_START_ADDR));
        freeMemHead->next = freeMemHead->prev = nullptr;
        freeMemHead->numOfBlocks = ((size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR) / MEM_BLOCK_SIZE;
        allocatedMemHead = nullptr;
    }

    Block *freeMemHead;
    Block *allocatedMemHead;

    void updateMemBlocks(void *del);

    int tryToJoin(Block *cur);

public:

    static MemoryAllocator &getInstance() {
        static MemoryAllocator instance;
        return instance;
    }

    void *allocate(size_t size);

    int deallocate(void *block);

    MemoryAllocator(const MemoryAllocator &) = delete;
    //MemoryAllocator& operator=(MemoryAllocator) = delete;
};

using Block = MemoryAllocator::Block;

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
