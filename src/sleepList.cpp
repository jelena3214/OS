//
// Created by os on 6/29/22.
//
#include "../h/sleepList.hpp"

int SleepList::put(_thread* th, time_t t){
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    Node* newNode = static_cast<Node *>(mem.allocate(sizeof(Node)));
    if(newNode == nullptr)return -1;
    newNode->thread = th;
    if(!head){
        head = newNode;
        head->time = t;
        head->next = nullptr;
    }else{
        time_t firstTime = head->time;
        if(firstTime <= t){
            time_t newNodeTime = t - firstTime;
            Node *p = head->next;
            Node *prev = head;
            while(p && p->time < newNodeTime){
                prev = p;
                p = p->next;
            }
            prev->next = newNode;
            newNode->next = p;
            newNode->time = newNodeTime;
        }else{
            newNode->next = head;
            newNode->time = t;
            head->time = (head->time - t);
            head = newNode;
        }

    }
    return 0;
}

void SleepList::decTime(){
    Node* p = head;
    if(p){
        p->time -= 1;
    }
}

_thread* SleepList::get(){
    if(head){
        if(head->time == 0){
            Scheduler::put(head->thread);
            _thread* ret = head->thread;
            head = head->next;
            return ret;
        }else{
            return nullptr;
        }
    }
    return nullptr;
}

void SleepList::printSleepList(){
    Node* p = head;
    //printString("LIST uspavanih\n");
    while(p){
        //printInteger((time_t)p->time);
        //printString("\n");
        p = p->next;
    }
}