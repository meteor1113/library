/**
 * Copyright (C) 2008 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


#ifndef THREAD_THREADIMPL_HPP_
#define THREAD_THREADIMPL_HPP_


// #include <standard library headers>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
#include <process.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

// #include "customer headers"


namespace thread
{

    class ThreadImpl
    {
    public:
#ifdef _WIN32
        struct ThreadStruct
        {
            HANDLE ht;
            unsigned int tid;
            ThreadStruct() : ht(NULL), tid(INVALID_THREAD_ID) {}
            void Cleanup();
            ~ThreadStruct() { Cleanup(); }
        };
        typedef unsigned int (__stdcall *ThreadFuncT)(void* param);
#else
        struct ThreadStruct
        {
            pthread_t pt;
            ThreadStruct() : pt(0) {}
            void Cleanup() { pt = 0; }
            ~ThreadStruct() { Cleanup(); }
        };
        typedef void* (*ThreadFuncT)(void* param);
#endif

    private:
        ThreadImpl();
        ThreadImpl(const ThreadImpl&) throw();
        void operator=(const ThreadImpl&);

    public:
        static bool Create(ThreadStruct& ts, ThreadFuncT func, void* arg);
        static void Destroy(ThreadStruct& ts) { ts.Cleanup(); }
        static bool WaitForEnd(const ThreadStruct& ts, int ms);
        static void Terminate(const ThreadStruct& ts);
        static bool IsAlive(const ThreadStruct& ts);
        static int GetId(const ThreadStruct& ts);
        static void Sleep(int ms);

    public:
#if _MSC_VER < 1400 // < vc8
        enum { WAIT_INFINITE = -1 };
        enum { INVALID_THREAD_ID = 0 };
        enum { WAIT_TIME_SLICE = 20 };
#else
        static const int WAIT_INFINITE = -1;
        static const int INVALID_THREAD_ID = 0;
        static const int WAIT_TIME_SLICE = 20;
#endif
    };


#ifdef _WIN32

    inline void ThreadImpl::ThreadStruct::Cleanup()
    {
        if (ht != NULL)
        {
            CloseHandle(ht);
            ht = NULL;
        }
        tid = INVALID_THREAD_ID;
    }

    inline bool ThreadImpl::Create(ThreadStruct& ts, ThreadFuncT func,
                                   void* arg)
    {
        ts.ht = (HANDLE)_beginthreadex(NULL, 0, func, arg, 0, &ts.tid);
        return (ts.ht != NULL);
    }

    inline bool ThreadImpl::WaitForEnd(const ThreadStruct& ts, int ms)
    {
        DWORD to = (DWORD)ms;
        if(ms == WAIT_INFINITE)
        {
            to = INFINITE;
        }
        DWORD res = WaitForSingleObject(ts.ht, to);
        return (res != WAIT_TIMEOUT);
    }

    inline void ThreadImpl::Terminate(const ThreadStruct& ts)
    {
        ::TerminateThread(ts.ht, 0);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        if (ts.ht == NULL)
        {
            return false;
        }
        DWORD res = WaitForSingleObject(ts.ht, 0);
        return (WAIT_TIMEOUT == res);
    }

    inline int ThreadImpl::GetId(const ThreadStruct& ts)
    {
        return (int)ts.tid;
    }

    inline void ThreadImpl::Sleep(int ms)
    {
        ::Sleep(ms);
    }

#else

    inline bool ThreadImpl::Create(ThreadStruct& ts, ThreadFuncT func,
                                   void* arg)
    {
        int res = pthread_create(&(ts.pt), NULL, func, arg);
        if (res != 0)
        {
            return false;
        }
        pthread_detach(ts.pt);
        return true;
    }

    inline bool ThreadImpl::WaitForEnd(const ThreadStruct& ts, int ms)
    {
        int iDelta = WAIT_TIME_SLICE;
        int iTotal = ms;
        if (ms == WAIT_INFINITE)
        {
            iDelta = 0;
            iTotal = 1;
        }
        for (int i = 0; i < iTotal; i += iDelta)
        {
            if (!IsAlive(ts))
            {
                return true;
            }
            else
            {
                ThreadImpl::Sleep(WAIT_TIME_SLICE);
            }
        }
        return false;
    }

    inline void ThreadImpl::Terminate(const ThreadStruct& ts)
    {
        ::pthread_cancel(ts.pt);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        if (ts.pt == 0)
        {
            return false;
        }

        int policy;
        struct sched_param sp;
        // in linux, pthread_getschedparam may SIGSEGV when tid is invalid.
        int res = pthread_getschedparam(ts.pt, &policy, &sp);
        return (res == 0);
    }

    inline int ThreadImpl::GetId(const ThreadStruct& ts)
    {
        return (int)ts.pt;
    }

    inline void ThreadImpl::Sleep(int ms)
    {
        int s = ms / 1000;
        int us = (ms % 1000) * 1000;
        if (s > 0)
        {
            sleep(s);
        }
        if (us > 0)
        {
            usleep(us);
        }
    }

#endif // _WIN32

}


#endif // THREAD_THREADIMPL_HPP_
