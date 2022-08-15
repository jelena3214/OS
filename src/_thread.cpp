//
// Created by os on 6/22/22.
//

#include "../h/_thread.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

_thread *_thread::running = nullptr;

uint64 _thread::timeSliceCounter = 0;
SleepList _thread::sleepQueue;


//TODO ovo je micko koristio kao dispatch jer je njemu prekidna ulazila samo i menjala kontekst
void _thread::yield()
{
    __asm__ volatile ("ecall");
}

void _thread::dispatch()
{
    _thread *old = running;
    if (!old->isFinished() && !old->sleeping && !old->blocked) {
        Scheduler::put(old);
    }

    _thread* newT = Scheduler::get();

    //TODO vidi da li ovo treba, jer logicki nit kada se uspava ona nije u scheduleru i nije zavrsila kao je u redu
    //ovo za main isto?
    if((newT->isMain && newT->finished) || newT->finished || newT->sleeping || newT->blocked){
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
    userRegime();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}
//kada zavrsi svoj posao nit predaje nekoj sledecoj niti u scheduleru


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
    newThread->blocked = false;
    return newThread;
}

void _thread::set_time(time_t time) {
    this->time = time;
}


