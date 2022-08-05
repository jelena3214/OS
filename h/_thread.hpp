//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASE__THREAD_HPP
#define PROJECT_BASE__THREAD_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"
#include "../h/sleepList.hpp"

class _thread {
public:
    ~_thread() {
        this->finished = true;
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        if(stack)deallocateStack();
        mem.deallocate(this);
    }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    void setSleeping(bool value) { sleeping = value; }

    void setMain(bool value) { isMain = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static _thread *createThread(Body body, uint64* stackAddr, void* ar);

    static void yield();

    static _thread *running;

    int startThread(){
        if(body != nullptr){
            Scheduler::put(this);
            return 1;
        }
        return 0;
    }

    static SleepList sleepQueue;
private:
    void deallocateStack();

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice; //broj perioda koji dobija neka nit svaki put kad joj se da procesor
    bool finished, sleeping;
    bool isMain;

    void* arg;

    friend class Riscv;
    friend class _sem;
    friend class _console;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;

};


#endif //PROJECT_BASE__THREAD_HPP
