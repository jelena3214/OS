//
// Created by os on 6/28/22.
//

#ifndef PROJECT_BASE_SLEEPLIST_HPP
#define PROJECT_BASE_SLEEPLIST_HPP
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
class _thread;
typedef unsigned long time_t;
class SleepList{
private:
    struct Node{
        _thread* thread;
        Node* next;
        time_t time;
    };
    Node* head;

public:
    int put(_thread* th, time_t t);

    _thread* get();

    void printSleepList();

    void decTime();
};




#endif //PROJECT_BASE_SLEEPLIST_HPP
