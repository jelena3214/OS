//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASE__THREAD_HPP
#define PROJECT_BASE__THREAD_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"

class _thread {
public:
    ~_thread() { delete[] stack; } //?

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)();

    static _thread *createThread(Body body);

    static void yield();

    static _thread *running;

private:
    _thread(Body body, uint64 timeSlice) :
    body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
    context({(uint64) &threadWrapper,
                stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
    }),
    timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    //PCB cuvaj sve registre
    struct Context
    {
        uint64 ra;
        uint64 sp;
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
