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

    int startThread();

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


    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice; //broj perioda koji dobija neka nit svaki put kad joj se da procesor
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
