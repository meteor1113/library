#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#endif

#include "logger.hpp"


void TestPre()
{
    Logger::GetLogger().SetFilepath("root.log");
    Logger::GetLogger().SetLayout("[myproject] {LOG} ({PID}) {LEVEL} %Y-%m-%d");
    Logger::GetLogger().SetLevel(LOGLEVEL_INFO);
    Logger::GetLogger("a").SetFilepath("a.log");
    Logger::GetLogger("b").SetFilepath("b.log");
    Logger::GetLogger("b").SetLayout("%Y-%m {LEVEL} {LOG}");
    Logger::GetLogger("c").SetLevel("TRACE");
    Logger::GetLogger("c").SetFilepath("log/log/log.log");
}


void Test()
{
    Logger x1;
    x1.Fatal("test log x1");
    Logger x2;
    x2.Fatal("test log x2");
    Logger::GetLogger("a");
    Logger::GetLogger("b");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("a");
    Logger& log = Logger::GetLogger("a");
    // log.SetFilepath("a.log");
    log.Info("aaa");
    log.Warn("bbb");
    log.Error("ccc");
    log.Trace("ddd");
    Logger::GetLogger("b").Warn("test b");
    Logger::GetLogger("b").Fatal("test fatal b:%d,%s,0%o,0x%x,%e,%c", 1234,"ok",128,256,3.14,'A');
    Logger::GetLogger("c").Warn("test c");
    Logger::GetLogger("c").Trace("trace:%d", 1);
    Logger::GetLogger("c").Debug("debug:%d", 2);
    Logger::GetLogger("c").Info("info:%d", 3);
    Logger::GetLogger("c").Warn("warn:%d", 4);
    Logger::GetLogger("c").Error("error:%d", 5);
    Logger::GetLogger("c").Fatal("fatal:%d", 6);
}


void Test1()
{
    Logger::GetLogger().Trace("trace");
    Logger::GetLogger().Warn("warn");
    Logger::GetLogger().Error("error");
    Logger::GetLogger().Fatal("fatal");
}


void DailyTestPre()
{
    Logger::GetLogger().SetFilepath("../log/%Y%m%d%H.log");
    Logger::GetLogger().SetLevel(LOGLEVEL_INFO);
    Logger::GetLogger("dz").SetLevel("error");
    Logger::GetLogger("dy").SetLevel("aabc");
}


void DailyTest()
{
    Logger dx1;
    dx1.Fatal("test log dx1");
    Logger dx2;
    dx2.Fatal("test log dx2");
    Logger::GetLogger("da");
    Logger::GetLogger("b");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("da");
    Logger& log = Logger::GetLogger("da");
    log.SetFilepath("logger%Y%m%d%H%M%S.log");
    log.Info("aaa");
    log.Warn("bbb");
#ifdef _WIN32
    Sleep(2000);
#else
    sleep(2);
#endif
    log.Error("ccc");
    log.Trace("ddd");

    log.Fatal("fatal");
    Logger::GetLogger("dz").Trace("test dz trace");
    Logger::GetLogger("dz").Debug("test dz debug");
    Logger::GetLogger("dz").Info("test dz info");
    Logger::GetLogger("dz").Warn("test dz warn");
    Logger::GetLogger("dz").Error("test dz error");
    Logger::GetLogger("dz").Fatal("test dz fatal");
    Logger::GetLogger("dy").Trace("test dy trace");
    Logger::GetLogger("dy").Debug("test dy debug");
    Logger::GetLogger("dy").Info("test dy info");
    Logger::GetLogger("dy").Warn("test dy warn");
    Logger::GetLogger("dy").Error("test dy error");
    Logger::GetLogger("dy").Fatal("test dy fatal");
}


void DailyTest1()
{
    Logger::GetLogger().Trace("trace");
    Logger::GetLogger().Warn("warn");
    Logger::GetLogger().Error("error");
    Logger::GetLogger().Fatal("fatal");
}


int main()
{
    std::cout << "======test Logger begin" << std::endl;
    TestPre();
    Test();
    Test1();
    std::cout << "======test Logger end" << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "======test DailyLogger begin" << std::endl;
    DailyTestPre();
    DailyTest();
    DailyTest1();
    std::cout << "======test DailyLogger end" << std::endl;

    return 0;
}
