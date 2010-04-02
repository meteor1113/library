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
    std::auto_ptr<log::FileAppender> fa(new log::FileAppender);
    fa->SetFilepath("root.log");
    fa->SetLayout("[myproject] {LOG} ({PID}) {LEVEL} %Y-%m-%d");
    fa->SetLevel(log::LOGLEVEL_INFO);
    log::Logger::GetLogger().AddAppender(std::auto_ptr<log::Appender>(fa));

    std::auto_ptr<log::Appender> a(new log::FileAppender("a.log"));
    log::Logger::GetLogger("a").AddAppender(a);
    log::Logger::GetLogger("b").AddAppender(
        std::auto_ptr<log::Appender>(new log::FileAppender("b.log", "", "%Y-%m {LEVEL} {LOG}")));
    log::Logger::GetLogger("c").AddAppender(
        std::auto_ptr<log::Appender>(new log::FileAppender("log/log/log.log", "TRACE")));
    log::Logger::GetLogger("c").AddAppender(
        std::auto_ptr<log::Appender>(new log::ConsoleAppender()));
}


void Test()
{
    log::Logger x1;
    x1.Fatal("test log x1");
    log::Logger x2;
    std::auto_ptr<log::Appender> a(new log::ConsoleAppender);
    x2.AddAppender(a);
    x2.Fatal("test log x2");
    log::Logger::GetLogger("a");
    log::Logger::GetLogger("b");
    log::Logger::GetLogger("c");
    log::Logger::GetLogger("c");
    log::Logger::GetLogger("c");
    log::Logger::GetLogger("a");
    log::Logger& log = log::Logger::GetLogger("a");
    log.Info("aaa");
    log.Warn("bbb");
    log.Error("ccc");
    log.Trace("ddd");
    log::Logger::GetLogger("b").Warn("test b");
    log::Logger::GetLogger("b").Fatal(
        str::Format("test fatal b:%d,%s,0%o,0x%x,%e,%c", 1234,"ok",128,256,3.14,'A'));
    log::Logger::GetLogger("c").Warn("test c");
    log::Logger::GetLogger("c").Trace(str::Format("trace:%d", 1));
    log::Logger::GetLogger("c").Debug(str::Format("debug:%d", 2));
    log::Logger::GetLogger("c").Info(str::Format("info:%d", 3));
    log::Logger::GetLogger("c").Warn(str::Format("warn:%d", 4));
    log::Logger::GetLogger("c").Error(str::Format("error:%d", 5));
    log::Logger::GetLogger("c").Fatal(str::Format("fatal:%d", 6));
    log::Logger::GetLogger("z").Fatal(str::Format("fatal:%d", 9999));
}


void Test1()
{
    log::Logger::GetLogger().Trace("trace");
    log::Logger::GetLogger().Warn("warn");
    log::Logger::GetLogger().Error("error");

    log::Logger::GetLogger().Fatal("fatal");
}


void DailyTestPre()
{
    // Logger::GetLogger().SetFilepath("../log/%Y%m%d%H.log");
    // Logger::GetLogger().SetLevel(LOGLEVEL_INFO);
    // Logger::GetLogger("dz").SetLevel("error");
    // Logger::GetLogger("dy").SetLevel("aabc");
}


void DailyTest()
{
//     Logger dx1;
//     dx1.Fatal("test log dx1");
//     Logger dx2;
//     dx2.Fatal("test log dx2");
//     Logger::GetLogger("da");
//     Logger::GetLogger("b");
//     Logger::GetLogger("c");
//     Logger::GetLogger("c");
//     Logger::GetLogger("c");
//     Logger::GetLogger("da");
//     Logger& log = Logger::GetLogger("da");
//     log.SetFilepath("logger%Y%m%d%H%M%S.log");
//     log.Info("aaa");
//     log.Warn("bbb");
// #ifdef _WIN32
//     Sleep(2000);
// #else
//     sleep(2);
// #endif
//     log.Error("ccc");
//     log.Trace("ddd");

//     log.Fatal("fatal");
//     Logger::GetLogger("dz").Trace("test dz trace");
//     Logger::GetLogger("dz").Debug("test dz debug");
//     Logger::GetLogger("dz").Info("test dz info");
//     Logger::GetLogger("dz").Warn("test dz warn");
//     Logger::GetLogger("dz").Error("test dz error");
//     Logger::GetLogger("dz").Fatal("test dz fatal");
//     Logger::GetLogger("dy").Trace("test dy trace");
//     Logger::GetLogger("dy").Debug("test dy debug");
//     Logger::GetLogger("dy").Info("test dy info");
//     Logger::GetLogger("dy").Warn("test dy warn");
//     Logger::GetLogger("dy").Error("test dy error");
//     Logger::GetLogger("dy").Fatal("test dy fatal");
}


void DailyTest1()
{
    // Logger::GetLogger().Trace("trace");
    // Logger::GetLogger().Warn("warn");
    // Logger::GetLogger().Error("error");
    // Logger::GetLogger().Fatal("fatal");
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
