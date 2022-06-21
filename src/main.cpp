//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/MemoryAllocator.hpp"

void checkNullptr(void* p) {
    static int x = 0;
    if(p == nullptr) {
        __putc('x');
        __putc('0' + x);
    }
    x++;
}

void checkStatus(int status) {
    static int y = 0;
    if(status == 0) {
        __putc('0' + y);
        __putc('y');
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

    int n = 16;
    char** matrix = (char**)mem->allocate(n*sizeof(char*));
    checkNullptr(matrix);
    for(int i = 0; i < n; i++) {
        matrix[i] = (char *) mem->allocate(n * sizeof(char));
        checkNullptr(matrix[i]);
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrix[i][j] = 'k';
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            __putc(matrix[i][j]);
            __putc(' ');
        }
        __putc('\n');
    }


    for(int i = 0; i < n; i++) {
        int status = mem->deallocate(matrix[i]);
        checkStatus(status);
    }
    int status = mem->deallocate(matrix);
    checkStatus(status);

    mem->ispisFree();

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
