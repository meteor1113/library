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

    class ThreadException : public std::exception
    {
    public:
        ThreadException(const std::string& s = "") : msg(s) {}
        virtual ~ThreadException() throw() {}

    public:
        virtual const char* what() const throw () { return msg.c_str(); }
        virtual const std::string What() const { return msg; }

    private:
        std::string msg;

    };


    /**
     * description
     */
    class Thread
    {
    public:
        explicit Thread() : mRetValue(0), mStop(false), mAlive(false) {}
        virtual ~Thread() { if (IsAlive()) { OnLeaking(); } }

    private:
        Thread(const Thread &);
        void operator=(const Thread &);

    public:
        static void Sleep(int ms) { ThreadImpl::Sleep(ms); }

    public:
        void Start(unsigned int ss = DEFAULT_STACK_SIZE);
        bool WaitForEnd(int ms = WAIT_INFINITE);
        void Terminate();
        void SetStop() { mStop = true; }
        bool GetStop() const { return mStop; }
        bool IsAlive() const { return mAlive; }
        int GetId() const { return ThreadImpl::GetThreadId(mTs); }
        int GetRetValue() const { return mRetValue; }

    protected:
        virtual int Run() = 0;
        virtual void OnException(const ThreadException& e);
        virtual void OnLeaking() throw() {printf("OnLeaking\n"); }

    private:
        void SetRetValue(const int& ret) { mRetValue = ret; }
        void Cleanup() { ThreadImpl::DestroyThread(mTs); }
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
        int mRetValue;
        bool mStop;
        bool mAlive;

    };


    template <typename T>
    class ThreadHolder : public Thread
    {
    public:
        ThreadHolder(T& t) : mT(t) {}
        virtual ~ThreadHolder() {}

    protected:
        virtual int Run() { mT(); return 0; }

    private:
        T& mT;

    };


    typedef void (*TypeThreadFunction)(void* arg);
    class ThreadHelper : public Thread
    {
    public:
        ThreadHelper(TypeThreadFunction tf, void* arg = NULL)
            : mTf(tf), mArg(arg) {}
        virtual ~ThreadHelper() {}

    protected:
        virtual int Run() { mTf(mArg); return 0; }

    private:
        TypeThreadFunction mTf;
        void* mArg;

    };


    inline bool StartThread(TypeThreadFunction func, void* arg = NULL)
    {
#ifdef _WIN32
        typedef void (*THREAD_FUNCTION)(void* param);
        int handle = _beginthread((THREAD_FUNCTION)func,
                                  Thread::DEFAULT_STACK_SIZE, arg);
        return (handle != -1L);
#else
        ThreadImpl::ThreadStruct ts;
        return ThreadImpl::CreateThread(ts, (ThreadImpl::THREAD_FUNCTION)func,
                                        Thread::DEFAULT_STACK_SIZE, arg);
#endif
    }


    inline void Thread::Start(unsigned int ss)
    {
        if (IsAlive())
        {
            throw ThreadException("Thread started.");
        }

        mAlive = true;
        bool res = ThreadImpl::CreateThread(mTs, ThreadFunction, ss, this);
        if (!res)
        {
            mAlive = false;
            throw ThreadException("CreateThread failed.");
        }
    }

    inline bool Thread::WaitForEnd(int ms)
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
            if (!IsAlive())
            {
                return true;
            }
            Sleep(WAIT_TIME_SLICE);
        }
        return false;
    }

    inline void Thread::Terminate()
    {
        ThreadImpl::TerminateThread(mTs);
        ThreadImpl::DestroyThread(mTs);
        mAlive = false;
    }

    inline void Thread::OnException(const ThreadException& e)
    {
        std::cout << "Exception:" << e.What() << std::endl;
    }

#ifdef _WIN32
    inline unsigned int __stdcall Thread::ThreadFunction(void* param)
#else
        inline void* Thread::ThreadFunction(void* param)
#endif
    {
        assert(param != NULL);
        Thread* thread = (Thread*)param;
        try
        {
            int ret = thread->Run();
            thread->SetRetValue(ret);
        }
        catch (const ThreadException& ex)
        {
            thread->OnException(ex);
        }
        catch (...)
        {
            thread->OnException(ThreadException("unknown exception"));
        }
        thread->Cleanup();
        thread->mAlive = false;
        return 0;
    }

}


#endif // THREAD_THREAD_HPP_
