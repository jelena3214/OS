//
// Created by os on 6/29/22.
//

#include "../h/_sem.hpp"

_sem *_sem::create_semaphore(unsigned int v) {
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    _sem* newSemaphore = reinterpret_cast<_sem*>(mem.allocate(sizeof(_sem)));
    if(newSemaphore == nullptr) {
        return nullptr;
    }
    newSemaphore->val = v;
    newSemaphore->done = false;
    return newSemaphore;
}

int _sem::wait() {
    if(done)return -1;
    _thread* old = _thread::running;
    if(--val < 0){
        threadQueue.insertNode(old);
    }else{
        Scheduler::put(old);
    }
    _thread* newT = _thread::running = Scheduler::get();
    if(old != newT){
        _thread::contextSwitch(&old->context, &newT->context);
    }
    return 0; //success
}

int _sem::signal() {
    if(done)return -1;
    if(++val <= 0){
        Scheduler::put(threadQueue.deleteNode());
    }
    _thread* old = _thread::running;
    Scheduler::put(old);
    _thread* newT = Scheduler::get();
    if(old != newT){
        _thread::contextSwitch(&old->context, &newT->context);
    }
    return 0; //success
}

int _sem::close() {
    while(1){
        _thread* tmp = threadQueue.deleteNode();
        if(tmp){
            Scheduler::put(tmp);
        }else{
            break;
        }
    }
    done = true;
    return 0;
}

