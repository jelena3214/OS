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
    static _console* getInstance();

    static void console_handler();

    friend class Riscv;

    static void* printingThread(void* p);

    _console(const _console&) = delete;

    bool outEmpty();
    bool inEmpty();

    ~_console();

};


#endif //PROJECT_BASE__CONSOLE_HPP
