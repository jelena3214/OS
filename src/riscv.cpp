//
// Created by os on 5/26/22.
//

#include "../h/riscv.hpp"


void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}