//
// Created by os on 5/23/22.
//
#include "../h/syscall_cpp.hpp"
void* operator new (size_t y){
    return mem_alloc(y);
}

void operator delete (void* y){
    mem_free(y);
}
