//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_ABI_HPP
#define PROJECT_BASE_ABI_HPP

#include "../h/riscv.hpp"
#include "../lib/console.h"
struct FunctionParameters{
    size_t code;
    void* first;
};

void* syscall_handler(struct FunctionParameters params);

extern "C" void ecall();

#endif //PROJECT_BASE_ABI_HPP
