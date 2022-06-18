//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_ABI_H
#define PROJECT_BASE_ABI_H
#include "../lib/hw.h"
#include "../lib/mem.h"


struct FunctionParameters{
    size_t code;
    void* first;
};

void* syscall_handler(struct FunctionParameters params);

#endif //PROJECT_BASE_ABI_H
