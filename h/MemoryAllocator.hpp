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
    struct Block{
        Block* prev,*next;
        size_t numOfBlocks;
    };

    static const size_t headerSize = sizeof(Block); //sizeof(MemBlock) == sizeof(FreeMem)
private:
    MemoryAllocator(){}
    static MemoryAllocator* instance;
    Block* freeMemHead;
    Block* allocatedMemHead;
    static void initialize(){
        Block* newB = reinterpret_cast<Block*>(const_cast<void*>(HEAP_START_ADDR));
        newB->next = newB->prev = nullptr;
        newB->numOfBlocks = (sizeof(MemoryAllocator) + headerSize)/MEM_BLOCK_SIZE + ((sizeof(MemoryAllocator) + headerSize)%MEM_BLOCK_SIZE == 0?0:1);
        instance = reinterpret_cast<MemoryAllocator*>((char*)newB + headerSize); //nakon header-a je instance
        instance->freeMemHead = reinterpret_cast<Block *>((char*)newB + (size_t)(newB->numOfBlocks * MEM_BLOCK_SIZE));
        instance->freeMemHead->prev = instance->freeMemHead->next = nullptr;
        instance->freeMemHead->numOfBlocks = (size_t)((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - newB->numOfBlocks*MEM_BLOCK_SIZE)/MEM_BLOCK_SIZE;
        printString("");
        printInteger((size_t)(size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - (size_t)newB->numOfBlocks*MEM_BLOCK_SIZE);
        __putc('\n');
        instance->allocatedMemHead = nullptr;
    }
    void updateMemBlocks(void* del);
    void joinFreeSpace();

public:
    void ispisAlloc();
    void ispisFree();
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

using Block = MemoryAllocator::Block;

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
