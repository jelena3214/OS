//
// Created by os on 6/22/22.
//

#include "../h/Scheduler.hpp"
#include "../h/print.hpp"
LinkedList Scheduler::queue;

_thread *Scheduler::get()
{
    //return readyThreadQueue.removeFirst();
    //printString("UZIMANJE: \n");
    _thread* th =  queue.deleteNode();
    //queue.printList();
    return th;
}

void Scheduler::put(_thread *ccb)
{
    //readyThreadQueue.addLast(ccb);
    //printString("Stavi\n");
    queue.insertNode(ccb);
    //queue.printList();
}
