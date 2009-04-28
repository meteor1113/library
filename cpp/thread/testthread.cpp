#include <iostream>
#include <string>

#include "thread.hpp"


using namespace std;

class Thread1 : public thread::Thread
{
public:
    Thread1() {}
    virtual int Run()
        {
            for (int i = 0; i < 10; i++)
            {
//            std::cout << "Thread1---" << GetName() << i << std::endl;
                printf("Thread1---%d, %i\n", GetId(), i);
                thread::Thread::Sleep(2);
            }
            return 0;
        }
};


class Thread2
{
public:
    void operator () ()
        {
            for (int i = 0; i < 10; i++)
            {
                //std::cout << "Thread2---" << i << std::endl;
                printf("Thread2---%i\n", i);
                thread::Thread::Sleep(2);
            }
        }
};


void Thread3(void* arg)
{
    for (int i = 0; i < 10; i++)
    {
        //std::cout << "Thread3---" << i << std::endl;
        printf("Thread3---%i\n", i);
        thread::Thread::Sleep(2);
    }
}


void Thread4(void* arg)
{
    for (int i = 0; i < 10; i++)
    {
        printf("Thread4---%i\n", i);
        thread::Thread::Sleep(1);
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
    t.WaitForEnd();

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt);
    t2.Start();
    try
    {
        t2.Start();
    }
    catch (...)
    {
        std::cout << "Start Thread2 failed." << std::endl;
    }

    Thread2 tt2;
    thread::ThreadHolder<Thread2> t3(tt2);
//    t3.Start();

    thread::ThreadHelper t4(Thread3);
    t4.Start();

    t2.WaitForEnd();
    t2.Start();
    t2.WaitForEnd();
    t2.WaitForEnd();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;


    t3.WaitForEnd();
    t4.WaitForEnd();
    t4.Start();

    thread::StartThread(Thread4, NULL);
    thread::StartThread(Thread4, NULL);
    
    thread::Thread::Sleep(10);
    return 0;
}
