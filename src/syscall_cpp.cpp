//
// Created by os on 5/23/22.
//
#include "../h/syscall_cpp.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}
