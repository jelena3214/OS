//
// Created by os on 5/23/22.
//
#include "../h/syscall_cpp.hpp"
void* operator new (size_t y){
    return mem_alloc(y);
}

void operator delete (void* y) noexcept{
    mem_free(y);
}

void *operator new[](size_t n){
    return mem_alloc(n);
}

void operator delete[](void *p) noexcept{
    mem_free(p);
}
