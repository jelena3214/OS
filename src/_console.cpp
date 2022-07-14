//
// Created by os on 7/3/22.
//

#include "../h/_console.hpp"

_console* _console::instance = nullptr;

void *_console::printingThread(void *p) {
    while(1){
        while(*(volatile char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
            _console* console = _console::getInstance();
            char c = console->inputBuffer->get();
            volatile uint64* const reg = (volatile uint64 *const)CONSOLE_TX_DATA;
            *reg = c;
        }
    }
}

void _console::console_handler() {
    uint64 cause = plic_claim();
    if(cause == CONSOLE_IRQ){
        while(*(volatile char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
            _console* console = _console::getInstance();
            volatile char c = *(volatile char*)(CONSOLE_RX_DATA);
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

_console* _console::getInstance(){
    if(instance == nullptr){
        MemoryAllocator& mem = MemoryAllocator::getInstance();
        instance = static_cast<_console *>(mem.allocate(sizeof(_console)));
        instance->inputBuffer = kbuffer::create_buffer();
        instance->outputBuffer = kbuffer::create_buffer();
    }
    return instance;
}