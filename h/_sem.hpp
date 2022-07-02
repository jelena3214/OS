//
// Created by os on 6/29/22.
//

#ifndef PROJECT_BASE__SEM_HPP
#define PROJECT_BASE__SEM_HPP

#include "../h/_thread.hpp"

class _sem {
private:
    unsigned val;
    LinkedList threadQueue;

    bool done;
public:
    _sem(int val){
        this->val = val;
    }

    static _sem* create_semaphore(unsigned v);

    int wait();
    int signal();
    int close();

    ~_sem(){
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        mem.deallocate(this);
    }
};


#endif //PROJECT_BASE__SEM_HPP
