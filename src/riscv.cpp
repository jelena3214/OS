//
// Created by os on 5/26/22.
//

#include "../h/riscv.hpp"

//MemoryAllocator* Riscv::memoryAllocator = MemoryAllocator::getInstance();

void Riscv::handleSupervisorTrap(){
    static MemoryAllocator* mA = MemoryAllocator::getInstance();
    uint64 scause = Riscv::r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 sepc = Riscv::r_sepc() + 4;
        uint64 sstatus = Riscv::r_sstatus();
        uint64 code, param1;
        asm("mv %[ime], x10" : [ime] "=r"(code));
        asm("mv %[ime], x11" : [ime] "=r"(param1));
        switch(code){
            case 0x01:
                mA->allocate((size_t)param1);
                __putc('J');
                break;
        }
        //TCB::timeSliceCounter = 0;
        //TCB::dispatch();
        Riscv::w_sstatus(sstatus);
        Riscv::w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        /*TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 sepc = r_sepc();
            uint64 sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
        mc_sip(SIP_SSIP);*/
    } else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        //console_handler();
    } else {
        // unexpected trap cause
    }
}

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}