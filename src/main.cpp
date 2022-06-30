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

void* rutina(void *p){
    int t = 150;
    while(--t > 0){
        //printString("ACAACACAAAAAAAAAAAAAAAAAAAAA\n");
        Thread::sleep(200);
    }
    thread_exit();
    return p;
}

void* rutina1(void *p){
    int t = 150;
    while(--t > 0){
        //printString("DRAGANANANANANANNANANANANANANA\n");
        Thread::sleep(150);
    }
    thread_exit();
    return p;
}

void* idle(void* p){
    while(1){
        thread_dispatch();
    }
}
_thread* userM;
/*void* userMain(void* p){
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    __putc('J');
    __putc('\n');
    uint64 t1 = 4;
    Thread t(reinterpret_cast<void (*)(void *)>(rutina), &t1);
    t.start();
    uint64 t2 = 4;
    Thread t3(reinterpret_cast<void (*)(void *)>(rutina1), &t2);
    t3.start();
    //userM->setFinished(true); //Da li sme ovako da oznacim da je zavrsila? ili je ok ono u destruktoru?
    Thread *tt = new Thread(reinterpret_cast<void (*)(void *)>(rutina1), &t2);
    tt->start();
    delete tt;
    Thread::dispatch();
    Thread::sleep(100);
    printString("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
    thread_exit(); //da li exit treba jos nesto vise od postavljanje finisha?
    return p;
}
*/
int main() {
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //Riscv::mc_sie(Riscv::SIE_SEIE);
    Riscv::ms_sie(Riscv::SIE_SSIE);
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine

    //__asm__ volatile("mv x16, %0" : : "r"(x));
    //__asm__ volatile("mv %0, x16" : "=r"(x1));

    //__asm__ volatile("ecall");

    MemoryAllocator& mem = MemoryAllocator::getInstance();
    userM = _thread::createThread(reinterpret_cast<void (*)(void*)>(userMain),
                                              static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread* idleT = _thread::createThread(reinterpret_cast<void (*)(void *)>(idle), static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))),
                                           nullptr);
    _thread* mainT = _thread::createThread(nullptr, nullptr, nullptr);
    idleT->startThread();
    mainT->startThread();
    userM->startThread();

    //printString("PROSAO USEEEEEEEEEEEEER");
    _thread::running = mainT;
    userRegime();

    while(!userM->isFinished());
    userM->~_thread();
    mainT->setFinished(true);
    mainT->setMain(true);
    //printString("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTttttttttttttttttttttttttttttttttttttttt");
    //IDLEt UVEK DA UDE TU???
    return 0;
}