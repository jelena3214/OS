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
    //treba unistiti stek i myhandle
}

int Thread::start() {
    return thread_start(&myHandle); //MOZE OVAKO?
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

Thread::Thread() {

}
