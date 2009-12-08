/**
 * Copyright (C) 2009 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-11-26
 */


#ifndef LOGGER_HPP_
#define LOGGER_HPP_


#pragma warning(disable: 4996)
// #include <standard library headers>
#include <time.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// #include <other library headers>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

// #include "customer headers"
#include "str.hpp"


const char* const DEFAULT_LAYOUT = "%Y-%m-%d %H:%M:%S {LEVEL} {LOG}";


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
 */
class Logger
{
public:
    Logger();
    ~Logger() {}

public:
    static Logger& GetLogger(const std::string& name = "");

public:
    /**
     * Set log file path.
     * If empty or can't create directory, will log to console.
     * filepath will be replace by strftime() at runtime.
     * For example:
     *    "/var/log/logger.log", will create /var/log/logger.log file
     *    "/var/log/%Y%m%d.log", will create /var/log/YYYYMMDD.log file
     *    "c:\%Y%m%d%H.log", will create c:\YYYYMMDDHH.log file
     * Default value is empty, so default to log to console.
     *
     * @param value log file path
     */
    void SetFilepath(const std::string& value) { filepath = value; }
    std::string GetFilepath() const { return filepath; }

    /**
     * Set log layout, now only support:
     *     {LEVEL}: log level
     *     {LOG}  : log content
     *     {PID}  : process id
     * Apart from this, you can use strftime' format string.
     * Default value is "%Y-%m-%d %H:%M:%S {LEVEL} {LOG}",
     * user can change it's order and add any other chars.
     *
     * @param value log layout
     */
    void SetLayout(const std::string& value) { layout = value; }
    std::string GetLayout() const { return layout;}

    /**
     * Set log level, Now it support 6 levels:
     *     TRACE < DEBUG < INFO < WARN < ERROR < FATAL.
     * Default value is DEBUG.
     *
     * @param value log level
     */
    void SetLevel(LogLevel value) { level = value; }
    void SetLevel(const std::string& v) { level = GetLevelFromString(v); }
    LogLevel GetLevel() const { return level; }

    void Trace(const std::string& log = "") const { Log(LOGLEVEL_TRACE, log); }
    void Debug(const std::string& log = "") const { Log(LOGLEVEL_DEBUG, log); }
    void Info(const std::string& log = "") const { Log(LOGLEVEL_INFO, log); }
    void Warn(const std::string& log = "") const { Log(LOGLEVEL_WARN, log); }
    void Error(const std::string& log = "") const { Log(LOGLEVEL_ERROR, log); }
    void Fatal(const std::string& log = "") const { Log(LOGLEVEL_FATAL, log); }
    void Log(LogLevel l, const std::string& log = "") const;
    void ForceLog(const std::string& l, const std::string& log = "") const;

protected:

private:
    std::string GetLevelString(LogLevel l) const;
    LogLevel GetLevelFromString(const std::string& v) const;
    std::string GetRealFilepath() const;
    static std::string FormatCurDateTime(const std::string& fmt);
    static int GetPid();

private:
    std::string filepath;
    std::string layout;
    LogLevel level;
};


inline
Logger::Logger()
    : layout(DEFAULT_LAYOUT),
      level(LOGLEVEL_DEBUG)
{
}


inline
Logger& Logger::GetLogger(const std::string& name)
{
    static std::map<std::string, Logger> map;
    return map[name];
}


inline
void Logger::Log(LogLevel l, const std::string& log) const
{
    if (l < level)
    {
        return;
    }

    ForceLog(GetLevelString(l), log);
}


inline
void Logger::ForceLog(const std::string& l, const std::string& log) const
{
    std::string str = FormatCurDateTime(layout);
    str = str::Replace<char>(str, "{LEVEL}", l);
    str = str::Replace<char>(str, "{LOG}", log);
    str = str::Replace<char>(str, "{PID}", str::Format("%d", GetPid()));

    if (filepath.empty())
    {
        std::cout << str << std::endl;
    }
    else
    {
        std::string path = GetRealFilepath();
        std::ofstream file;
        file.open(path.c_str(), std::ios_base::out | std::ios_base::app);
        if (file.is_open())
        {
            file << str << std::endl;
            file.flush();
        }
        else
        {
            std::cout << "(log to file failed)" << str << std::endl;
        }
    }
}


inline
std::string Logger::GetLevelString(LogLevel l) const
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


inline
LogLevel Logger::GetLevelFromString(const std::string& v) const
{
    static std::vector<std::pair<std::string, LogLevel> > vec;
    if (vec.empty())
    {
        vec.push_back(std::make_pair("TRACE", LOGLEVEL_TRACE));
        vec.push_back(std::make_pair("DEBUG", LOGLEVEL_DEBUG));
        vec.push_back(std::make_pair("INFO", LOGLEVEL_INFO));
        vec.push_back(std::make_pair("WARN", LOGLEVEL_WARN));
        vec.push_back(std::make_pair("ERROR", LOGLEVEL_ERROR));
        vec.push_back(std::make_pair("FATAL", LOGLEVEL_FATAL));
    }
    std::vector<std::pair<std::string, LogLevel> >::iterator it;
    for (it = vec.begin(); it != vec.end(); ++it)
    {
        if (str::EqualsIgnoreCase(it->first, str::Trim(v)))
        {
            return it->second;
        }
    }
    return LOGLEVEL_DEBUG;
}


inline
std::string Logger::GetRealFilepath() const
{
    std::string dir = str::DeleteLastPath(filepath);
    if (!dir.empty())
    {
#ifdef _WIN32
        mkdir(dir.c_str());
#else
        mkdir(dir.c_str(), 0777);
#endif
    }

    return FormatCurDateTime(filepath);
}


inline
std::string Logger::FormatCurDateTime(const std::string& fmt)
{
    time_t clock;
    time(&clock);
    char date[512];
    memset(date, 0, 512);
    strftime(date, 512, fmt.c_str(), localtime(&clock));
    return date;
}


inline
int Logger::GetPid()
{
#ifdef _WIN32
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}


#endif
