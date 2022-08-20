//
// Created by os on 7/3/22.
//

#ifndef PROJECT_BASE__CONSOLE_HPP
#define PROJECT_BASE__CONSOLE_HPP


#include "_buffer.hpp"
#include "../lib/hw.h"

class _console {
private:
    _buffer *inputBuffer, *outputBuffer;

    _console() {}

    static _console *instance;
public:
    //stvaranje instance konzole
    static _console *getInstance();

    //funkcija za obradu prekida pri unosu
    static void console_handler();

    friend class Riscv;

    //funkcija koja salje karaktere kontoleru
    static void *printingThread(void *p);

    _console(const _console &) = delete;

    bool outEmpty();

    bool inEmpty();

    ~_console();
};


#endif //PROJECT_BASE__CONSOLE_HPP
