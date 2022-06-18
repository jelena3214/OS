//
// Created by os on 6/18/22.
//

#include "../h/MemoryAllocator.hpp"

FreeMem* MemoryAllocator::freeMemHead = (FreeMem*)HEAP_START_ADDR;