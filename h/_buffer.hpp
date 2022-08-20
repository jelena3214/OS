//
// Created by os on 7/2/22.
//

#ifndef PROJECT_BASE__BUFFER_HPP
#define PROJECT_BASE__BUFFER_HPP

#include "_sem.hpp"
#include "MemoryAllocator.hpp"

class _buffer {
public:

    static _buffer *create_buffer(int capa = BUFFER_SIZE) {
        MemoryAllocator &mem = MemoryAllocator::getInstance();
        _buffer *newBuffer = static_cast<_buffer *>(mem.allocate(sizeof(_buffer)));
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

    ~_buffer();

    bool empty() {
        return head == tail;
    }

    static const int BUFFER_SIZE = 1024;
private:

    int cap, head, tail;
    int *buffer;
    _sem *mutexHead, *mutexTail, *itemAvailable, *spaceAvailable;
};


#endif //PROJECT_BASE__BUFFER_HPP
