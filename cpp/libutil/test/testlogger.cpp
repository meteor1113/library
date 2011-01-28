#pragma warning(disable: 4786)
#include <assert.h>

#include <iostream>

// #ifdef _WIN32
// #include <windows.h>
// #else
// #endif

#include "logger.hpp"

using namespace util;

void TestPre()
{
    Logger::GetLogger().SetLevel(LOGLEVEL_INFO);
    FileAppender* fa = new FileAppender;
#ifdef _WIN32
    fa->SetFilepath("z:\\root.log");
#else
    fa->SetFilepath("/root.log");
#endif
    fa->SetLayout("[myproject] {LOG} ({PID}) {LEVEL} %Y-%m-%d");
    Logger::GetLogger().AddAppender(std::auto_ptr<Appender>(fa));

    std::auto_ptr<Appender> a(new FileAppender("a.log"));
    Logger::GetLogger("a").AddAppender(a);

    Logger::GetLogger("b").AddAppender(
        std::auto_ptr<Appender>(new FileAppender("b.log", "%Y-%m {LEVEL} {LOG}")));

    Logger::GetLogger("c").AddAppender(
        std::auto_ptr<Appender>(new FileAppender("log/log/log.log")));
    Logger::GetLogger("c").AddAppender(
        std::auto_ptr<Appender>(new ConsoleAppender()));
}


void Test()
{
    Logger::GetLogger("a");
    Logger::GetLogger("b");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("a");
    Logger& log = Logger::GetLogger("a");
    log.Info("aaa");
    log.Warn("bbb");
    log.Error("ccc");
    log.Trace("ddd");
    Logger::GetLogger("b").Warn("test b");
    Logger::GetLogger("b").Fatal(
        Format("test fatal b:%d,%s,0%o,0x%x,%e,%c", 1234,"ok",128,256,3.14,'A'));
    Logger::GetLogger("c").Warn("test c");
    Logger::GetLogger("c").Trace(Format("trace:%d", 1));
    Logger::GetLogger("c").Debug(Format("debug:%d", 2));
    Logger::GetLogger("c").Info(Format("info:%d", 3));
    Logger::GetLogger("c").Warn(Format("warn:%d", 4));
    Logger::GetLogger("c").Error(Format("error:%d", 5));
    Logger::GetLogger("c").Fatal(Format("fatal:%d", 6));
    Logger::GetLogger("z").Fatal(Format("fatal:%d", 9999));
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
    FileAppender* fa = new FileAppender();
#ifdef _WIN32
    fa->SetFilepath("f:/log/log/%Y/%m/%d/%H/%M/%S.log");
#else
    fa->SetFilepath("~/log/log/%Y/%m/%d/%H/%M/%S.log");
#endif
    Logger::GetLogger("da").SetLevel(LOGLEVEL_DEBUG);
    Logger::GetLogger("da").AddAppender(std::auto_ptr<Appender>(fa));

    Logger::GetLogger("dz").SetLevel(LOGLEVEL_ERROR);
    Logger::GetLogger("dz").AddAppender(std::auto_ptr<Appender>(new ConsoleAppender()));
}


void DailyTest()
{
    Logger::GetLogger("da");
    Logger::GetLogger("b");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("c");
    Logger::GetLogger("da");
    Logger& log = Logger::GetLogger("da");
    log.AddAppender(std::auto_ptr<Appender>(new FileAppender("logger%Y%m%d%H%M%S.log")));
    log.Info("aaa");
    log.Warn("bbb");
#ifdef _WIN32
    Sleep(1000);
#else
    sleep(1);
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
    Logger::GetLogger("da").Trace("trace");
    Logger::GetLogger("da").Warn("warn");
    Logger::GetLogger("da").Error("error");
    Logger::GetLogger("da").Fatal("fatal");
}


int main()
{
    std::cout << "timezone:" << GetTimezone() << std::endl;

    std::cout << "======test Logger begin" << std::endl;
    TestPre();
    Test();
    Test1();
    std::cout << "======test Logger end" << std::endl;
    std::cout << std::endl;

    std::cout << "======test DailyLogger begin" << std::endl;
    DailyTestPre();
    DailyTest();
    DailyTest1();
    std::cout << "======test DailyLogger end" << std::endl;

    return 0;
}
