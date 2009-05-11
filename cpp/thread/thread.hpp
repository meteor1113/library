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


#ifndef THREAD_THREAD_HPP_
#define THREAD_THREAD_HPP_


// #include <standard library headers>
#include <stdio.h>
#include <cassert>
#include <string>
#include <exception>
#include <iostream>

// #include <other library headers>

// #include "customer headers"
#include "threadimpl.hpp"


namespace thread
{

    typedef void (*TypeThreadFunction)(void* arg);
    /**
     * class Thread
     */
    class Thread
    {
    public:
        explicit Thread(TypeThreadFunction tf = NULL)
            : mTf(tf), mStop(false) {}
        virtual ~Thread() { ThreadImpl::DestroyThread(mTs); }

    private:
        Thread(const Thread &);
        void operator=(const Thread &);

    public:
        static void Sleep(int ms) { ThreadImpl::Sleep(ms); }

    public:
        bool Start(void* arg = NULL);
        bool WaitForEnd(int ms = WAIT_INFINITE);
        void Terminate() { ThreadImpl::TerminateThread(mTs); }
        void SetStop() { mStop = true; }
        bool GetStop() const { return mStop; }
        bool IsAlive() const { return ThreadImpl::IsAlive(mTs); }
        int GetId() const { return ThreadImpl::GetThreadId(mTs); }

    protected:
        virtual void Run(void* arg) { if (mTf != NULL) { mTf(arg); } }

    private:
#ifdef _WIN32
        static unsigned int __stdcall ThreadFunction(void* param);
#else
        static void* ThreadFunction(void* param);
#endif

    public:
#if _MSC_VER < 1300 // < vc7
        enum { WAIT_INFINITE = ThreadImpl::WAIT_INFINITE };
        enum { INVALID_THREAD_ID = ThreadImpl::INVALID_THREAD_ID };
        enum { DEFAULT_STACK_SIZE = ThreadImpl::DEFAULT_STACK_SIZE };
        enum { WAIT_TIME_SLICE = ThreadImpl::WAIT_TIME_SLICE };
#elif
        static const int WAIT_INFINITE = ThreadImpl::WAIT_INFINITE;
        static const int INVALID_THREAD_ID = ThreadImpl::INVALID_THREAD_ID;
        static const size_t DEFAULT_STACK_SIZE = ThreadImpl::DEFAULT_STACK_SIZE;
        static const int WAIT_TIME_SLICE = ThreadImpl::WAIT_TIME_SLICE;
#endif

    private:
        ThreadImpl::ThreadStruct mTs; /* thread information structure */
        bool mStop;
        TypeThreadFunction mTf;
        void* mArg;

    };


    template <typename T>
    class ThreadHolder : public Thread
    {
    public:
        ThreadHolder(T& t) : mT(t) {}
        virtual ~ThreadHolder() {}

    protected:
        virtual void Run(void* arg) { mT(); }

    private:
        T& mT;

    };


    inline bool Thread::Start(void* arg)
    {
        if (IsAlive())
        {
            return false;
        }

        ThreadImpl::DestroyThread(mTs);
        mArg = arg;
        return ThreadImpl::CreateThread(mTs, ThreadFunction,
                                        DEFAULT_STACK_SIZE, this);
    }

    inline bool Thread::WaitForEnd(int ms)
    {
        return ThreadImpl::WaitForThreadEnd(mTs, ms);
    }

#ifdef _WIN32
    inline unsigned int __stdcall Thread::ThreadFunction(void* param)
#else
        inline void* Thread::ThreadFunction(void* param)
#endif
    {
        assert(param != NULL);
        Thread* thread = (Thread*)param;
        thread->Run(thread->mArg);
        return 0;
    }

}


#endif // THREAD_THREAD_HPP_
