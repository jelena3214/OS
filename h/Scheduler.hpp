//
// Created by os on 6/22/22.
//

#ifndef PROJECT_BASE_SCHEDULER_HPP
#define PROJECT_BASE_SCHEDULER_HPP
#include "../h/list.hpp"


class _thread;

class Scheduler {
private:
    static List<_thread> readyThreadQueue;

public:
    static _thread *get();

    static void put(_thread *ccb);

};


#endif //PROJECT_BASE_SCHEDULER_HPP
