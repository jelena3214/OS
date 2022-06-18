//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../lib/mem.h"

void* mem_alloc (size_t size);
int mem_free (void* allocatedBlock);

#endif //PROJECT_BASE_SYSCALL_C_HPP
