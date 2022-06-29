//
// Created by os on 6/22/22.
//

#include "../h/_thread.hpp"
#include "../h/riscv.hpp"

_thread *_thread::running = nullptr;

uint64 _thread::timeSliceCounter = 0;
SleepList _thread::sleepQueue;

void _thread::yield()
{
    __asm__ volatile ("ecall");
}

void _thread::dispatch()
{
    _thread *old = running;
    if (!old->isFinished() && !old->sleeping) {
        Scheduler::put(old);
    }

    _thread* newT = Scheduler::get();
    if((newT->isMain && newT->finished) || newT->finished || newT->sleeping){
        running = Scheduler::get();
    }else{
        running = newT;
    }
    _thread::contextSwitch(&old->context, &running->context);
}

void _thread::deallocateStack(){
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    mem.deallocate(stack);
}

void _thread::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    _thread::yield();
}


_thread *_thread::createThread(_thread::Body body, uint64 *stackAddr, void* ar) {
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    _thread* newThread = reinterpret_cast<_thread*>(mem.allocate(sizeof(_thread)));
    if(newThread == nullptr) {
        return nullptr;
    }
    newThread->stack = stackAddr;
    newThread->body = body;
    newThread->context.ra = (uint64) &threadWrapper;
    newThread->context.sp =  newThread->stack != nullptr ? (uint64) &newThread->stack[STACK_SIZE] : 0;
    newThread->timeSlice = TIME_SLICE;
    newThread->finished = false;
    newThread->arg = ar;
    newThread->isMain = false;
    newThread->sleeping = false;
    return newThread;
}

