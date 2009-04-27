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


    class Thread;

#ifdef _WIN32

    class ThreadImpl
    {
    public:
        struct ThreadStruct
        {
            HANDLE hThread;
            DWORD dwThreadId;
            ThreadStruct() : hThread(NULL), dwThreadId(INVALID_THREAD_ID) {}
            void Cleanup();
            ~ThreadStruct() { Cleanup(); }
        };
        typedef void (*THREAD_FUNCTION)(void* param);

    private:
        ThreadImpl();
        ThreadImpl(const ThreadImpl&) throw();
        void operator=(const ThreadImpl&);

    public:
        static bool CreateThread(ThreadStruct& ts,
                                 THREAD_FUNCTION threadFunc,
                                 unsigned int cbStackSize,
                                 void* arg);
        static void DestroyThread(ThreadStruct& ts) { ts.Cleanup(); }
        static bool WaitForThreadEnd(const ThreadStruct& ts, int ms);
        static void TerminateThread(const ThreadStruct& ts);
        static bool IsAlive(const ThreadStruct& ts);
        static int GetThreadId(const ThreadStruct& ts);
        static void Sleep(int iMs) { ::Sleep(iMs); }

    public:
#if _MSC_VER < 1300
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

    inline void ThreadImpl::ThreadStruct::Cleanup()
    {
        // if(hThread != NULL)     // only when use CreateThread
        // {
        //     CloseHandle(hThread);
        //     hThread = NULL;
        // }
    }

    inline bool ThreadImpl::CreateThread(ThreadStruct& ts,
                                         THREAD_FUNCTION threadFunc,
                                         unsigned int cbStackSize,
                                         void* arg)
    {
        ts.hThread = (HANDLE)_beginthread(threadFunc, cbStackSize, arg);
        return (ts.hThread != NULL);
    }

    inline bool ThreadImpl::WaitForThreadEnd(const ThreadStruct& ts, int ms)
    {
        DWORD dwTO = (DWORD)ms;
        if(ms == WAIT_INFINITE)
        {
            dwTO = INFINITE;
        }
        DWORD dwRes = WaitForSingleObject(ts.hThread, dwTO);
        return (dwRes != WAIT_TIMEOUT);
    }

    inline void ThreadImpl::TerminateThread(const ThreadStruct& ts)
    {
        ::TerminateThread(ts.hThread, 0);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        if (NULL == ts.hThread)
        {
            return false;
        }
        DWORD dwRes = WaitForSingleObject(ts.hThread, 0);
        return (WAIT_TIMEOUT == dwRes);
    }

    inline int ThreadImpl::GetThreadId(const ThreadStruct& ts)
    {
        return (int)ts.dwThreadId;
    }

#else

    /**
     * description
     */
    class ThreadImpl
    {
    public:
        struct ThreadStruct
        {
            pthread_t tThread;
            ThreadStruct() : tThread(INVALID_THREAD_ID) {}
            ~ThreadStruct() {}
        };
        typedef void* (*THREAD_FUNCTION)(void* param);

    private:
        ThreadImpl();
        ThreadImpl(const ThreadImpl&) throw();
        void operator=(const ThreadImpl&);

    public:
        static bool CreateThread(ThreadStruct& ts,
                                 THREAD_FUNCTION threadFunc,
                                 unsigned int cbStackSize,
                                 void* arg);
        static void DestroyThread(ThreadStruct& ts) {}
        static bool WaitForThreadEnd(const ThreadStruct& ts, int ms);
        static void TerminateThread(ThreadStruct& ts);
        static bool IsAlive(const ThreadStruct& ts);
        static int GetThreadId(const ThreadStruct& ts);
        static void Sleep(int iMs);

    public:
        static const int WAIT_INFINITE = -1;
        static const int INVALID_THREAD_ID = 0;
        static const unsigned int DEFAULT_STACK_SIZE = 0;
        static const int WAIT_TIME_SLICE = 20;
    };

    inline bool ThreadImpl::CreateThread(ThreadStruct& ts,
                                         THREAD_FUNCTION threadFunc,
                                         unsigned int cbStackSize,
                                         void* arg)
    {
        pthread_attr_t *pAttr = NULL;
        pthread_attr_t attr;
        if (cbStackSize != DEFAULT_STACK_SIZE)
        {
            if (0 != pthread_attr_init(&attr))
            {
                return false;
            }
            if (0 != pthread_attr_setstacksize(&attr, cbStackSize))
            {
                pthread_attr_destroy(&attr);
                return false;
            }
            pAttr = &attr;
        }
        int iRes = pthread_create(&(ts.tThread),
                                  pAttr,
                                  threadFunc,
                                  arg);
        if (NULL != pAttr)
        {
            pthread_attr_destroy(&attr);
        }
        if (0 != iRes)
        {
            return false;
        }
        pthread_detach(ts.tThread);
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

    inline void ThreadImpl::TerminateThread(ThreadStruct& ts)
    {
        ::pthread_cancel(ts.tThread);
    }

    inline bool ThreadImpl::IsAlive(const ThreadStruct& ts)
    {
        int iPolicy;
        struct sched_param sp;
        int iRes = pthread_getschedparam(ts.tThread, &iPolicy, &sp);
        return (0 == iRes);
    }

    inline int ThreadImpl::GetThreadId(const ThreadStruct& ts)
    {
        return (int)ts.tThread;
    }

    inline void ThreadImpl::Sleep(int iMs)
    {
        int iS  = iMs / 1000;
        int iUs = (iMs % 1000) * 1000;
        if(iS > 0)
        {
            sleep(iS);
        }
        if(iUs > 0)
        {
            usleep(iUs);
        }
    }

#endif // _WIN32


}


#endif // THREAD_THREADIMPL_HPP_
