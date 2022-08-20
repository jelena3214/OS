//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_ABI_HPP
#define PROJECT_BASE_ABI_HPP

#include "../h/riscv.hpp"

using Function = void (*)(void *);

struct FunctionParameters {
    uint64 code;
    void *first;
    Function second; //na ovaj nacin prenosimo funkciju za nit
    void *third;
    void *fourth;
};

void *syscall_handler(struct FunctionParameters params);

#endif //PROJECT_BASE_ABI_HPP
