//
// Created by os on 7/2/22.
//

#include "../h/kbuffer.hpp"
#include "../h/_console.hpp"

//TODO Da li je potrebno mutex 2 puta? Jer samo jedna nit stavlja i jedna uzima
//DA ZATO STO NPR BAFER ZA ISPIS, JEDNA NIT MOZE AKTIVNO DA STAVLJA U TAJ BAFER(ZELI DA PRINTUJE) I ONA CE SE BLOKIRATI SAMO NA MUTEX JDNOM
//DOK KONTROLER NE MORA DA CEKA DA BI UZEO IZ TOG BUFFERA

void kbuffer::put(int val) {
    spaceAvailable->wait();
    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % cap;

    mutexTail->signal();

    itemAvailable->signal();
}

int kbuffer::get() {
    int r = itemAvailable->wait();

    r = mutexHead->wait();

    int ret = buffer[head];
    head = (head + 1) % cap;

    r = mutexHead->signal();

    r = spaceAvailable->signal();

    if(r < 0) return r;

    return ret;
}

kbuffer::~kbuffer() {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    mem.deallocate(buffer);
}
