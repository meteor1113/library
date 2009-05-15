#include <time.h>
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
            thread::Thread::Sleep(10000);

            delete this;
        }

private:
    int index;
    double a[10000];

};


int main(int argc, char* argv[])
{
    int index = 0;
    int randdiv = 100;
    srand((unsigned)time(NULL));
    thread::ThreadPool pool(40, 50, 10);
    while (true)
    {
        if (pool.GetThreadCount() > 60) randdiv = 10;
        if (pool.GetThreadCount() < 50) randdiv = 1000;
        if (pool.GetTaskCount() > 100)
        {
            pool.SetMinThread(pool.GetMinThread() + 10);
            pool.SetMaxThread(pool.GetMaxThread() + 10);
            pool.SetIdleTime(pool.GetIdleTime() + 10);
        }
        if (pool.GetTaskCount() <= 0)
        {
            //pool.RemoveAllThreads();
            pool.SetMinThread(40);
            pool.SetMaxThread(50);
            pool.SetIdleTime(10);
        }

        unsigned short sleep = (unsigned short)rand() / randdiv;
        std::cout << "thread=" << pool.GetThreadCount()
                  << " idle=" << pool.GetIdleThreadCount()
                  << " tcount=" << pool.GetTaskCount()
                  << " sleep=" << sleep
                  << " idx=" << index + 1
                  << " min=" << pool.GetMinThread()
                  << " max=" << pool.GetMaxThread()
                  << " itime=" << pool.GetIdleTime() << "s"
#ifdef _DEBUG
                  << " hist=" << pool.GetHistThreadCount()
#endif
                  << std::endl;
        pool.AddTask(new MyTask(index++));
        thread::Thread::Sleep(sleep);
    }
    
    return 0;
}
