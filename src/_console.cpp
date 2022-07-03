//
// Created by os on 7/3/22.
//

#include "../h/_console.hpp"

void *_console::printingThread(void *p) {
    while(1){
        if(CONSOLE_TX_STATUS_BIT){
            _console& console = _console::getInstance();
            char c = console.inputBuffer.get();
            uint64 volatile* const reg = reinterpret_cast<volatile uint64 *const>(CONSOLE_TX_DATA);
            *reg = c;
        }
    }
}