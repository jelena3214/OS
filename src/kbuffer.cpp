//
// Created by os on 7/2/22.
//

#include "../h/kbuffer.hpp"
#include "../h/print.hpp"

//Da li je potrebno mutex 2 puta?

void kbuffer::put(int val) {

    spaceAvailable->wait();
    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    mutexTail->signal();

    itemAvailable->signal();


}

int kbuffer::get() {
    itemAvailable->wait();

    mutexHead->wait();

    int ret = buffer[head];
    head = (head + 1) % cap;
    mutexHead->signal();

    spaceAvailable->signal();

    return ret;
}

kbuffer::~kbuffer() {
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    mem.deallocate(buffer);
}
