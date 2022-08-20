//
// Created by os on 6/29/22.
//

#include "../h/_sem.hpp"
#include "../h/syscall_c.hpp"

_sem *_sem::create_semaphore(int v) {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    _sem *newSemaphore = reinterpret_cast<_sem *>(mem.allocate(sizeof(_sem)));
    if (newSemaphore == nullptr) {
        return nullptr;
    }
    newSemaphore->val = v;
    newSemaphore->done = false;
    newSemaphore->threadQueue.head = nullptr;
    return newSemaphore;
}

int _sem::wait() {
    if (done)return -1;
    _thread *old = _thread::running;
    if (--val < 0) {
        threadQueue.insertNode(old);
        old->setBlocked(true);
        thread_dispatch();
        if(done)return -1;
    }
    return 0; //uspeh
}

int _sem::signal() {
    if (done)return -1;
    if (++val <= 0) {
        _thread *th = threadQueue.deleteNode();
        if (!th->isFinished()) {
            th->setBlocked(false);
            Scheduler::put(th);
        }
    }
    return 0; //uspeh
}

int _sem::close() {
    while (1) {
        _thread *tmp = threadQueue.deleteNode();
        if (tmp) {
            Scheduler::put(tmp);
        } else {
            break;
        }
    }
    done = true;
    return 0;
}

