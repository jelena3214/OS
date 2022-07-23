//
// Created by os on 5/26/22.
//

#include "../h/riscv.hpp"
#include "../h/printing.hpp"
#include "../lib/console.h"



void Riscv::handleSupervisorTrap(){
    volatile uint64 code, param1, param2, param3, param4;
    asm("mv %0, x10" : "=r"(code));
    asm("mv %0, x11" : "=r"(param1));
    asm("mv %0, x12" : "=r"(param2));
    asm("mv %0, x13" : "=r"(param3));
    asm("mv %0, x14" : "=r"(param4));

    uint64 scause = Riscv::r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        volatile uint64 sepc = Riscv::r_sepc() + 4;
        uint64 volatile sstatus;
        __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));

        MemoryAllocator &mem = MemoryAllocator::getInstance();
        switch(code) {
            case 0x01: {
                uint64 ret = (uint64) mem.allocate((size_t) param1*MEM_BLOCK_SIZE - sizeof(Block));
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x02: {
                uint64 ret = (uint64) mem.deallocate((void *) param1);
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x11:{
                volatile uint64 ret = 0;
                _thread* newThread = _thread::createThread(reinterpret_cast<void (*)(void *)>(param2), (uint64 *) param4,
                                                           (void *) param3);
                if(newThread != nullptr){
                    ret = 1;
                }
                uint64** handlePlace = (uint64 **) param1;
                *handlePlace = reinterpret_cast<uint64 *>(newThread);
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x44:{
                uint64** thr = (uint64**)param1;
                _thread* thread = (_thread *) *thr;
                uint64 ret = (uint64)thread->startThread();
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x13:{
                //da li treba raditi dispatch kad ce se svakako desiti na kraju?????
                break;
            }
            case 0x12: {
                _thread::running->setFinished(true);
                break;
            }
            case 0x55:{
                uint64** thr = (uint64**)param1;
                _thread* thread = (_thread *) *thr;
                thread->~_thread();
                break;
            }
            case 0x26:
            {
                __asm__ volatile ("mv x10, %0" : : "r"(1));
                __asm__ volatile("sd x10, 80(fp)");
                sstatus &= ~(1 << 8); //clear spp
                sstatus |= (1 << 5); //set spie

                Riscv::ms_sie(Riscv::SIE_SEIE);
                Riscv::ms_sie(Riscv::SIE_SSIE);
                break;
            }
            case 0x31:{
                time_t time = param1;
                volatile int ret = 0;
                if(time > 0) {
                    ret = _thread::running->sleepQueue.put(_thread::running, time);
                    if (ret == 0) {
                        _thread::running->setSleeping(true);
                    }
                }
                if(time < 0)ret = -1; //error
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x21:{
                volatile uint64 ret = 0;
                _sem* newSemaphore = _sem::create_semaphore(param3);
                if(newSemaphore != nullptr){
                    ret = 1;
                }
                uint64** handlePlace = (uint64 **) param1;
                *handlePlace = reinterpret_cast<uint64 *>(newSemaphore);
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x22:{
                _sem* semaphore = (_sem *) param1;
                uint64 ret = semaphore->close();
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x23:{
                _sem* semaphore = (_sem *) param1;
                uint64 ret = semaphore->wait();
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x24:{
                _sem* semaphore = (_sem *) param1;
                uint64 ret = semaphore->signal();
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x77:{
                uint64** sm = (uint64**)param1;
                _sem* semaphore = (_sem *) *sm;
                semaphore->~_sem();
                break;
            }
            case 0x41:{
                _console* console = _console::getInstance();
                char c = console->outputBuffer->get();
                __asm__ volatile ("mv x10, %0" : : "r"((uint64)c));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x42:{
                uint64 c = (uint64)param1;
                char cc = (char)c;
                _console* console = _console::getInstance();
                console->inputBuffer->put(cc);
                break;
            }
        }
        _thread::timeSliceCounter = 0;
        _thread::dispatch();

        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL)
    {
        _thread::timeSliceCounter++;
        if (_thread::timeSliceCounter >= _thread::running->getTimeSlice())
        {
            volatile uint64 sepc = r_sepc();
            volatile uint64 sstatus = r_sstatus();
            _thread::timeSliceCounter = 0;
            _thread::dispatch();

            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        _thread::sleepQueue.decTime();
        while(1){
            _thread* unsleepTHread = _thread::sleepQueue.get();
            if(unsleepTHread == nullptr)break;
            unsleepTHread->setSleeping(false);
        }

        mc_sip(SIP_SSIP); //cistimo bit koji predstavlja zahtev za softverskim prekidom
    } else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        volatile uint64 cause = plic_claim();
        if(cause == CONSOLE_IRQ){
            _console::console_handler();
        }
        plic_complete(cause);
        mc_sip(SIP_SEIP);

    } else {
        // unexpected trap cause
        printString("\nNZM\n");
        printInt(scause);
    }

}

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}