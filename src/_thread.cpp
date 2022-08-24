//
// Created by os on 6/22/22.
//

#include "../h/_thread.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

_thread *_thread::running = nullptr;

uint64 _thread::timeSliceCounter = 0;
SleepList _thread::sleepQueue;


void _thread::dispatch() {
    _thread *old = running;
    if (!old->finished && !old->sleeping && !old->blocked) {
        Scheduler::put(old);
    }

    running = Scheduler::get();

    //provera za svaki slucaj
    /*if ((newT->isMain && newT->finished) || newT->finished || newT->sleeping || newT->blocked) {
        running = Scheduler::get();
    } else {
        running = newT;
    }*/
    _thread::contextSwitch(&old->context, &running->context);
}

void _thread::deallocateStack() {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    mem.deallocate(stack);
}

void _thread::threadWrapper() {
    if(!running->supervised){
        userRegime();
    }
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch(); //kada zavrsi svoj posao nit predaje nekoj sledecoj niti u scheduleru
}


_thread *_thread::createThread(_thread::Body body, uint64 *stackAddr, void *ar) {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    _thread *newThread = reinterpret_cast<_thread *>(mem.allocate(sizeof(_thread)));
    if (newThread == nullptr) {
        return nullptr;
    }
    newThread->stack = stackAddr;
    newThread->body = body;
    newThread->context.ra = (uint64) &threadWrapper;
    newThread->context.sp = newThread->stack != nullptr ? (uint64) &newThread->stack[STACK_SIZE] : 0; //sp pokazuje na poslednju zauzetu(stek raste ka nizim adr)
    newThread->timeSlice = TIME_SLICE;
    newThread->finished = false;
    newThread->arg = ar;
    newThread->sleeping = false;
    newThread->blocked = false;
    newThread->supervised = false;
    return newThread;
}

void _thread::set_time(time_t time) {
    this->time = time;
}


