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

class _sem;
typedef _sem* sem_t;

int thread_create(
        thread_t *handle,
        void(*start_routine)(void *),
        void *arg
);

void userRegime();

void thread_deallocate(thread_t *handle);

int thread_exit();

int thread_start(thread_t *handle);

void thread_dispatch ();

typedef unsigned long time_t;
int time_sleep (time_t);

int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

void sem_deallocate(sem_t* handle);

#endif //PROJECT_BASE_SYSCALL_C_HPP
