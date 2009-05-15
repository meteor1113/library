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
                printf("Thread1---id:%d, %i\n", GetId(), i);
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
        thread::Thread::Sleep(10);
    }
}


void Thread7(void* arg)
{
    char* p = (char*)arg;
    for (int i = 0; i < 10; i++)
    {
        printf("Thread7---%i----(%s)\n", i, p);
        thread::Thread::Sleep(20);
    }
}


void Thread8(void* arg)
{
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
    printf("Thread1-id:%d\n", t.GetId());
    t.WaitForEnd();

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt);
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());

    Thread2 tt2;
    thread::ThreadHolder<Thread2> t3(tt2);
//    t3.Start();

    thread::Thread t4(Thread3);
    t4.Start();
    printf("Thread4-id:%d\n", t4.GetId());

    t2.WaitForEnd();
    t2.Start();
    printf("Thread2-id:%d\n", t2.GetId());
    t2.WaitForEnd();
    t2.WaitForEnd();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;

    t3.WaitForEnd();
    t4.WaitForEnd();
    t4.Start();
    printf("Thread4-id:%d\n", t4.GetId());
    t4.WaitForEnd();

    // thread::Thread t6;
    // try
    // {
    //     t6.Start();
    //     printf("Thread6-id:%d\n", t6.GetId());
    // }
    // catch (...)
    // {
    //     printf("thread6 error");
    // }

    thread::Thread t8(Thread8);
    t8.Start();
    printf("Thread8-id:%d\n", t8.GetId());

    const char* p3 = "3";
    const char* p4 = "4";
    thread::StartThread(Thread7, (void*)p3);
    thread::StartThread(Thread7, (void*)p4);

    const char* p1 = "1";
    const char* p2 = "2";
    thread::Thread t5(Thread5);
    t5.Start((void*)p1);
    printf("Thread5-id:%d\n", t5.GetId());
    t5.Start((void*)p2);
    printf("Thread5-id:%d\n", t5.GetId());

    return 0;
}
