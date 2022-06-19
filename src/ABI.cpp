//
// Created by os on 5/23/22.
//

#include "../h/ABI.hpp"

//podesavanje prekidne rutine i upisivanje njene adrese u stvec, ecall treba da udje u tu prekidnu rutinu
//na teku treba da se nadje params, i onda u prekidnoj rutini pozivamo odgovarajuci sistemski poziv sa parametrima params
void* syscall_handler(struct FunctionParameters params){
    //stavljanje parametara u registre u a0..a7
    uint64 c1 = params.code;
    uint64 c2 = params.first;
    asm volatile("mv x10, %[code]" : : [code] "r"(c1));
    asm("mv x11, %[ime]" : : [ime] "r"(c2));
    asm("ecall");
    uint64 returnValue;
    asm("mv %[ime], x10" : [ime] "=r"(returnValue));
    //return &returnValue;
    return nullptr;
}
