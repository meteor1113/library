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


#ifndef THREAD_THREAD_HPP_
#define THREAD_THREAD_HPP_


// #include <standard library headers>
#include <assert.h>

// #include <other library headers>

// #include "customer headers"
#include "threadimpl.hpp"
#include "mutex.hpp"


namespace thread
{


typedef void (*ThreadFunc)(void* arg);


class Thread
{
protected:
    class ThreadData
    {
        friend class Thread;
    private:
        ThreadData() : ref(1) {}
        /*virtual*/ ~ThreadData() {}

    private:
        ThreadData(const ThreadData& rhs);
        ThreadData& operator=(const ThreadData& rhs);

    public:
        bool GetStop() const { return stop; }

    private:
        static ThreadData* Create() { return new ThreadData(); }
        void AddRef() { Lock l(mutex); ref++; }
        void Release();

    private:
        ThreadImpl::ThreadStruct ts;
        void* arg;
        bool start;
        bool alive;
        bool stop;

    private:
        Mutex mutex;
        int ref;
    };

public:
    explicit Thread(ThreadFunc f = 0) : td(0), tf(f) {}
    virtual ~Thread() { if (td != 0) { td->Release(); } }

private:
    Thread(const Thread &);
    void operator=(const Thread &);

public:
    static void Sleep(int ms) { ThreadImpl::Sleep(ms); }

public:
    bool Start(void* arg = 0);
    bool IsAlive() const { return (td == 0) ? false : td->alive; }
    void SetStop() { if (td != 0) { td->stop = true; } }
    bool WaitForEnd(int ms = ThreadImpl::WAIT_INFINITE);
    void Terminate() { if (td != 0) { ThreadImpl::Terminate(td->ts); } }
    int GetId() const { return (td == 0) ? 0 : ThreadImpl::GetId(td->ts); }

protected:
    virtual void Run(const ThreadData& data, void* arg)
        { assert(tf != 0); tf(arg); }

private:
#ifdef _WIN32
    static unsigned int __stdcall ThreadFunction(void* param);
#else
    static void* ThreadFunction(void* param);
#endif

private:
    ThreadData* td;
    ThreadFunc tf;
};


template <typename T>
class ThreadHolder : public Thread
{
public:
    ThreadHolder(T& t) : obj(t) {}
    virtual ~ThreadHolder() {}

protected:
    virtual void Run(const ThreadData& data, void* arg) { obj(); }

private:
    T& obj;
};


inline bool StartThread(ThreadFunc tf, void* arg = 0)
{
#ifdef _WIN32
    return (_beginthread(tf, 0, arg) != -1);
#else
    ThreadImpl::ThreadStruct ts;
    return ThreadImpl::Create(ts, (ThreadImpl::ThreadFuncT)tf, arg);
#endif
}


inline void Thread::ThreadData::Release()
{
    mutex.Lock();
    int r = --ref;
    mutex.UnLock();
    if (r <= 0)
    {
        delete this;
    }
}


inline bool Thread::Start(void* arg)
{
    if (td != 0)
    {
        return false;
    }

    td = ThreadData::Create();
    td->arg = arg;
    td->start = false;
    td->alive = false;
    td->stop = false;
    bool ret = ThreadImpl::Create(td->ts, ThreadFunction, this);
    if (!ret)
    {
        td->Release();
        td = 0;
        return false;
    }
    while (!td->start)
    {
        Sleep(1);
    }
    return ret;
}


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
    ThreadData* data = thread->td;
    data->AddRef();
    data->alive = true;
    data->start = true;

    thread->Run(*data, data->arg);

    data->alive = false;
    data->Release();
    return 0;
}


}


#endif // THREAD_THREAD_HPP_
