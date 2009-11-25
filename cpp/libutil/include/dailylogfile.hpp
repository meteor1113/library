/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-11-25
 */


#ifndef DAILYLOGFILE_HPP_
#define DAILYLOGFILE_HPP_


// #include <standard C library headers>

// #include <standard C++ library headers>
#include <string>

// #include <other library headers>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

// #include "custom headers"
#include "logfile.hpp"
#include "str.hpp"


const char* const DEFAULT_FILENAME_PATTERN = "%Y%m%d.log";


/**
 * This is a daily logging class.
 * It support singleton and non-singleton mode.
 */
class DailyLogFile
{
public:
    DailyLogFile() : filenamePattern(DEFAULT_FILENAME_PATTERN) {}
    /*virtual*/ ~DailyLogFile() {}

private:
    DailyLogFile(const DailyLogFile& rhs);
    DailyLogFile& operator=(const DailyLogFile& rhs);

public:
    static DailyLogFile& Instance() { static DailyLogFile inst; return inst; }

public:
    /**
     * Set log dir, logfile will put in here,
     * if empty, put log in current dir.
     *
     * @param v log dir
     */
    void SetLogdir(const std::string& v) { logdir = v; }

    /**
     * Set filename pattern. it use strftime() to format,
     * so filenamePattern must recognize by strftime().
     * It's default value is "%Y%m%d.log", will create YYYYMMDD.log file,
     * and if set to "MyLog%Y-%m-%d.txt", will create MyLogYYYY-MM-DD.txt file.
     * If empty, will write log to console;
     *
     * @param v filename pattern
     */
    void SetFilenamePattern(const std::string& v) { filenamePattern = v; }

    /**
     * Set log layout, now only support:
     *     {DATE} : log date, see SetDateFormat().
     *     {LEVEL}: log level.
     *     {LOG}  : log content.
     * It's default value is "{DATE} {LEVEL} {LOG}",
     * user can change it's order and add any other chars.
     *
     * @param v log layout
     */
    void SetLayout(const std::string& v) { logFile.SetLayout(v); }

    /**
     * Set date format, it use strftime() to format date,
     * so dateFormat must recognize by strftime().
     * It's default value is "%Y-%m-%d %H:%M:%S".
     *
     * @param v date format
     */
    void SetDateFormat(const std::string& v) { logFile.SetDateFormat(v); }

    /**
     * Set log level, Now it support 6 levels:
     *     TRACE < DEBUG < INFO < WARN < ERROR < FATAL.
     *
     * @param v log level
     */
    void SetLevel(LogLevel v) { logFile.SetLevel(v); }

    void Trace(const std::string& log) { Log(LOGLEVEL_TRACE, log); }
    void Debug(const std::string& log) { Log(LOGLEVEL_DEBUG, log); }
    void Info(const std::string& log) { Log(LOGLEVEL_INFO, log); }
    void Warn(const std::string& log) { Log(LOGLEVEL_WARN, log); }
    void Error(const std::string& log) { Log(LOGLEVEL_ERROR, log); }
    void Fatal(const std::string& log) { Log(LOGLEVEL_FATAL, log); }
    void Log(LogLevel l, const std::string& log);
    void ForceLog(const std::string& l, const std::string& log);

protected:

private:
    std::string GetFilepath() const;

private:
    LogFile logFile;
    std::string logdir;
    std::string filenamePattern;
};


inline
void DailyLogFile::Log(LogLevel l, const std::string& log)
{
    logFile.SetFilepath(GetFilepath());
    logFile.Log(l, log);
}


inline
void DailyLogFile::ForceLog(const std::string& l, const std::string& log)
{
    logFile.SetFilepath(GetFilepath());
    logFile.ForceLog(l, log);
}


inline
std::string DailyLogFile::GetFilepath() const
{
    if (filenamePattern.empty())
    {
        return std::string();
    }

    if (!logdir.empty())
    {
#ifdef _WIN32
        mkdir(logdir.c_str());
#else
        mkdir(logdir.c_str(), 0777);
#endif
    }
    time_t clock;
    time(&clock);
    char name[128];
    memset(name, 0, 128);
    strftime(name, 128, filenamePattern.c_str(), localtime(&clock));
    return str::AppendPath(logdir, name);
}


#endif // DAILYLOGFILE_HPP_
