//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_thread.hpp"
#include "../h/print.hpp"

extern void userMain();

void* idle(void* p){
    while(1){
        thread_dispatch();
    }
}

void* userMa(void* p){
    putc('a');
    putc('b');
    putc('s');
    char s  = 0;
    while(1){
        s = getc();
        putc(s);
        time_sleep(30);
        //putc(s);
        //printInteger(s);
    }
    return p;
}

void* mainWrapper(void* p){
    userRegime();
    userMa(nullptr);
    return p;
}

int main() {
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::mc_sie(Riscv::SIE_SSIE);
    Riscv::w_sip(0);
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine


    MemoryAllocator& mem = MemoryAllocator::getInstance();
    _thread* userM = _thread::createThread(reinterpret_cast<void (*)(void*)>(mainWrapper),
                                              static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread* idleT = _thread::createThread(reinterpret_cast<void (*)(void *)>(idle), static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread* mainT = _thread::createThread(nullptr, nullptr, nullptr);

    _thread* inputT = _thread::createThread(reinterpret_cast<void (*)(void *)>(_console::printingThread), static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                            nullptr);

    idleT->startThread();
    mainT->startThread();
    userM->startThread();
    inputT->startThread();

    _thread::running = mainT;

    Riscv::ms_sie(Riscv::SIE_SEIE);
    Riscv::ms_sie(Riscv::SIE_SSIE);

    while(!userM->isFinished()){
        thread_dispatch();
    }


    _console* console = _console::getInstance();
    while(!console->inEmpty()); //OBEZBEDI DA SE SVE ISPISE PRE KRAJA
    while(!console->outEmpty()); //OBEZBEDI DA SE SVE upise PRE KRAJA
    userM->~_thread();
    //OVAJ DEO MORA DA BI SE LEPO ZAVRSIO KERNEL DA NE PRIHVATA PREKIDE I SLICNO, jer tajmer
    mainT->setFinished(true);
    mainT->setMain(true);
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::mc_sie(Riscv::SIE_SSIE);
    Riscv::w_sip(0);
    return 0;
}