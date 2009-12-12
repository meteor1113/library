#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#include "thread.hpp"
#include "threadpool.hpp"


class MyTask : public thread::Task
{
public:
    MyTask(int i) : index(i) {}
    virtual ~MyTask() {}

public:
    virtual void Run()
        {
            std::cout << "task:" << index << std::endl;
            fflush(stdout);
            thread::Thread::Sleep(10000);

            delete this;
        }

private:
    int index;
    double a[100000];

};


int main(int argc, char* argv[])
{
    int index = 0;
    int randdiv = 100;
    srand((unsigned)time(NULL));
    thread::ThreadPool pool(50, 10);
    while (true)
    {
        if (pool.GetThreadCount() > 60) randdiv = 10;
        if (pool.GetThreadCount() < 50) randdiv = 1000;
        if (pool.GetPendingTaskCount() > 100)
        {
            MyTask* t = new MyTask(index++);
            pool.Add(t);
            pool.Priority(t);
            pool.SetMaxThread(pool.GetMaxThread() + 10);
            pool.SetIdleTime(pool.GetIdleTime() + 10);
        }
        if (pool.GetPendingTaskCount() <= 0)
        {
            pool.SetMaxThread(50);
            pool.SetIdleTime(10);
        }

        if (index == 100)
        {
            pool.Stop();
            std::list<thread::Task*> l = pool.GetPendingTasks();
            pool.ClearPendingTasks();
            for (std::list<thread::Task*>::iterator i = l.begin();
                i != l.end(); ++i)
            {
                delete (*i);
            }
            int j = 0;
        }

        unsigned short sleep = (unsigned short)rand() / randdiv;
        std::cout << "thread=" << pool.GetThreadCount()
                  << " idle=" << pool.GetIdleThreadCount()
                  << " tcount=" << pool.GetPendingTaskCount()
                  << " sleep=" << sleep
                  << " idx=" << index + 1
//                  << " min=" << pool.GetMinThread()
                  << " max=" << pool.GetMaxThread()
                  << " itime=" << pool.GetIdleTime() << "s"
                  << " hist=" << pool.GetHistThreadCount()
                  << std::endl;
        fflush(stdout);
        pool.Add(new MyTask(index++));
        thread::Thread::Sleep(sleep);
    }
    
    return 0;
}
