/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


#ifndef LOGFILE_HPP_
#define LOGFILE_HPP_


#pragma warning(disable: 4996)
// #include <standard library headers>
#include <time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

// #include <other library headers>

// #include "customer headers"
#include "str.hpp"


const char* const DEFAULT_LAYOUT = "{DATE} {LEVEL} {LOG}";
const char* const DEFAULT_DATE_FORMAT = "%Y-%m-%d %H:%M:%S";


enum LogLevel
{
    LOGLEVEL_TRACE,
    LOGLEVEL_DEBUG,
    LOGLEVEL_INFO,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR,
    LOGLEVEL_FATAL
};


/**
 * This is a logging class like log4j, but it's very very simple.
 * It support singleton and non-singleton mode.
 */
class LogFile
{
public:
    LogFile();
    ~LogFile() {}

public:
    static LogFile& Instance() { static LogFile inst; return inst; }

public:
    /**
     * Set log file path. if empty, will write log to console.
     *
     * @param value log file path
     */
    void SetFilepath(const std::string& value) { filepath = value; }

    /**
     * Set log layout, now only support:
     *     {DATE} : log date, see SetDateFormat().
     *     {LEVEL}: log level.
     *     {LOG}  : log content.
     * It's default value is "{DATE} {LEVEL} {LOG}",
     * user can change it's order and add any other chars.
     *
     * @param value log layout
     */
    void SetLayout(const std::string& value) { layout = value; }

    /**
     * Set date format, it use strftime() to format date,
     * so dateFormat must recognize by strftime().
     * It's default value is "%Y-%m-%d %H:%M:%S".
     *
     * @param value date format
     */
    void SetDateFormat(const std::string& value) { dateFormat = value; }

    /**
     * Set log level, Now it support 6 levels:
     *     TRACE < DEBUG < INFO < WARN < ERROR < FATAL.
     *
     * @param value log level
     */
    void SetLevel(LogLevel value) { level = value; }

    void Trace(const std::string& log) const { Log(LOGLEVEL_TRACE, log); }
    void Debug(const std::string& log) const { Log(LOGLEVEL_DEBUG, log); }
    void Info(const std::string& log) const { Log(LOGLEVEL_INFO, log); }
    void Warn(const std::string& log) const { Log(LOGLEVEL_WARN, log); }
    void Error(const std::string& log) const { Log(LOGLEVEL_ERROR, log); }
    void Fatal(const std::string& log) const { Log(LOGLEVEL_FATAL, log); }
    void Log(LogLevel l, const std::string& log) const;
    void ForceLog(const std::string& l, const std::string& log) const;

protected:

private:
    std::string GetLevelString(LogLevel l) const;

private:
    std::string filepath;
    std::string layout;
    std::string dateFormat;
    LogLevel level;
};


inline
LogFile::LogFile()
    : layout(DEFAULT_LAYOUT),
      dateFormat(DEFAULT_DATE_FORMAT),
      level(LOGLEVEL_TRACE)
{
}


inline
void LogFile::Log(LogLevel l, const std::string& log) const
{
    if (l < level)
    {
        return;
    }

    ForceLog(GetLevelString(l), log);
}


inline
void LogFile::ForceLog(const std::string& l, const std::string& log) const
{
    time_t clock;
    time(&clock);
    char date[128];
    memset(date, 0, 128);
    strftime(date, 128, dateFormat.c_str(), localtime(&clock));
    std::string str = str::Replace<char>(layout, "{DATE}", date);
    str = str::Replace<char>(str, "{LEVEL}", l);
    str = str::Replace<char>(str, "{LOG}", log);

    if (filepath.empty())
    {
        std::cout << str << std::endl;
    }
    else
    {
        std::ofstream file;
        file.open(filepath.c_str(), std::ios_base::out | std::ios_base::app);
        if (file.is_open())
        {
            file << str << std::endl;
            file.flush();
        }
        else
        {
            std::cout << "open file " << filepath
                      << " failed, write to console:" << std::endl;
            std::cout << str << std::endl;

        }
    }
}


inline
std::string LogFile::GetLevelString(LogLevel l) const
{
    static std::map<LogLevel, std::string> map;
    if (map.empty())
    {
        map[LOGLEVEL_TRACE] = "TRACE";
        map[LOGLEVEL_DEBUG] = "DEBUG";
        map[LOGLEVEL_INFO] = "INFO ";
        map[LOGLEVEL_WARN] = "WARN ";
        map[LOGLEVEL_ERROR] = "ERROR";
        map[LOGLEVEL_FATAL] = "FATAL";
    }
    return map[l];
}


#endif
