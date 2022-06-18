//
// Created by os on 5/23/22.
//

#include "../h/ABI.h"

//podesavanje prekidne rutine i upisivanje njene adrese u stvec, ecall treba da udje u tu prekidnu rutinu
//na teku treba da se nadje params, i onda u prekidnoj rutini pozivamo odgovarajuci sistemski poziv sa parametrimaparams
void* syscall_handler(struct FunctionParameters params){
    asm("ecall");
}