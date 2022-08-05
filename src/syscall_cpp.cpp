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

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
    thread_deallocate(&myHandle);
}

int Thread::start() {
    return thread_start(&myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

void wrapper(void* thread){
    if(thread != nullptr){
        ((Thread*)thread)->run();
    }
}

Thread::Thread() {
    thread_create(&myHandle, wrapper, this);//create thread za run funkciju, a za run neki wrapper izvan klase?
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
    sem_deallocate(&myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

