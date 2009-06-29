#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

#include "logfile.hpp"


void Test()
{
    LogFile log("a.log");
    log.Log("aaa");
    log.Log("bbb");
}


int main()
{
    Test();
    return 0;
}
