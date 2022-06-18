//
// Created by os on 5/23/22.
//

#include "../h/syscall_c.hpp"
#include "../h/ABI.hpp"
//TODO vidi kako ces da vratis
void* mem_alloc (size_t size){
    struct FunctionParameters param;
    param.code = 0x01;
    param.first = (void *) ((size) / MEM_BLOCK_SIZE + ((size % MEM_BLOCK_SIZE != 0) ? 1 : 0));
    return syscall_handler(param);
}

int mem_free (void* allocatedBlock){
    struct FunctionParameters param;
    param.code = 0x02;
    param.first = allocatedBlock;
    return *((int*) syscall_handler(param));
}
