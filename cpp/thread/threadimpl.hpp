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
            DWORD tid;
            ThreadStruct() : ht(NULL), tid(INVALID_THREAD_ID) {}
            void Cleanup() { if (ht != 0) { CloseHandle(ht); ht = 0; } }
            ~ThreadStruct() { Cleanup(); }
        };
        typedef unsigned int (__stdcall *THREAD_FUNCTION)(void* param);
#else
        struct ThreadStruct
        {
            pthread_t tid;
            ThreadStruct() : tid(INVALID_THREAD_ID) {}
            void Cleanup() { tid = INVALID_THREAD_ID; }
            ~ThreadStruct() { Cleanup(); }
        };
        typedef void* (*THREAD_FUNCTION)(void* param);
#endif

    private:
        ThreadImpl();
        ThreadImpl(const ThreadImpl&) throw();
        void operator=(const ThreadImpl&);

    public:
        static bool CreateThread(ThreadStruct& ts,
                                 THREAD_FUNCTION func,
                                 unsigned int stackSize,
                                 void* arg);
        static void DestroyThread(ThreadStruct& ts) { ts.Cleanup(); }
        static bool WaitForThreadEnd(const ThreadStruct& ts, int ms);
        static void TerminateThread(const ThreadStruct& ts);
        static bool IsAlive(const ThreadStruct& ts);
        static int GetThreadId(const ThreadStruct& ts);
        static void Sleep(int ms);

    public:
#if _MSC_VER < 1300 // < vc7
        enum { WAIT_INFINITE = -1 };
        enum { INVALID_THREAD_ID = 0 };
        enum { DEFAULT_STACK_SIZE = 0 };
        enum { WAIT_TIME_SLICE = 20 };
#else
        static const int WAIT_INFINITE = -1;
        static const int INVALID_THREAD_ID = 0;
        static const unsigned int DEFAULT_STACK_SIZE = 0;
        static const int WAIT_TIME_SLICE = 20;
#endif
    };


#ifdef _WIN32

    inline bool ThreadImpl::CreateThread(ThreadStruct& ts,
                                         THREAD_FUNCTION func,
                                         unsigned int stackSize,
                                         void* arg)
    {
        ts.ht = (HANDLE)_beginthreadex(NULL, stackSize, func, arg, 0,
                                       (unsigned int*)&ts.tid);
        return (ts.ht != NULL);
    }

    inline bool ThreadImpl::WaitForThreadEnd(const ThreadStruct& ts, int ms)
    {
        DWORD to = (DWORD)ms;
        if(ms == WAIT_INFINITE)
        {
            to = INFINITE;
        }
        DWORD res = WaitForSingleObject(ts.ht, to);
        return (res != WAIT_TIMEOUT);
    }

    inline void ThreadImpl::TerminateThread(const ThreadStruct& ts)
    {
        ::TerminateThread(ts.ht, 0);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        if (NULL == ts.ht)
        {
            return false;
        }
        DWORD res = WaitForSingleObject(ts.ht, 0);
        return (WAIT_TIMEOUT == res);
    }

    inline int ThreadImpl::GetThreadId(const ThreadStruct& ts)
    {
        return (int)ts.tid;
    }

    inline void ThreadImpl::Sleep(int ms)
    {
        ::Sleep(ms);
    }

#else

    inline bool ThreadImpl::CreateThread(ThreadStruct& ts,
                                         THREAD_FUNCTION func,
                                         unsigned int stackSize,
                                         void* arg)
    {
        pthread_attr_t *pAttr = NULL;
        pthread_attr_t attr;
        if (stackSize != DEFAULT_STACK_SIZE)
        {
            if (0 != pthread_attr_init(&attr))
            {
                return false;
            }
            if (0 != pthread_attr_setstacksize(&attr, stackSize))
            {
                pthread_attr_destroy(&attr);
                return false;
            }
            pAttr = &attr;
        }
        int res = pthread_create(&(ts.tid), pAttr, func, arg);
        if (NULL != pAttr)
        {
            pthread_attr_destroy(&attr);
        }
        if (0 != res)
        {
            return false;
        }
        pthread_detach(ts.tid);
        return true;
    }

    inline bool ThreadImpl::WaitForThreadEnd(const ThreadStruct& ts, int ms)
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
                Sleep(WAIT_TIME_SLICE);
            }
        }
        return false;
    }

    inline void ThreadImpl::TerminateThread(const ThreadStruct& ts)
    {
        ::pthread_cancel(ts.tid);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        int policy;
        struct sched_param sp;
        int iRes = pthread_getschedparam(ts.tid, &policy, &sp);
        return (0 == iRes);
    }

    inline int ThreadImpl::GetThreadId(const ThreadStruct& ts)
    {
        return (int)ts.tid;
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
