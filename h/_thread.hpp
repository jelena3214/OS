//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASE__THREAD_HPP
#define PROJECT_BASE__THREAD_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"

class _thread {
public:
    ~_thread() {
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        mem.deallocate(stack);
        mem.deallocate(this);
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)();

    static _thread *createThread(Body body, uint64* stackAddr);

    static void yield();

    static _thread *running;

    int startThread(){
        if(body != nullptr){
            Scheduler::put(this);
            return 1;
        }
        return 0;
    }

private:
    _thread(Body body, uint64* stackAddr) :
    body(body),
            stack(body != nullptr ? stackAddr : nullptr),
    context({(uint64) &threadWrapper,
                stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
    }),
    timeSlice(TIME_SLICE),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }



    //TODO sta raditi sa a0 i a1??
    struct Context
    {
        uint64 ra;
        uint64 sp;
        uint64 gp;
        uint64 tp;
        uint64 t0;
        uint64 t1;
        uint64 t2;
        uint64 s0;
        uint64 s1;
        uint64 a0;
        uint64 a1;
        uint64 a2;
        uint64 a3;
        uint64 a4;
        uint64 a5;
        uint64 a6;
        uint64 a7;
        uint64 s2;
        uint64 s3;
        uint64 s4;
        uint64 s5;
        uint64 s6;
        uint64 s7;
        uint64 s8;
        uint64 s9;
        uint64 s10;
        uint64 s11;
        uint64 t3;
        uint64 t4;
        uint64 t5;
        uint64 t6;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;

};


#endif //PROJECT_BASE__THREAD_HPP
