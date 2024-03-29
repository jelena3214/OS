//
// Created by os on 5/23/22.
//

#include "../h/syscall_c.hpp"
#include "../h/ABI.hpp"

void *mem_alloc(size_t size) {
    struct FunctionParameters param;
    param.code = 0x01;
    int p = ((size + sizeof(Block)) % MEM_BLOCK_SIZE != 0) ? 1 : 0;
    param.first = reinterpret_cast<void *>((size + sizeof(Block)) / MEM_BLOCK_SIZE + p);
    return syscall_handler(param);
}

int mem_free(void *allocatedBlock) {
    struct FunctionParameters param;
    param.code = 0x02;
    param.first = allocatedBlock;
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

int thread_create(thread_t *handle, void(*start_routine)(void *), void *arg) {
    struct FunctionParameters param;
    param.code = 0x11;
    void *stackAddr = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    param.first = (void *) ((uint64) handle);
    param.second = start_routine;
    param.third = arg;
    param.fourth = stackAddr;
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    if (!t)thread_start(handle); //c api thread_create pokrene nit, ako nije doslo do greske
    return (int) t;
}

int thread_init_handle(thread_t *handle, void(*start_routine)(void *), void *arg) { //init handle bez startovanja niti
    struct FunctionParameters param;
    param.code = 0x11;
    void *stackAddr = mem_alloc(DEFAULT_STACK_SIZE * sizeof(uint64));
    param.first = (void *) ((uint64) handle);
    param.second = start_routine;
    param.third = arg;
    param.fourth = stackAddr;
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

void set_period(thread_t *handle, time_t period) {
    struct FunctionParameters param;
    param.code = 0x88;
    param.first = (void *) ((uint64) handle);
    param.third = (void *) period;
    syscall_handler(param);
}

void sleep_periodic_thread(thread_t *handle) {
    struct FunctionParameters param;
    param.code = 0x99;
    param.first = (void *) ((uint64) handle);
    syscall_handler(param);
}


int thread_start(thread_t *handle) {
    struct FunctionParameters param;
    param.code = 0x44;
    param.first = (void *) ((uint64) handle);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

void userRegime() {
    struct FunctionParameters param;
    param.code = 0x26;
    syscall_handler(param);
}


void thread_dispatch() {
    struct FunctionParameters param;
    param.code = 0x13;
    syscall_handler(param);
}

int thread_exit() {
    struct FunctionParameters param;
    param.code = 0x12;
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

void thread_deallocate(thread_t *handle) {
    struct FunctionParameters param;
    param.code = 0x55;
    param.first = (void *) ((uint64) handle);
    syscall_handler(param);
}


int time_sleep(time_t time) {
    struct FunctionParameters param;
    param.code = 0x31;
    param.first = reinterpret_cast<void *>(time);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}


int sem_open(sem_t *handle, unsigned init) {
    struct FunctionParameters param;
    param.code = 0x21;
    param.first = (void *) ((uint64) handle);
    param.third = reinterpret_cast<void *>(init);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

int sem_close(sem_t handle) {
    struct FunctionParameters param;
    param.code = 0x22;
    param.first = (void *) ((uint64) handle);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

int sem_wait(sem_t id) {
    struct FunctionParameters param;
    param.code = 0x23;
    param.first = (void *) ((uint64) id);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

int sem_signal(sem_t id) {
    struct FunctionParameters param;
    param.code = 0x24;
    param.first = (void *) ((uint64) id);
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (int) t;
}

void sem_deallocate(sem_t *handle) {
    struct FunctionParameters param;
    param.code = 0x77;
    param.first = (void *) ((uint64) handle);
    syscall_handler(param);
}


char getc() {
    struct FunctionParameters param;
    param.code = 0x41;
    void *ret = syscall_handler(param);
    uint64 t = (uint64) (ret);
    return (char) t;
}

void putc(char c) {
    struct FunctionParameters param;
    param.code = 0x42;
    param.first = reinterpret_cast<void *>(c);
    syscall_handler(param);
}
