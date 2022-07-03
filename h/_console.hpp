//
// Created by os on 7/3/22.
//

#ifndef PROJECT_BASE__CONSOLE_HPP
#define PROJECT_BASE__CONSOLE_HPP


#include "kbuffer.hpp"
#include "../lib/hw.h"

class _console {
private:
    kbuffer inputBuffer, outputBuffer;
    _console() {}
public:
    static _console& getInstance(){
        static _console instance;
        return instance;
    }

    friend class Riscv;

    static void* printingThread(void* p);

    _console(const _console&) = delete;

};


#endif //PROJECT_BASE__CONSOLE_HPP
