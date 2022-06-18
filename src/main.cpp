//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"


extern "C" void supervisorTrap();

int main(){
    Riscv::w_stvec(reinterpret_cast<uint64>(&supervisorTrap)); //init za adresu prekidne rutine
    uint64 x = 106;
    //asm __volatile__("ld %0, s1" : : "r"(x));
    __asm__ volatile("ecall");
    __putc('f');
    //asm __volatile__("sd %0, s0" : "=r"(x));
    __putc(x);
    return 0;
}
