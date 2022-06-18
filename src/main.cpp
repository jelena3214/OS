#include "../lib/console.h"


void main(){
    __putc('o');
    __putc('s');
    __putc('1');

    while(1){
        char s = __getc();
        __putc(s + 30);
    }
}
