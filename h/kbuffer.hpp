//
// Created by os on 7/2/22.
//

#ifndef PROJECT_BASE_KBUFFER_HPP
#define PROJECT_BASE_KBUFFER_HPP
#include "_sem.hpp"
#include "MemoryAllocator.hpp"

class kbuffer {
public:
    /*kbuffer(int cap = BUFFER_SIZE): mutexHead(1), mutexTail(1), itemAvailable(0), spaceAvailable(cap){
        this->cap = cap;
        head = tail = 0;
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        buffer = static_cast<int *>(mem.allocate(sizeof(int) * cap));
    }*/

    static kbuffer* create_buffer(int capa = BUFFER_SIZE){
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        kbuffer* newBuffer = static_cast<kbuffer *>(mem.allocate(sizeof(kbuffer)));
        newBuffer->buffer = static_cast<int *>(mem.allocate(sizeof(int) * capa));
        newBuffer->head = newBuffer->tail = 0;
        newBuffer->cap = capa;
        newBuffer->mutexHead = _sem::create_semaphore(1);
        newBuffer->mutexTail = _sem::create_semaphore(1);
        newBuffer->itemAvailable = _sem::create_semaphore(0);
        newBuffer->spaceAvailable = _sem::create_semaphore(capa);
        return newBuffer;
    }

    void put(int val);
    int get();
    ~kbuffer();

    bool empty(){
        return head == tail;
    }

    static const int BUFFER_SIZE = 1024;
private:

    int cap, head, tail;
    int* buffer;
    _sem* mutexHead, *mutexTail, *itemAvailable, *spaceAvailable;
};


#endif //PROJECT_BASE_KBUFFER_HPP
