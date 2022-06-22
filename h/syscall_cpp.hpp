//
// Created by os on 5/23/22.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP

#include "../h/syscall_c.hpp"
//ZASTO NECE SA ::
void* operator new (size_t y);
void *operator new[](size_t n);
void operator delete (void* y) noexcept;
void operator delete[](void *p) noexcept;

/*class Thread {
        public:
        Thread (void (*body)(void*), void* arg);
        virtual ~Thread ();
        int start ();
        static void dispatch ();
        static int sleep (time_t);
        protected:
        Thread ();
        virtual void run () {}
        private:
        thread_t myHandle;
};
class Semaphore {
        public:
        Semaphore (unsigned init = 1);
        virtual ~Semaphore ();
        int wait ();
        int signal ();
        private:
        sem_t myHandle;
};
class PeriodicThread : public Thread {
        protected:
        PeriodicThread (time_t period);
        virtual void periodicActivation () {}
};
class Console {
        public:
        static char getc ();
        static void putc (char);
};*/

#endif //PROJECT_BASE_SYSCALL_CPP_HPP
