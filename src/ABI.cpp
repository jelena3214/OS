//
// Created by os on 5/23/22.
//

#include "../h/ABI.hpp"

//podesavanje prekidne rutine i upisivanje njene adrese u stvec, ecall treba da udje u tu prekidnu rutinu
//na teku treba da se nadje params, i onda u prekidnoj rutini pozivamo odgovarajuci sistemski poziv sa parametrima params
void* syscall_handler(struct FunctionParameters params){
    //stavljanje parametara u registre u a0..a7
    asm("ecall");
    return nullptr;
}

extern "C" void ecall(){
    //dodavanje 4 na sepc jer ecall pamti adresu sa koje je pozvan a ne sledecu instrukciju
    //uint64 save_pc = Riscv::r_sepc();
    //save_pc = save_pc + 4;
    //Riscv::w_sepc(save_pc);
    __putc('u');
}