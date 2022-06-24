//
// Created by os on 6/22/22.
//

#include "../h/Scheduler.hpp"
#include "../h/print.hpp"
LinkedList Scheduler::queue;

_thread *Scheduler::get()
{
    //return readyThreadQueue.removeFirst();
    return queue.deleteNode();
}

void Scheduler::put(_thread *ccb)
{
    //readyThreadQueue.addLast(ccb);
    queue.insertNode(ccb);
    printString("UBAIOOOO\n");
}
