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

// #include <other library headers>

// #include "custom headers"
#include "thread.hpp"
#include "mutex.hpp"


namespace thread
{


class Task
{
public:
    virtual ~Task() {}
    virtual void Run() = 0;
};


class TaskQueue
{
public:
    TaskQueue() {}
    /*virtual*/ ~TaskQueue() {}

public:
    void Push(Task* t) { Lock l(mutex); coll.push_back(t); }
    Task* Pop();
    bool Priority(Task* t);
    int Size() const { return coll.size(); }
    void Clear() { Lock l(mutex); coll.clear(); }
    const std::list<Task*> ToList() const { Lock l(mutex); return coll; }

private:
    std::list<Task*> coll;
    mutable Mutex mutex;
};


class TaskThread : public Thread, public Stopable
{
public:
    TaskThread(TaskQueue& q) : queue(q), currentTask(NULL) {}
    virtual ~TaskThread() { WaitForEnd(); }

public:
    bool GetRunning() const { return (currentTask != NULL); }
    Task* GetCurrentTask() { return currentTask; }
    int GetIdleTime() const { return (time(NULL) - idleStart); }

protected:
    virtual void Run(void* arg);

private:
    TaskQueue& queue;
    Task* currentTask;
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
    ThreadPool(int max = 100, int idle = 60)
        : maxThread(max), idleTime(idle), histThread(0) {}
    /*virtual*/ ~ThreadPool() { Stop(); }

private:
    ThreadPool(const ThreadPool& rhs);
    ThreadPool& operator=(const ThreadPool& rhs);

public:
    void Add(Task* t);
    void Priority(Task* t);
    void Stop();
    std::list<Task*> GetRunningTasks() const;
    std::list<Task*> GetPendingTasks() const { return queue.ToList(); }
    void ClearPendingTasks() { queue.Clear(); }
    int GetPendingTaskCount() const { return queue.Size(); }
    int GetThreadCount() const{ return threads.size(); }
    int GetIdleThreadCount() const;
    int GetHistThreadCount() const { return histThread; }
    int GetMaxThread() const { return maxThread; }
    void SetMaxThread(const int& value) { maxThread = value; }
    int GetIdleTime() const { return idleTime; }
    void SetIdleTime(const int& value) { idleTime = value; }

protected:

private:
    bool RemoveOneIdleTimeoutThread();
    void RemoveStopedThreads();
    void AddThread(int count);

private:
    std::list<TaskThread*> threads;
    std::list<TaskThread*> stopThreads;
    TaskQueue queue;
    int maxThread;
    int idleTime;
    int histThread;
};


inline Task* TaskQueue::Pop()
{
    Lock l(mutex);
    Task* t = NULL;
    if (coll.size() > 0)
    {
        t = coll.front();
        coll.pop_front();
    }
    return t;
}


inline bool TaskQueue::Priority(Task* t)
{
    Lock l(mutex);
    std::list<Task*>::iterator i;
    for (i = coll.begin(); i != coll.end(); ++i)
    {
        if (*i == t)
        {
            coll.erase(i);
            coll.push_front(t);
            return true;
        }
    }
    return false;
}


inline void TaskThread::Run(void* arg)
{
    idleStart = time(NULL);
    do
    {
        currentTask = queue.Pop();
        if (currentTask != NULL)
        {
            currentTask->Run();
            currentTask = NULL;
            idleStart = time(NULL);
        }
        else
        {
            Sleep(1);
        }
    }
    while (!GetStop());
}


inline void ThreadPool::Add(Task* t)
{
    assert(t != NULL);

    queue.Push(t);
    if (queue.Size() > GetIdleThreadCount()) // may be need add new thread
    {
        int need1 = maxThread - GetThreadCount();
        int need2 = queue.Size() - GetIdleThreadCount();
        int count = (need1 > need2) ? need2 : need1;
        AddThread(count);
    }
    else                    // may be need delete idle timeout thread
    {
        while (RemoveOneIdleTimeoutThread())
        {
        }
    }
    RemoveStopedThreads();
}


inline void ThreadPool::Priority(Task* t)
{
    if (queue.Priority(t))
    {
        TaskThread* thread = new TaskThread(queue);
        thread->Start();
        thread->SetStop();
        stopThreads.push_back(thread);
        histThread++;
    }
}


inline void ThreadPool::Stop()
{
    std::list<TaskThread*>::iterator i;
    for (i = threads.begin(); i != threads.end(); ++i)
    {
        (*i)->SetStop();
    }
    for (i = stopThreads.begin(); i != stopThreads.end(); ++i)
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


inline std::list<Task*> ThreadPool::GetRunningTasks() const
{
    std::list<Task*> col;
    std::list<TaskThread*>::const_iterator i;
    for (i = threads.begin(); i != threads.end(); ++i)
    {
        Task* t = (*i)->GetCurrentTask();
        if (t != NULL)
        {
            col.push_back(t);
        }
    }
    for (i = stopThreads.begin(); i != stopThreads.end(); ++i)
    {
        Task* t = (*i)->GetCurrentTask();
        if (t != NULL)
        {
            col.push_back(t);
        }
    }
    return col;
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


inline void ThreadPool::AddThread(int count)
{
    for (int i = 0; i < count; i++)
    {
        TaskThread* thread = new TaskThread(queue);
        thread->Start();
        threads.push_back(thread);
        histThread++;
    }
}


}


#endif // THREAD_THREADPOOL_HPP_
