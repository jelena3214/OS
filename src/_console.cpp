//
// Created by os on 7/3/22.
//

#include "../h/_console.hpp"

_console* _console::instance = nullptr;

void *_console::printingThread(void *p) {
    _console* console = _console::getInstance();
    while(1){
        while(*(volatile char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
            volatile char c = console->inputBuffer->get();
            volatile uint64* const reg = (volatile uint64 *const)CONSOLE_TX_DATA;
            *reg = c;
        }
    }
}

void _console::console_handler() {
    _console* console = _console::getInstance();
    while(*(volatile char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
        volatile char c = *(volatile char*)(CONSOLE_RX_DATA);
        if(c == '\r')c = '\n'; //jer pritiskanjem entera prepozna samo \r karakter
        console->outputBuffer->put(c);
    }
}

bool _console::outEmpty() {
    return outputBuffer->empty();;
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