//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

void checkNullptr(void* p) {
    static int x = 0;
    if(p == nullptr) {
        __putc('?');
        __putc('0' + x);
    }
    x++;
}

void checkStatus(int status) {
    static int y = 0;
    if(status) {
        __putc('0' + y);
        __putc('?');
    }
    y++;
}

int main(){
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine
    //asm __volatile__("mv %0, s1" : : "r"(x));
    //__asm__ volatile("ecall");
    __putc('f');
    __putc('\n');


    MemoryAllocator* mem = MemoryAllocator::getInstance();

    size_t velicinaZaglavlja = sizeof(Block); // meni je ovoliko
    size_t instanceSize = (sizeof(MemoryAllocator) + velicinaZaglavlja)/MEM_BLOCK_SIZE + ((sizeof(MemoryAllocator) + velicinaZaglavlja)%MEM_BLOCK_SIZE == 0?0:1);

    const size_t maxMemorija = ((size_t)HEAP_END_ADDR - ((size_t)HEAP_START_ADDR  + (size_t)instanceSize*MEM_BLOCK_SIZE) - velicinaZaglavlja);

    int *p1 = (int*)mem->allocate(15*sizeof(int)); // trebalo bi da predje jedan blok od 64
    checkNullptr(p1);
    int *p2 = (int*)mem->allocate(30*sizeof(int));
    checkNullptr(p2);

    int *p3 = (int*)mem->allocate(30*sizeof(int));
    checkNullptr(p3);

    checkStatus(mem->deallocate(p1));
    checkStatus(mem->deallocate(p3));
    checkStatus(mem->deallocate(p2)); // p2 treba da se spoji sa p1 i p3

    int *celaMemorija = (int*)mem->allocate(maxMemorija);
    checkNullptr(celaMemorija);

    checkStatus(mem->deallocate(celaMemorija));

    return 0;



    /*size_t velicinaZaglavlja = sizeof(Block); // meni je ovoliko
    size_t instanceSize = (sizeof(MemoryAllocator) + velicinaZaglavlja)/MEM_BLOCK_SIZE + ((sizeof(MemoryAllocator) + velicinaZaglavlja)%MEM_BLOCK_SIZE == 0?0:1);

    const size_t maxMemorija = ((size_t)HEAP_END_ADDR - ((size_t)HEAP_START_ADDR  + (size_t)instanceSize*MEM_BLOCK_SIZE) - velicinaZaglavlja);
    printInteger(maxMemorija);
    __putc('\n');
    char* niz = (char*)mem->allocate((maxMemorija/MEM_BLOCK_SIZE)*MEM_BLOCK_SIZE); // celokupan prostor
    if(niz == nullptr) {
        __putc('t');
        __putc('\n');
    }

    int n = 10;
   char* niz2 = (char*)mem->allocate(n*sizeof(char));
   if(niz2 == nullptr) {
       __putc('k');
       __putc('\n');
   }

   int status = mem->deallocate((void*)niz);
   if(status == 0) {
       __putc('g');
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
    return 0;*/
}
