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
    param.first = (size + sizeof(Block)) / MEM_BLOCK_SIZE + p;
    printString("U C API: ");
    printInteger(param.first);
    __putc('\n');
    return syscall_handler(param);
}

int mem_free (void* allocatedBlock){
    struct FunctionParameters param;
    param.code = 0x02;
    param.first = reinterpret_cast<size_t>(allocatedBlock);
    void* ret = syscall_handler(param);
    uint64 t = (uint64)(ret);
    printInteger(t);
    return (int)t;
}
