#include <stdio.h>

#include <iostream>
#include <string>

#include "thread.hpp"


using namespace std;


class Thread1 : public thread::Thread
{
protected:
    virtual void Run(void* arg)
        {
            for (int i = 0; i < 5; i++)
            {
                printf("Thread1---%i\n", i);
                fflush(stdout);
                thread::Thread::Sleep(1);
            }
        }
};


class Thread2
{
public:
    void operator()()
        {
            for (int i = 0; i < 5; i++)
            {
                printf("Thread2---%i\n", i);
                fflush(stdout);
                thread::Thread::Sleep(1);
            }
        }
};


void Thread3(void* arg)
{
}


void Thread4(void* arg)
{
    for (int i = 0; i < 5; i++)
    {
        printf("Thread4---%i\n", i);
        fflush(stdout);
        thread::Thread::Sleep(1);
    }
}


void Thread5(void* arg)
{
    char* p = (char*)arg;
    for (int i = 0; i < 5; i++)
    {
        printf("Thread5---%i----(%s)\n", i, p);
        fflush(stdout);
        thread::Thread::Sleep(20);
    }
}


void Thread7(void* arg)
{
    char* p = (char*)arg;
    for (int i = 0; i < 5; i++)
    {
        printf("Thread7---%i----(%s)\n", i, p);
        fflush(stdout);
        thread::Thread::Sleep(20);
    }
}


class Thread8 : public thread::Thread
{
public:
    virtual ~Thread8() { WaitForEnd();  }

protected:
    virtual void Run(void* arg)
        {
            for (int i = 0; i < 5; i++)
            {
                printf("Thread8---%i---id:%d\n", i, GetId());
                fflush(stdout);
                thread::Thread::Sleep(1);
            }
        }
};


template <typename T>
void Delete(T* obj)
{
    std::cout << "delete T* obj" << std::endl;
    delete obj;
}


int main(int argc, char* argv[])
{
    Thread1* t1 = new Thread1();
    t1->Start();
    printf("Thread1-id:%d\n", t1->GetId());
    fflush(stdout);
    delete t1;
    t1 = NULL;
    printf("delete Thread1\n");
    fflush(stdout);

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt);
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());
    fflush(stdout);
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());
    fflush(stdout);

    thread::Thread t3(Thread3);
    t3.Start();

    thread::Thread t4(Thread4);
    t4.Start();
    printf("Thread4-id:%d\n", t4.GetId());
    fflush(stdout);

    t2.WaitForEnd();
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());
    fflush(stdout);
    t2.WaitForEnd();
    t2.WaitForEnd();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;
    fflush(stdout);

    t3.WaitForEnd();
    t4.WaitForEnd();

    const char* p3 = "3";
    const char* p4 = "4";
    thread::StartThread(Thread7, (void*)p3);
    thread::StartThread(Thread7, (void*)p4);

    const char* p1 = "1";
    const char* p2 = "2";
    thread::Thread t5(Thread5);
    t5.Start((void*)p1);
    printf("Thread5-id:%d\n", t5.GetId());
    fflush(stdout);
    t5.Start((void*)p2);
    printf("Thread5-id:%d\n", t5.GetId());
    fflush(stdout);

    Thread8 t8;
    t8.Start();
    printf("Thread8-id:%d\n", t8.GetId());
    fflush(stdout);

//    thread::Thread::Sleep(10);
    printf("exit\n");
    fflush(stdout);
    return 0;
}
