//
// Created by os on 6/28/22.
//

#ifndef PROJECT_BASE_SLEEPLIST_HPP
#define PROJECT_BASE_SLEEPLIST_HPP
#include "../lib/hw.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
class _thread;

class SleepList{
private:
    struct Node{
        _thread* thread;
        Node* next;
        uint64 time;
    };
    Node* head;

public:
    int put(_thread* th, uint64 t){
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        Node* newNode = static_cast<Node *>(mem.allocate(sizeof(Node)));
        if(newNode == nullptr)return -1;
        newNode->thread = th;
        if(!head){
            head = newNode;
            head->time = t;
        }else{
            uint64 firstTime = head->time;
            if(firstTime >= t){
                uint64 newNodeTime = firstTime - t;
                Node *p = head->next;
                Node *prev = head;
                while(p && p->time < newNodeTime){
                    prev = p;
                    p = p->next;
                }
                prev->next = newNode;
                newNode->next = p;
            }else{
                newNode->next = head;
                head->time = (t - head->time);
                head = newNode;
            }

        }
        return 0;
    }

    _thread* get(){
        if(head){
            head->time -= 1;
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
};




#endif //PROJECT_BASE_SLEEPLIST_HPP
