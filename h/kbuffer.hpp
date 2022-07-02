//
// Created by os on 7/2/22.
//

#ifndef PROJECT_BASE_KBUFFER_HPP
#define PROJECT_BASE_KBUFFER_HPP
#include "_sem.hpp"
#include "MemoryAllocator.hpp"

class kbuffer {
public:
    kbuffer(int cap = BUFFER_SIZE): mutexHead(1), mutexTail(1), itemAvailable(0), spaceAvailable(cap){
        this->cap = cap;
        head = tail = 0;
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        buffer = static_cast<int *>(mem.allocate(sizeof(int) * cap));
    }

    void put(int val);
    int get();
    ~kbuffer();

    static const int BUFFER_SIZE = 1024;
private:
    int cap, head, tail;
    int* buffer;
    _sem mutexHead, mutexTail, itemAvailable, spaceAvailable;
};


#endif //PROJECT_BASE_KBUFFER_HPP
