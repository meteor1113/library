#include <iostream>
#include <string>

#include "thread.hpp"


using namespace std;

class Thread1 : public thread::Thread
{
public:
    Thread1(const std::string& threadName)
        :Thread(threadName) {}
    virtual int Run()
        {
            for (int i = 0; i < 10; i++)
            {
//            std::cout << "Thread1---" << GetName() << i << std::endl;
                printf("Thread1---%s%i\n", GetName().c_str(), i);
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


void Thread3()
{
    for (int i = 0; i < 10; i++)
    {
        //std::cout << "Thread3---" << i << std::endl;
        printf("Thread3---%i\n", i);
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

    Thread1 t("aaaa");
    t.Start();
    t.WaitForEnd();

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt, "bbbb");
    t2.Start();

    Thread2 tt2;
    thread::ThreadHolder<Thread2> t3(tt2, "cccc");
//    t3.Start();

    thread::ThreadHelper t4(Thread3, "dddd");
    t4.Start();

    t2.WaitForEnd();
    t2.WaitForEnd();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;



    t3.WaitForEnd();
    t4.WaitForEnd();

#ifdef _MSC_VER
    getchar();
#endif

    return 0;
}
