//
// Created by os on 7/3/22.
//

#include "../h/_console.hpp"
#include "../h/print.hpp"
_console* _console::instance = nullptr;

void *_console::printingThread(void *p) {
    while(1){
        if(CONSOLE_TX_STATUS_BIT){
            _console* console = _console::getInstance();
            char c = console->inputBuffer->get();
            uint64 volatile* const reg = reinterpret_cast<volatile uint64 *const>(CONSOLE_TX_DATA);
            *reg = c;
        }
    }
}

void _console::console_handler() {
    if(plic_claim() == CONSOLE_IRQ){
        if(CONSOLE_RX_STATUS_BIT){
            _console* console = _console::getInstance();
            char c = *reinterpret_cast<char*>(CONSOLE_RX_DATA);
            console->outputBuffer->put(c);
        }
        plic_complete(CONSOLE_IRQ);
    }
}
