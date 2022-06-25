//
// Created by os on 6/18/22.
//
#include "../h/ABI.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/_thread.hpp"

void* rutina(void *p){
    int *k = static_cast<int *>(p);
    printString("JEEEEEEEEEEEEEEEEEEEEEEEEEEEKAAAAAAAAAAAAAAAAAAAAAAA\n");
    printInteger(*k);
    return k;
}

void* rutina1(void *p){
    int *k = static_cast<int *>(p);
    printString("ACAACACAAAAAAAAAAAAAAAAAAAAA\n");
    printInteger(*k);
    return k;
}

int main() {
    Riscv::w_stvec(reinterpret_cast<uint64>(&Riscv::supervisorTrap)); //init za adresu prekidne rutine

    //__asm__ volatile("mv x16, %0" : : "r"(x));
    //__asm__ volatile("mv %0, x16" : "=r"(x1));
    //TODO main nit napraviti
    //__asm__ volatile("ecall");
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE); //omogucavamo prekide globalno
    _thread *threads[3];
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    threads[0] = _thread::createThread(nullptr, nullptr);
    _thread::running = threads[0];
    threads[0]->startThread();
    threads[1] = _thread::createThread(reinterpret_cast<void (*)()>(rutina),
                                       static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    threads[1]->startThread();
    threads[2] = _thread::createThread(reinterpret_cast<void (*)()>(rutina1),
                                       static_cast<uint64 *>(mem.allocate(DEFAULT_STACK_SIZE * sizeof(uint64))));
    threads[2]->startThread();
    _thread::yield();
    printString("DOSLI DO MAINA\n");
    //thread_t* pl = static_cast<thread_t *>(operator new(sizeof(thread_t)));
    //int x = thread_create(pl, reinterpret_cast<void (*)(void *)>(rutina), &s);
    //printInteger((uint64)pl);

    return 0;

}

/*int n = 16;
char** matrix = (char**)mem.allocate(n*sizeof(char*));
checkNullptr(matrix);
for(int i = 0; i < n; i++) {
    matrix[i] = (char *) mem.allocate(n * sizeof(char));
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
    int status = mem.deallocate(matrix[i]);
    checkStatus(status);
}
int status = mem.deallocate(matrix);
checkStatus(status);

mem.ispisFree();

return 0;



size_t velicinaZaglavlja = sizeof(Block); // meni je ovoliko
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

