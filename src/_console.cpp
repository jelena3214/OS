//
// Created by os on 7/3/22.
//

#include "../h/_console.hpp"
#include "../h/print.hpp"
_console* _console::instance = nullptr;

void *_console::printingThread(void *p) {
    while(1){
        while(*(volatile char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
            _console* console = _console::getInstance();
            volatile char c = console->inputBuffer->get();
            uint64 volatile* const reg = reinterpret_cast<volatile uint64 *const>(CONSOLE_TX_DATA);
            *reg = c;
        }
    }
}

void _console::console_handler() {
    uint64 cause = plic_claim();
    if(cause == CONSOLE_IRQ){
        while(*(volatile char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
            _console* console = _console::getInstance();
            char c = *reinterpret_cast<char*>(CONSOLE_RX_DATA);
            console->outputBuffer->put(c);
        }
    }
    plic_complete(cause);
}

bool _console::outEmpty() {
    volatile bool is = outputBuffer->empty();
    return is;
}

bool _console::inEmpty() {
    return inputBuffer->empty();
}

_console::~_console() {
    inputBuffer->~kbuffer();
    outputBuffer->~kbuffer();
    MemoryAllocator& mem = MemoryAllocator::getInstance();
    mem.deallocate(this);
}
