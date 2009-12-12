#include <stdio.h>
#include <stdlib.h>

#include <iostream>

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
            delete this;
        }

private:
    int index;
};


int main(int argc, char* argv[])
{
    int index = 0;
    thread::ThreadPool pool(50, 60);
    while (true)
    {
        std::cout << "thread=" << pool.GetThreadCount()
                  << " idle=" << pool.GetIdleThreadCount()
                  << " tcount=" << pool.GetPendingTaskCount()
                  << " idx=" << index + 1
//                  << " min=" << pool.GetMinThread()
                  << " max=" << pool.GetMaxThread()
                  << " itime=" << pool.GetIdleTime() << "s"
                  << " hist=" << pool.GetHistThreadCount()
                  << std::endl;
        fflush(stdout);
        pool.Add(new MyTask(index++));
        thread::Thread::Sleep(10);
    }
    
    return 0;
}
