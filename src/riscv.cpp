//
// Created by os on 5/26/22.
//

#include "../h/riscv.hpp"

//MemoryAllocator* Riscv::memoryAllocator = MemoryAllocator::getInstance();

void Riscv::handleSupervisorTrap(){
    //
    uint64 scause = Riscv::r_scause();
    //printString("USAO\n");
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        volatile uint64 sepc = Riscv::r_sepc() + 4;
        volatile uint64 sstatus = Riscv::r_sstatus();
        volatile uint64 code, param1;
        asm("mv %0, x10" : "=r"(code));
        asm("mv %0, x11" : "=r"(param1));
        printString("U riscv: ");
        printInteger(param1);
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
                mem.ispisFree();
                break;
            }
        }
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL)
    {
        printString("Softverski treceg\n");

    } else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        //console_handler();
        printString("ECALL SISTESKOG\n");
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