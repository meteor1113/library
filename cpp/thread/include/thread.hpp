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
#include <assert.h>
#include <string>
#include <exception>

// #include <other library headers>

// #include "customer headers"
#include "threadimpl.hpp"


namespace thread
{


    // class ThreadData
    // {
    // private:
    //     ThreadData() {}
    //     virtual ~ThreadData() {}
    //     ThreadData(const ThreadData &);
    //     void operator=(const ThreadData &);

    // public:
    //     static ThreadData* Create() { return new ThreadData; }
    //     void AddRef() { Lock l(mMutex); mRefCount++; }
    //     void Release() { Lock l(mMutex); mRefCount--; if (mRefCount == 0) { delete this; } }

    // private:
    //     ThreadImpl::ThreadStruct mTs;
    //     bool mStop;
    //     ThreadFunc mTf;
    //     void* mArg;
    //     bool mAlive;
    //     int mRefCount;
    //     Mutex mMutex;
    // };


    typedef void (*ThreadFunc)(void* arg);
    /**
     * class Thread
     */
    class Thread
    {
    public:
        explicit Thread(ThreadFunc tf = 0)
            : mTf(tf), mStop(false), mAlive(false) {}
        /** destructor will SetStop() and WaitForEnd */
        virtual ~Thread();

    private:
        Thread(const Thread &);
        void operator=(const Thread &);

    public:
        static void Sleep(int ms) { ThreadImpl::Sleep(ms); }

    public:
        bool Start(void* arg = 0);
        bool WaitForEnd(int ms = ThreadImpl::WAIT_INFINITE);
        void Terminate() { ThreadImpl::TerminateThread(mTs); }
        void SetStop() { mStop = true; }
        bool GetStop() const { return mStop; }
        // bool IsAlive() const { return ThreadImpl::IsAlive(mTs); }
        bool IsAlive() const { return mAlive; }
        int GetId() const { return ThreadImpl::GetThreadId(mTs); }

    protected:
        virtual void Run(void* arg) { assert(mTf != 0); mTf(arg); }

    private:
#ifdef _WIN32
        static unsigned int __stdcall ThreadFunction(void* param);
#else
        static void* ThreadFunction(void* param);
#endif

    private:
        ThreadImpl::ThreadStruct mTs; /* thread information structure */
        bool mStop;
        ThreadFunc mTf;
        void* mArg;
        bool mAlive;
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


    inline bool StartThread(ThreadFunc tf, void* arg = NULL)
    {
#ifdef _WIN32
        return (_beginthread(tf, 0, arg) != -1);
#else
        ThreadImpl::ThreadStruct ts;
        return ThreadImpl::CreateThread(ts, (ThreadImpl::ThreadFuncT)tf, arg);
#endif
    }


    inline Thread::~Thread()
    {
        SetStop();
        WaitForEnd();
        ThreadImpl::DestroyThread(mTs);
    }


    inline bool Thread::Start(void* arg)
    {
        if (IsAlive())
        {
            return false;
        }

        ThreadImpl::DestroyThread(mTs);
        mArg = arg;
        mAlive = true;
        bool ret = ThreadImpl::CreateThread(mTs, ThreadFunction, this);
        if (!ret)
        {
            mAlive = false;
        }
        return ret;
    }


    // inline bool Thread::WaitForEnd(int ms)
    // {
    //     return ThreadImpl::WaitForThreadEnd(mTs, ms);
    // }
    inline bool Thread::WaitForEnd(int ms)
    {
        int iDelta = ThreadImpl::WAIT_TIME_SLICE;
        int iTotal = ms;
        if (ms == ThreadImpl::WAIT_INFINITE)
        {
            iDelta = 0;
            iTotal = 1;
        }
        for (int i = 0; i < iTotal; i += iDelta)
        {
            if (!IsAlive())
            {
                return true;
            }
            Sleep(ThreadImpl::WAIT_TIME_SLICE);
        }
        return false;
    }


#ifdef _WIN32
    inline unsigned int __stdcall Thread::ThreadFunction(void* param)
#else
        inline void* Thread::ThreadFunction(void* param)
#endif
    {
        assert(param != 0);
        Thread* thread = (Thread*)param;
        thread->mAlive = true;
        thread->Run(thread->mArg);
        thread->mAlive = false;
        return 0;
    }


}


#endif // THREAD_THREAD_HPP_
