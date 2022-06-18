//
// Created by os on 5/23/22.
//
#include "../h/syscall_cpp.hpp"

MemoryAllocator* MemoryAllocator::instance = nullptr;

void MemoryAllocator::operator delete(void *allocatedBlock) {
    mem_free(allocatedBlock);
}

void *MemoryAllocator::operator new(size_t size)
{
    return mem_alloc(size);
}
