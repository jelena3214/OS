//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_thread.hpp"
#include "../h/printing.hpp"

extern void userMain();

void *idle(void *p) {
    while (1) {
        thread_dispatch();
    }
}

class Moja : public PeriodicThread {
public:
    Moja(time_t v) : PeriodicThread(v) {};

    void periodicActivation() override {
        putc('j');
    }

};

void *mainWrapper(void *p) {
    //userRegime();
    userMain();
    return p;
}

int main() {
    //onemogucavanje prekida
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::mc_sie(Riscv::SIE_SSIE);
    Riscv::w_sip(0); //nema aktivnih prekida
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine


    MemoryAllocator &mem = MemoryAllocator::getInstance();
    _thread *userM = _thread::createThread(reinterpret_cast<void (*)(void *)>(userMain),
                                           static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread *idleT = _thread::createThread(reinterpret_cast<void (*)(void *)>(idle),
                                           static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread *mainT = _thread::createThread(nullptr, nullptr, nullptr);

    _thread *inputT = _thread::createThread(reinterpret_cast<void (*)(void *)>(_console::printingThread),
                                            static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                            nullptr);

    //idle je potreban da postoji bar kao jedina nit u scheduleru spremna
    idleT->startThread();
    mainT->startThread();
    userM->startThread();
    inputT->startThread();


    _thread::running = mainT;

    //omogucavamo prekide u supervised modu
    Riscv::ms_sie(Riscv::SIE_SEIE);
    Riscv::ms_sie(Riscv::SIE_SSIE);

    while (!userM->isFinished()) { //uposleno cekanje da userMain nit zavrsi
        thread_dispatch();
    }

    _console *console = _console::getInstance();
    while (!console->inEmpty()); //uposleno cekanje dok se sve ne ispise na konzolu
    while (!console->outEmpty()); //uposleno cekanje dok se isprazni izlazni bafer

    //gasenje prekida u supervised modu
    Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::mc_sie(Riscv::SIE_SSIE);
    Riscv::w_sip(0);

    //ciscenje pre kraja
    userM->~_thread();
    inputT->setFinished(true);
    inputT->~_thread();
    idleT->setFinished(true);
    idleT->~_thread();
    mainT->setFinished(true);
    mainT->~_thread();

    return 0;
}