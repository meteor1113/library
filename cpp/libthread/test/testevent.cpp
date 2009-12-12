#include <stdio.h>

#include <iostream>
#include <string>

#include "thread.hpp"
#include "event.hpp"


using namespace std;



thread::Event gEvent;


class Thread1 : public thread::Thread
{
public:
    virtual void Run(void* arg)
        {
            for (int i = 0; i < 5; i++)
            {
                printf("Thread1---%d, %i\n", GetId(), i);
                fflush(stdout);
                thread::Thread::Sleep(2);
            }
            thread::Thread::Sleep(200);
            gEvent.NotifyAll();
        }
};


class Thread2
{
public:
    void operator()()
        {
            printf("Thread2-Wait\n"); fflush(stdout);
            gEvent.Wait();
            printf("Thread2-Waited\n"); fflush(stdout);
            for (int i = 0; i < 5; i++)
            {
                printf("Thread2---%i\n", i);
                fflush(stdout);
                thread::Thread::Sleep(2);
            }
        }
};


void Thread3(void* arg)
{
    printf("Thread3-Wait\n"); fflush(stdout);
    gEvent.Wait();
    printf("Thread3-Waited\n"); fflush(stdout);
    for (int i = 0; i < 5; i++)
    {
        printf("Thread3---%i\n", i);
        fflush(stdout);
        thread::Thread::Sleep(2);
    }
}


template <typename T>
void Delete(T* obj)
{
    std::cout << "delete T* obj" << std::endl;
    delete obj;
}


int main(int argc, char* argv[])
{

    Thread1 t;
    t.Start();

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt);
    t2.Start();

    thread::Thread t3(Thread3);
    t3.Start();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;
    fflush(stdout);

    t.WaitForEnd();
    t2.WaitForEnd();
    t3.WaitForEnd();

    return 0;
}
