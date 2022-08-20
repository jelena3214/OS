//
// Created by os on 6/22/22.
//

#include "../h/Scheduler.hpp"

LinkedList Scheduler::queue;

_thread *Scheduler::get() {
    _thread *th = queue.deleteNode();
    return th;
}

void Scheduler::put(_thread *ccb) {
    queue.insertNode(ccb);
}
