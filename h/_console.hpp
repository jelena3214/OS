//
// Created by os on 7/3/22.
//

#ifndef PROJECT_BASE__CONSOLE_HPP
#define PROJECT_BASE__CONSOLE_HPP


#include "kbuffer.hpp"
#include "../lib/hw.h"

class _console {
private:
    kbuffer *inputBuffer, *outputBuffer;
    _console() {}
    static _console* instance;
public:
    static _console* getInstance(){
        if(instance == nullptr){
            MemoryAllocator& mem = MemoryAllocator::getInstance();
            instance = static_cast<_console *>(mem.allocate(sizeof(_console)));
            instance->inputBuffer = kbuffer::create_buffer();
            instance->outputBuffer = kbuffer::create_buffer();
        }
        return instance;
    }

    friend class Riscv;

    static void* printingThread(void* p);

    _console(const _console&) = delete;

    //destruktor

};


#endif //PROJECT_BASE__CONSOLE_HPP
