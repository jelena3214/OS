//
// Created by os on 5/26/22.
//

#include "../h/riscv.hpp"
#include "../h/_thread.hpp"

//MemoryAllocator* Riscv::memoryAllocator = MemoryAllocator::getInstance();

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
        volatile uint64 sstatus = Riscv::r_sstatus();
        printString("U riscv: ");
        printInteger(param4);
        printString("\ncode : ");
        printInteger(code);
        __putc('\n');
        MemoryAllocator &mem = MemoryAllocator::getInstance();
        switch(code) {
            case 0x01: {
                uint64 ret = (uint64) mem.allocate((size_t) param1*MEM_BLOCK_SIZE - sizeof(Block));
                __putc('J');
                printInteger(ret);
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x02: {
                uint64 ret = (uint64) mem.deallocate((void *) param1);
                __putc('\n');
                printInteger(ret);
                __putc('\n');
                __asm__ volatile ("mv x10, %0" : : "r"(ret));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
            case 0x11:{
                volatile uint64 ret = 0;
                printInteger((uint64 ) param4);
                _thread* newThread = _thread::createThread(reinterpret_cast<void (*)(void *)>(param2), (uint64 *) param4,
                                                           (void *) param3);
                if(newThread != nullptr){
                    ret = 1;
                }
                printInteger(ret);
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
                printString("REEEEEEEEEEEEEEEET ");
                printInteger(ret);
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
                Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
                sstatus = Riscv::r_sstatus();
                __asm__ volatile ("mv x10, %0" : : "r"(1));
                __asm__ volatile("sd x10, 80(fp)");
                break;
            }
        }
        printInteger(sstatus);
        printString("\nTOOOOO");
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
            printInteger(sstatus);
            printString("\nTOOOOO");
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIP); //cistimo bit koji predstavlja zahtev za softverskim prekidom
        printString("TAJMER\n");
    } else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
        printString("spoljaski hardverski\n");
    } else {
        // unexpected trap cause
        uint64 sepc = Riscv::r_sepc() + 4;
        uint64 sstatus = Riscv::r_sstatus();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
        printString("\nNZM\n");
    }
}

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}