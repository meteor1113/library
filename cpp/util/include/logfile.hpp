/**
 * Copyright (C) 2008 Meteor Liu
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


// #include <standard library headers>
#include <string>
#include <fstream>
#include <ctime>

// #include <other library headers>

// #include "customer headers"


namespace util
{

    const char* const DEFAULT_TIME_FORMAT = "%Y-%m-%d %H:%M:%S";


    /**
     * description
     */
    class LogFile
    {
    public:
        LogFile(const std::string& filename,
                const std::string& tf = DEFAULT_TIME_FORMAT)
            : file(filename.c_str(),
                   std::ios_base::out | std::ios_base::app), timeFormat(tf) {}
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
        std::time_t clock;
        std::time(&clock);
#pragma warning(push)
#pragma warning(disable: 4996)
        char buf[128];
        memset(buf, 0, 128);
        std::strftime(buf, 128, timeFormat.c_str(),
                      std::localtime(&clock));
#pragma warning(pop)
        std::string str = std::string(buf) + "    " + log;
        file << str << std::endl;
    }

}

#endif
