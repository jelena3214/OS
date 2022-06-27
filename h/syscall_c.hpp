//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_HPP
#define PROJECT_BASE_SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"

void *mem_alloc(size_t size);

int mem_free(void *allocatedBlock);

class _thread;

typedef _thread *thread_t;

int thread_create(
        thread_t *handle,
        void(*start_routine)(void *),
        void *arg
);

void userRegime();

int thread_exit();

int thread_start(thread_t *handle);

void thread_dispatch ();

typedef unsigned long time_t;
int time_sleep (time_t);

#endif //PROJECT_BASE_SYSCALL_C_HPP
