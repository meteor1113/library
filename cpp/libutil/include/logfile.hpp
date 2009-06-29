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
#include <fstream>

// #include <other library headers>

// #include "customer headers"


const char* const DEFAULT_TIME_FORMAT = "%Y-%m-%d %H:%M:%S";


/**
 * description
 */
class LogFile
{
public:
    LogFile(const std::string& filename,
            const std::string& tf = DEFAULT_TIME_FORMAT)
        : file(filename.c_str(), std::ios_base::out | std::ios_base::app),
          timeFormat(tf) {}
    ~LogFile() { file.flush(); }

public:
    void Log(const std::string& log);

protected:

private:
    std::ofstream file;
    std::string timeFormat;
};

inline void LogFile::Log(const std::string& log)
{
    time_t clock;
    time(&clock);
    char buf[128];
    memset(buf, 0, 128);
    strftime(buf, 128, timeFormat.c_str(), localtime(&clock));
    std::string str = std::string(buf) + " " + log;
    file << str << std::endl;
}


#endif
