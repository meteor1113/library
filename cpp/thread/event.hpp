/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-04-16
 */


#ifndef THREAD_EVENT_H_
#define THREAD_EVENT_H_


// #include <standard C library headers>

// #include <standard C++ library headers>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

// #include "custom headers"


namespace thread
{


    class Event
    {
    public:
        Event();
        /*virtual*/ ~Event();

    private:
        Event(const Event& rhs);
        Event& operator=(const Event& rhs);

    public:
        void SignalAll();
        void Wait();
        bool Wait(int ms);

    protected:

    private:
#ifdef _WIN32
        HANDLE event;
#else
        pthread_cond_t cond;
        pthread_mutex_t mutex;
#endif

    };


#ifdef _WIN32

    inline Event::Event()
    {
        event = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    inline Event::~Event()
    {
        ::CloseHandle(event);
    }

    inline void Event::SignalAll()
    {
        ::PulseEvent(event);
    }

    inline void Event::Wait()
    {
        ::WaitForSingleObject(event, INFINITE);
    }

    inline bool Event::Wait(int ms)
    {
        return (::WaitForSingleObject(event, ms) == WAIT_OBJECT_0);
    }

#else

    inline Event::Event()
    {
        pthread_cond_init(&cond, NULL);
        pthread_mutex_init(&mutex, NULL);
    }

    inline Event::~Event()
    {
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }

    inline void Event::SignalAll()
    {
        pthread_cond_broadcast(&cond);
    }

    inline void Event::Wait()
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    inline bool Event::Wait(int ms)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += (ms / 1000);
        ts.tv_nsec += ((ms % 1000) * 1000);
        pthread_mutex_lock(&mutex);
        bool ret = (pthread_cond_timedwait(&cond, &mutex, &ts) == 0);
        pthread_mutex_unlock(&mutex);
        return ret;
    }

#endif // _WIN32


} // namespace thread


#endif // EVENT_H_
