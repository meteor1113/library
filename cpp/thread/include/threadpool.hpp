/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-05-06
 */


#ifndef THREAD_THREADPOOL_HPP_
#define THREAD_THREADPOOL_HPP_


// #include <standard C library headers>
#include <time.h>
#include <assert.h>

// #include <standard C++ library headers>
#include <list>
#include <queue>

// #include <other library headers>

// #include "custom headers"
#include "thread.hpp"
#include "mutex.hpp"


namespace thread
{


    class Task
    {
    public:
        virtual ~Task()	{}
        virtual void Run() = 0;
    };


    class TaskQueue
    {
    public:
        TaskQueue() {}
        /*virtual*/ ~TaskQueue() {}

    public:
        void Push(Task* t) { Lock l(mutex); queue.push(t); }
        Task* Pop();
        int Size() const { return queue.size(); }

    private:
        std::queue<Task*> queue;
        Mutex mutex;
    };


    class TaskThread : public Thread
    {
    public:
        TaskThread(TaskQueue& q) : queue(q), running(false) {}
        virtual ~TaskThread() {}

    public:
        bool GetRunning() const { return running; }
        int GetIdleTime() const { return (time(NULL) - idleStart); }

    protected:
        virtual void Run(void* arg);

    private:
        TaskQueue& queue;
        bool running;
        int idleStart;
    };


    class ThreadPool
    {
    public:
        /**
         * @param min min threads
         * @param max max threads
         * @param idle in seconds
         */
        ThreadPool(int min = 10, int max = 100, int idle = 60);
        /*virtual*/ ~ThreadPool();

    private:
        ThreadPool(const ThreadPool& rhs);
        ThreadPool& operator=(const ThreadPool& rhs);

    public:
        void AddTask(Task* t);
        void StopAll();
        int GetThreadCount() const{ return threads.size(); }
        int GetIdleThreadCount() const;
        int GetHistThreadCount() const { return histThreadCount; }
        TaskQueue& GetPendingTasks() { return queue; }
        int GetPendingTaskCount() const { return queue.Size(); }
        int GetMinThread() const { return minThread; }
        void SetMinThread(const int& value) { minThread = value; }
        int GetMaxThread() const { return maxThread; }
        void SetMaxThread(const int& value) { maxThread = value; }
        int GetIdleTime() const { return idleTime; }
        void SetIdleTime(const int& value) { idleTime = value; }

    protected:

    private:
        bool RemoveOneIdleTimeoutThread();
        void RemoveStopedThreads();
        void AddNewThread(int count);

    private:
        std::list<TaskThread*> threads;
        std::list<TaskThread*> stopThreads;
        TaskQueue queue;
        int minThread;
        int maxThread;
        int idleTime;
        int histThreadCount;
    };


    inline Task* TaskQueue::Pop()
    {
        Lock l(mutex);
        Task* t = NULL;
        if (queue.size() > 0)
        {
            t = queue.front();
            queue.pop();
        }
        return t;
    }


    inline void TaskThread::Run(void* arg)
    {
        idleStart = time(NULL);
        while (!GetStop())
        {
            Task* t = queue.Pop();
            if (t == NULL)
            {
                Sleep(10);
                continue;
            }
            running = true;
            t->Run();
            running = false;
            idleStart = time(NULL);
        }
    }


    inline ThreadPool::ThreadPool(int min, int max, int idle)
        : minThread(min), maxThread(max), idleTime(idle), histThreadCount(0)
    {
        AddNewThread(minThread);
    }


    inline ThreadPool::~ThreadPool()
    {
        StopAll();
        std::list<TaskThread*>::iterator i;
        for (i = stopThreads.begin(); i != threads.end(); ++i)
        {
            delete (*i);
        }
        stopThreads.clear();
        for (i = threads.begin(); i != threads.end(); ++i)
        {
            delete (*i);
        }
        threads.clear();
    }


    inline void ThreadPool::AddTask(Task* t)
    {
        assert(t != NULL);

        queue.Push(t);
        if (queue.Size() > GetIdleThreadCount()) // may be need add new thread
        {
            int need1 = maxThread - GetThreadCount();
            int need2 = queue.Size() - GetIdleThreadCount();
            int count = (need1 > need2) ? need2 : need1;
            AddNewThread(count);
        }
        else                    // may be need delete idle timeout thread
        {
            while ((GetThreadCount() > minThread)
                   && RemoveOneIdleTimeoutThread())
            {
            }
        }
        RemoveStopedThreads();
    }


    inline void ThreadPool::StopAll()
    {
        std::list<TaskThread*>::iterator i;
        for (i = threads.begin(); i != threads.end(); ++i)
        {
            (*i)->SetStop();
        }
        stopThreads.insert(stopThreads.end(), threads.begin(), threads.end());
        threads.clear();
    }


    inline int ThreadPool::GetIdleThreadCount() const
    {
        int count = 0;
        for (std::list<TaskThread*>::const_iterator i = threads.begin();
             i != threads.end(); ++i)
        {
            count += ((*i)->GetRunning() ? 0 : 1);
        }
        return count;
    }


    inline bool ThreadPool::RemoveOneIdleTimeoutThread()
    {
        for (std::list<TaskThread*>::iterator i = threads.begin();
             i != threads.end(); ++i)
        {
            if ((!(*i)->GetRunning()) && ((*i)->GetIdleTime() > idleTime))
            {
                (*i)->SetStop();
                stopThreads.push_back(*i);
                threads.erase(i);
                return true;
            }
        }
        return false;
    }


    inline void ThreadPool::RemoveStopedThreads()
    {
        std::list<TaskThread*> tmp;
        for (std::list<TaskThread*>::iterator i = stopThreads.begin();
             i != stopThreads.end(); ++i)
        {
            if ((*i)->IsAlive())
            {
                tmp.push_back(*i);
            }
            else
            {
                delete *i;
            }
        }
        stopThreads.swap(tmp);
    }


    inline void ThreadPool::AddNewThread(int count)
    {
        for (int i = 0; i < count; i++)
        {
            TaskThread* thread = new TaskThread(queue);
            thread->Start();
            threads.push_back(thread);
            histThreadCount++;
        }
    }


}


#endif // THREAD_THREADPOOL_HPP_
