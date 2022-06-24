//
// Created by os on 6/24/22.
//

#include "../h/LinkedList.hpp"

void LinkedList::insertNode(_thread* data) {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    Node* newNode = reinterpret_cast<Node *>(mem.allocate(sizeof(Node)));
    newNode->next = nullptr;
    newNode->data = data;
    if (head == nullptr) {
        head = newNode;
        return;
    }

    // Traverse till end of list
    Node* temp = head;
    while (temp->next != nullptr) {

        // Update temp
        temp = temp->next;
    }

    // Insert at the last.
    temp->next = newNode;
}

_thread* LinkedList::deleteNode() {
    MemoryAllocator &mem = MemoryAllocator::getInstance();
    Node *temp1 = head;

    if (head == nullptr) {
        return nullptr;
    }
    head = head->next;
    _thread* ret = temp1->data;
    mem.deallocate(temp1);
    return ret;
}
