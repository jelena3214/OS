//
// Created by os on 6/22/22.
//

#include "../h/Scheduler.hpp"

List<_thread> Scheduler::readyThreadQueue;

_thread *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(_thread *ccb)
{
    readyThreadQueue.addLast(ccb);
}
