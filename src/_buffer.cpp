//
// Created by os on 7/2/22.
//

#include "../h/_buffer.hpp"
#include "../h/_console.hpp"

//TODO Da li je potrebno mutex 2 puta? Jer samo jedna nit stavlja i jedna uzima
//DA ZATO STO NPR BAFER ZA ISPIS, JEDNA NIT MOZE AKTIVNO DA STAVLJA U TAJ BAFER(ZELI DA PRINTUJE) I ONA CE SE BLOKIRATI SAMO NA MUTEX JDNOM
//DOK KONTROLER NE MORA DA CEKA DA BI UZEO IZ TOG BUFFERA

void _buffer::put(int val) {
    spaceAvailable->wait();
    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    numOfElems++;
    mutexTail->signal();
    itemAvailable->signal();
}

int _buffer::get() {
    //proveravamo da li je neki wait/signal vratio negativnu vrednost jer je to greska
    int r = itemAvailable->wait();
    if (r < 0) return r;
    r = mutexHead->wait();
    if (r < 0) return r;
    int ret = buffer[head];
    head = (head + 1) % cap;
    numOfElems--;
    r = mutexHead->signal();
    if (r < 0) return r;
    r = spaceAvailable->signal();
    if (r < 0) return r;
    return ret;
}

_buffer::~_buffer() {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    mem.deallocate(buffer);
    mem.deallocate(this);
}
