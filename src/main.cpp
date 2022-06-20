//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

int main(){
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine
    //asm __volatile__("ld %0, s1" : : "r"(x));
    //__asm__ volatile("ecall");
    __putc('f');
    __putc('\n');
    MemoryAllocator* mem = MemoryAllocator::getInstance();
    size_t velicinaZaglavlja = sizeof(MemoryAllocator)/MEM_BLOCK_SIZE + (sizeof(MemoryAllocator)%MEM_BLOCK_SIZE == 0?0:1); // meni je ovoliko

    const size_t maxMemorija = (((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - velicinaZaglavlja*MEM_BLOCK_SIZE) );

    size_t numOfBlocks = maxMemorija/MEM_BLOCK_SIZE + maxMemorija%MEM_BLOCK_SIZE==0?0:1;
    printInteger(numOfBlocks);
    __putc('\n');
    char* niz = (char*)mem->allocate(numOfBlocks); // celokupan prostor
    if(niz == nullptr) {
        __putc('?');
        __putc('\n');
    }

    int n = 10;
    char* niz2 = (char*)mem->allocate((n*sizeof(char))/MEM_BLOCK_SIZE);
    if(niz2 == nullptr) {
        __putc('k');
        __putc('\n');
    }

    int status = mem->deallocate(niz);
    if(status == 0) {
        __putc('h');
        __putc('\n');
    }
    niz2 = (char*)mem->allocate(n*sizeof(char));
    if(niz2 == nullptr) {
        __putc('?');
        __putc('\n');
    }

    //asm __volatile__("sd %0, s0" : "=r"(x));
    //new int;
    __putc('M');
    __putc('\n');
    return 0;
}
