//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_thread.hpp"

void* rutina(void *p){
    __putc('S');
    __putc('\n');
    __putc('S');
    __putc('\n');
    __putc('S');
    __putc('\n');
    __putc('S');
    __putc('\n');
    return p;
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
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    //uint64 t1 = 4;
    //Thread t(reinterpret_cast<void (*)(void *)>(rutina), &t1);
    //t.start();
    return p;
}

int main() {
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::ms_sie(Riscv::SIE_SSIE);
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine

    //__asm__ volatile("mv x16, %0" : : "r"(x));
    //__asm__ volatile("mv %0, x16" : "=r"(x1));

    //__asm__ volatile("ecall");

    MemoryAllocator& mem = MemoryAllocator::getInstance();
    _thread* userM = _thread::createThread(reinterpret_cast<void (*)(void*)>(userMain),
                                              static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);

    _thread* main = _thread::createThread(nullptr, nullptr, nullptr);
    main->startThread();
    userM->startThread();

    printString("PROSAO USEEEEEEEEEEEEER");
    _thread::running = main;
    userRegime();

    return 0;

}