//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/syscall_cpp.hpp"

int main(){
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine
    //asm __volatile__("ld %0, s1" : : "r"(x));
    //__asm__ volatile("ecall");
    __putc('f');
    //asm __volatile__("sd %0, s0" : "=r"(x));
    int* x = new int;
    __putc(*x);
    return 0;
}
