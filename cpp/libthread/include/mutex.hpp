/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


#ifndef THREAD_MUTEX_HPP_
#define THREAD_MUTEX_HPP_


// #include <standard library headers>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
WINBASEAPI BOOL WINAPI
TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
#else
#include <pthread.h>
#endif

// #include "customer headers"


namespace thread
{


#ifdef _WIN32

    class Mutex
    {
    public:
        Mutex() { ::InitializeCriticalSection(&cs); }
        ~Mutex() { ::DeleteCriticalSection(&cs); }

    private:
        Mutex(const Mutex& rhs);
        Mutex& operator=(const Mutex& rhs);

    public:
        void Lock() { ::EnterCriticalSection(&cs); }
        void UnLock() { ::LeaveCriticalSection(&cs); }
        bool TryLock() { return (::TryEnterCriticalSection(&cs) == TRUE); }

    private:
        //volatile HANDLE mMutex;
        CRITICAL_SECTION cs;

    };

#else

    class Mutex
    {
    public:
        Mutex() { pthread_mutex_init(&mutex, NULL); }
        ~Mutex() { pthread_mutex_destroy(&mutex); }

    private:
        Mutex(const Mutex& rhs);
        Mutex& operator=(const Mutex& rhs);

    public:
        void Lock() { pthread_mutex_lock(&mutex); }
        void UnLock() { pthread_mutex_unlock(&mutex); }
        bool TryLock() { return (pthread_mutex_trylock(&mutex) == 0); }

    private:
        pthread_mutex_t mutex;

    };

#endif // _WIN32


    class Lock
    {
    public:
        Lock(Mutex& m) : mMutex(m) { mMutex.Lock(); }
        ~Lock() { mMutex.UnLock(); }

    private:
        Mutex& mMutex;

    };


}


#endif // THREAD_MUTEX_HPP_
