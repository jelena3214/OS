//
// Created by os on 6/29/22.
//

#ifndef PROJECT_BASE_BUFFER_HPP
#define PROJECT_BASE_BUFFER_HPP


#include "../h/syscall_c.hpp"
#include "../h/printing.hpp"

class Buffer {
private:
    int cap;
    int *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;
    sem_t mutexHead;
    sem_t mutexTail;

public:
    Buffer(int _cap);
    ~Buffer();

    void put(int val);
    int get();

    int getCnt();

};


#endif //PROJECT_BASE_BUFFER_HPP
