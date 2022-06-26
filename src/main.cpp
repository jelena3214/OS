//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_thread.hpp"

void* rutina(void *p){
    int *k = static_cast<int *>(p);
    printString("JEEEEEEEEEEEEEEEEEEEEEEEEEEEKAAAAAAAAAAAAAAAAAAAAAAA\n");
    printInteger(*k);
    return k;
}

void* rutina1(void *p){
    int *k = static_cast<int *>(p);
    printString("ACAACACAAAAAAAAAAAAAAAAAAAAA\n");
    printInteger(*k);
    return k;
}

void* idle(void* p){
    while(1);
}

void* userMain(void* p){
    Riscv::ms_sstatus(Riscv::BitMaskSstatus::SSTATUS_SPP); //prelazak u user mode
    uint64 t1 = 4;
    Thread* t = new Thread(reinterpret_cast<void (*)(void *)>(rutina), &t1);
    printString("STIGAOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n");
    t->start();

    return p;
}

int main() {
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine

    //__asm__ volatile("mv x16, %0" : : "r"(x));
    //__asm__ volatile("mv %0, x16" : "=r"(x1));

    //__asm__ volatile("ecall");
    _thread *threads[4];
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    threads[0] = _thread::createThread(nullptr, nullptr);
    _thread::running = threads[0];
    threads[0]->startThread();
    //threads[1] = _thread::createThread(reinterpret_cast<void (*)()>(rutina),
                                       //static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    //threads[1]->startThread();
    //threads[2] = _thread::createThread(reinterpret_cast<void (*)()>(rutina1),
                                       //static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    //threads[2]->startThread();
    threads[1] = _thread::createThread(reinterpret_cast<void (*)()>(idle),
                                       static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    threads[1]->startThread();
    threads[2] = _thread::createThread(reinterpret_cast<void (*)()>(userMain),
                                       static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    threads[2]->startThread();
    printString("NESTOOOOOOOOOOOOO");
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    /*

    _thread::yield();*/
    printString("DddddddddddddddddddddddddddddddddddddddddddddddddOSLI DO MAINA\n");
    int i = 1000;
    while(--i > 0);
    threads[0]->setFinished(true);

    return 0;

}