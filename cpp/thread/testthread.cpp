#include <iostream>
#include <string>

#include "thread.hpp"


using namespace std;

class Thread1 : public thread::Thread
{
protected:
    virtual void Run(void* arg)
        {
            for (int i = 0; i < 10; i++)
            {
//            std::cout << "Thread1---" << GetName() << i << std::endl;
                printf("Thread1---%d, %i\n", GetId(), i);
                thread::Thread::Sleep(2);
            }
        }
};


class Thread2
{
public:
    void operator()()
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


void Thread5(void* arg)
{
    char* p = (char*)arg;
    for (int i = 0; i < 10; i++)
    {
        printf("Thread5---%i----(%s)\n", i, p);
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

    thread::Thread t4(Thread3);
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

    thread::Thread t6;
    t6.Start();

    thread::Thread t5(Thread5);
    const char* p1 = "1";
    const char* p2 = "2";
    t5.Start((void*)p1);
    t5.Start((void*)p2);
    t5.WaitForEnd();
    const char* p3 = "3";
    t5.Start((void*)p3);
    
    thread::Thread::Sleep(20);
    return 0;
}
