//
// Created by os on 6/29/22.
//

#ifndef PROJECT_BASE_THREADS_C_API_TEST_HPP
#define PROJECT_BASE_THREADS_C_API_TEST_HPP

#include "../h/syscall_c.hpp"

#include "../h/print.hpp"

bool finishedA = false;
bool finishedB = false;
bool finishedC = false;
bool finishedD = false;

uint64 fibonacci(uint64 n) {
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void* arg) {
    for (uint64 i = 0; i < 10; i++) {
        printString("A: i="); printInteger(i); printString("\n");
        for (uint64 j = 0; j < 100; j++) {
            for (uint64 k = 0; k < 300; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printString("A finished!\n");
    finishedA = true;
}

void workerBodyB(void* arg) {
    for (uint64 i = 0; i < 16; i++) {
        printString("B: i="); printInteger(i); printString("\n");
        for (uint64 j = 0; j < 100; j++) {
            for (uint64 k = 0; k < 300; k++) { /* busy wait */ }
            thread_dispatch();
        }
    }
    printString("B finished!\n");
    finishedB = true;
    thread_dispatch();
}

void workerBodyC(void* arg) {
    uint8 i = 0;
    for (; i < 3; i++) {
        printString("C: i="); printInteger(i); printString("\n");
    }

    printString("C: dispatch\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("C: t1="); printInteger(t1); printString("\n");

    uint64 result = fibonacci(12);
    printString("C: fibonaci="); printInteger(result); printString("\n");

    for (; i < 6; i++) {
        printString("C: i="); printInteger(i); printString("\n");
    }

    printString("C finished!\n");
    finishedC = true;
    thread_dispatch();
}

void workerBodyD(void* arg) {
    uint8 i = 10;
    for (; i < 13; i++) {
        printString("D: i="); printInteger(i); printString("\n");
    }

    printString("D: dispatch\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(16);
    printString("D: fibonaci="); printInteger(result); printString("\n");

    for (; i < 16; i++) {
        printString("D: i="); printInteger(i); printString("\n");
    }

    printString("D finished!\n");
    finishedD = true;
    thread_dispatch();
}


void Threads_C_API_test() {
    thread_t threads[4];
    thread_create(&threads[0], workerBodyA, nullptr);
    printString("ThreadA created\n");
    thread_start(&threads[0]);

    thread_create(&threads[1], workerBodyB, nullptr);
    printString("ThreadB created\n");
    thread_start(&threads[1]);

    thread_create(&threads[2], workerBodyC, nullptr);
    printString("ThreadC created\n");
    thread_start(&threads[2]);

    thread_create(&threads[3], workerBodyD, nullptr);
    printString("ThreadD created\n");
    thread_start(&threads[3]);



    while (!(finishedA && finishedB && finishedC && finishedD)) {
        thread_dispatch();
    }

}


#endif //PROJECT_BASE_THREADS_C_API_TEST_HPP
