//
// Created by os on 5/23/22.
//

#include "../h/syscall_c.hpp"
#include "../h/ABI.hpp"


//TODO vidi kako ces da vratis
void* mem_alloc (size_t size){
    struct FunctionParameters param;
    param.code = 0x01;
    int p = ((size + sizeof(Block))% MEM_BLOCK_SIZE != 0) ? 1 : 0;
    param.first = reinterpret_cast<void *>((size + sizeof(Block)) / MEM_BLOCK_SIZE + p);
    return syscall_handler(param);
}

int mem_free (void* allocatedBlock){
    struct FunctionParameters param;
    param.code = 0x02;
    param.first = allocatedBlock;
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    return (int)t;
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    struct FunctionParameters param;
    param.code = 0x11;
    void* stackAddr = mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));
    param.first = (void*)((uint64)handle);
    param.second = start_routine;
    param.third = arg;
    param.fourth = stackAddr;
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    return (int)t;
}

int thread_start(thread_t *handle){
    struct FunctionParameters param;
    param.code = 0x44;
    param.first = (void*)((uint64)handle);
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    return (int)t;
}

void userRegime(){
    struct FunctionParameters param;
    param.code = 0x26;
    syscall_handler(param);
}

void thread_dispatch (){
    struct FunctionParameters param;
    param.code = 0x13;
    syscall_handler(param);
}

int thread_exit(){
    struct FunctionParameters param;
    param.code = 0x12;
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    return (int)t;
}

void thread_deallocate(thread_t *handle){
    struct FunctionParameters param;
    param.code = 0x55;
    param.first = (void*)((uint64)handle);
    syscall_handler(param);
}


int time_sleep (time_t time){
    struct FunctionParameters param;
    param.code = 0x31;
    param.first = reinterpret_cast<void *>(time);
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    return (int)t;
}
