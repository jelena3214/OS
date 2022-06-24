//
// Created by os on 6/24/22.
//

#ifndef PROJECT_BASE_LINKEDLIST_HPP
#define PROJECT_BASE_LINKEDLIST_HPP
#include "../h/MemoryAllocator.hpp"

class _thread;
class Node {
public:
    _thread* data;
    Node* next;
};

class LinkedList {
    Node* head;
public:
    // Default constructor
    LinkedList() { head = nullptr;}


    void insertNode(_thread* data);

    _thread* deleteNode();
};


#endif //PROJECT_BASE_LINKEDLIST_HPP
