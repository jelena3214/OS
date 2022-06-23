//
// Created by os on 5/23/22.
//

#include "../h/ABI.hpp"

//podesavanje prekidne rutine i upisivanje njene adrese u stvec, ecall treba da udje u tu prekidnu rutinu
//na teku treba da se nadje params, i onda u prekidnoj rutini pozivamo odgovarajuci sistemski poziv sa parametrima params
void* syscall_handler(struct FunctionParameters params){
    //stavljanje parametara u registre u a0..a7
    volatile uint64 c1 = params.code;
    volatile uint64 c2 = reinterpret_cast<uint64>(params.first);
    printInteger(c2);
    Function c3 = params.second;
    volatile uint64 c4 = reinterpret_cast<uint64>(params.third);
    volatile uint64 c5 = reinterpret_cast<uint64>(params.fourth);
    printString("U ABI: ");
    printInteger((uint64)params.first);
    __putc('\n');
    printInteger(c4);
    printString("DRUGI: \n");
    printInteger(c5);
    __asm__ volatile("mv x10, %0" : : "r"(c1));
    __asm__ volatile("mv x11, %0" : : "r"(c2));
    __asm__ volatile("mv x12, %0" : : "r"(c3));
    __asm__ volatile("mv x13, %0" : : "r"(c4));
    __asm__ volatile("mv x14, %0" : : "r"(c5));
    __asm__ volatile("ecall");
    volatile uint64 returnValue;
    __asm__ volatile("mv %0, x10" : "=r"(returnValue));

    printString("\nABI VRAT: ");
    printInteger(returnValue);
    __putc('\n');
    return reinterpret_cast<void *>(returnValue);
}
