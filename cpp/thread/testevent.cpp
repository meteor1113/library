#include <iostream>
#include <string>

#include "thread.hpp"
#include "event.hpp"


using namespace std;



thread::Event gEvent;


class Thread1 : public thread::Thread
{
public:
    Thread1(const std::string& threadName)
        :Thread(threadName) {}
    virtual int Run()
        {
            for (int i = 0; i < 10; i++)
            {
                printf("Thread1---%s%i\n", GetName().c_str(), i);
                fflush(stdout);
                thread::Thread::Sleep(2);
            }
            thread::Thread::Sleep(5000);
            gEvent.SignalAll();
            return 0;
        }
};


class Thread2
{
public:
    void operator () ()
        {
            gEvent.Wait(5000);
            for (int i = 0; i < 10; i++)
            {
                printf("Thread2---%i\n", i);
                fflush(stdout);
                thread::Thread::Sleep(2);
            }
        }
};


void Thread3()
{
    gEvent.Wait();
    for (int i = 0; i < 10; i++)
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

    Thread1 t("aaaa");
    t.Start();

    Thread2 tt;
    thread::ThreadHolder<Thread2> t2(tt, "bbbb");
    t2.Start();

    thread::ThreadHelper t3(Thread3, "dddd");
    t3.Start();

    thread::Thread::Sleep(9);
    std::cout << "main()" << std::endl;
    fflush(stdout);

    t.WaitForEnd();
    t2.WaitForEnd();
    t3.WaitForEnd();

#ifdef _MSC_VER
    getchar();
#endif

    return 0;
}
