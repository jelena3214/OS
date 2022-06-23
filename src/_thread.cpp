//
// Created by os on 6/22/22.
//

#include "../h/_thread.hpp"
#include "../h/riscv.hpp"

_thread *_thread::running = nullptr;

uint64 _thread::timeSliceCounter = 0;


void _thread::yield()
{
    __asm__ volatile ("ecall");
}

void _thread::dispatch()
{
    _thread *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();

    _thread::contextSwitch(&old->context, &running->context);
}

void _thread::threadWrapper()
{
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    _thread::yield();
}


_thread *_thread::createThread(_thread::Body body, uint64 *stackAddr) {
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    _thread* newThread = reinterpret_cast<_thread*>(mem.allocate(sizeof(_thread)));
    if(newThread == nullptr) {
        return nullptr;
    }
    newThread->stack = stackAddr;
    newThread->body = reinterpret_cast<void (*)()>(body);
    newThread->context.ra = (uint64) &threadWrapper;
    newThread->context.sp =  newThread->stack != nullptr ? (uint64) &newThread->stack[STACK_SIZE] : 0;
    newThread->timeSlice = TIME_SLICE;
    newThread->finished = false;

    return newThread;
}
