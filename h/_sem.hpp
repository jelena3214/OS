//
// Created by os on 6/29/22.
//

#ifndef PROJECT_BASE__SEM_HPP
#define PROJECT_BASE__SEM_HPP

#include "../h/_thread.hpp"

class _sem {
private:
    int val;
    LinkedList threadQueue;

    volatile bool done;

    _sem(int val) {
        this->val = val;
    }

public:

    static _sem *create_semaphore(int v);

    int wait();

    int signal();

    int close();

    ~_sem() {
        MemoryAllocator &mem = MemoryAllocator::getInstance();
        mem.deallocate(this);
    }
};


#endif //PROJECT_BASE__SEM_HPP
