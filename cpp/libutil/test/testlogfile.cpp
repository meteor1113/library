#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#endif

#include "logfile.hpp"
#include "dailylogfile.hpp"


void TestPre()
{
    LogFile::Instance().SetFilepath("default.log");
    LogFile::Instance().SetLayout("[myproject] {LOG} {LEVEL} {DATE}");
    LogFile::Instance().SetDateFormat("%Y-%m-%d");
    LogFile::Instance().SetLevel(LOGLEVEL_INFO);
}


void Test()
{
    LogFile log;
    // log.SetFilepath("a.log");
    log.Info("aaa");
    log.Warn("bbb");
    log.Error("ccc");
    log.Trace("ddd");
}


void Test1()
{
    LogFile::Instance().Trace("trace");
    LogFile::Instance().Warn("warn");
    LogFile::Instance().Error("error");
    LogFile::Instance().Fatal("fatal");
}


void DailyTestPre()
{
    DailyLogFile::Instance().SetLogdir("../log");
    // DailyLogFile::Instance().SetLayout("[bank_client] {LOG} {LEVEL} {DATE}");
    // DailyLogFile::Instance().SetDateFormat("%Y-%m-%d");
    DailyLogFile::Instance().SetLevel(LOGLEVEL_INFO);
}


void DailyTest()
{
    DailyLogFile log;
    // log.SetFilepath("a.log");
    log.SetFilenamePattern("log4j%Y%m%d%H%M%S.log");
    log.Info("aaa");
    log.Warn("bbb");
#ifdef _WIN32
    Sleep(2000);
#else
    sleep(2);
#endif
    log.Error("ccc");
    log.Trace("ddd");

    log.SetFilenamePattern("");
    log.Fatal("fatal");
}


void DailyTest1()
{
    DailyLogFile::Instance().Trace("trace");
    DailyLogFile::Instance().Warn("warn");
    DailyLogFile::Instance().Error("error");
    DailyLogFile::Instance().Fatal("fatal");
}


int main()
{
    std::cout << "======test LogFile begin" << std::endl;
    TestPre();
    Test();
    Test1();
    std::cout << "======test LogFile end" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "======test DailyLogFile begin" << std::endl;
    DailyTestPre();
    DailyTest();
    DailyTest1();
    std::cout << "======test DailyLogFile end" << std::endl;

    return 0;
}
