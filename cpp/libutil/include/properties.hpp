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


#ifndef PROPERTIES_HPP_
#define PROPERTIES_HPP_


#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif
// #include <standard library headers>
#include <string>
#include <map>
#include <fstream>

// #include <other library headers>

// #include "customer headers"
#include "str.hpp"


/**
 * in Properties file,
 * #, if a line start with #, means this line is a comment;
 * any valid line must have a "=" to split key and value;
 * param can use {1}, {2}, {3} and {4}.
 */
class Properties
{
    typedef std::map<std::string, std::string> DataMap;

public:
    Properties(const std::string& filename = "") { Load(filename); }
    ~Properties() {}

private:
    Properties(const Properties& rhs);
    Properties& operator=(const Properties& rhs);

public:
    void Clear() { filename_ = ""; values_.clear(); }
    void Load(const std::string& filename);
    void Save(const std::string& filename = "");
    std::string Get(const std::string& key,
                    const char* param1 = NULL,
                    const char* param2 = NULL,
                    const char* param3 = NULL,
                    const char* param4 = NULL);
    void Set(const std::string& key, const std::string& value)
        { values_[key] = value; }

protected:

private:
    std::string filename_;
    DataMap values_;
};


inline void Properties::Load(const std::string& filename)
{
    Clear();
    filename_ = filename;
    if (filename_.empty())
    {
        return;
    }

    std::ifstream fs(filename_.c_str());
    while (fs)
    {
        std::string line;
        std::getline(fs, line);
        if (line.length() > 0 && line[0] != '#')
        {
            std::string::size_type pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value =
                    line.substr(pos + 1, line.length() - pos);
                Set(str::Trim<char>(key), str::Trim<char>(value));
            }
        }
    }
    fs.close();
}


inline void Properties::Save(const std::string& filename)
{
    std::string name = filename.empty() ? filename_ : filename;

    std::ofstream fs(name.c_str(),
                     std::ios_base::out | std::ios_base::trunc);
    for (DataMap::iterator it = values_.begin();
         it != values_.end(); ++it)
    {
        fs << it->first << "=" << it->second << std::endl;
    }
    fs.close();
}


/**
 * if not found, return key.
 */
inline std::string Properties::Get(const std::string& key,
                                   const char* param1,
                                   const char* param2,
                                   const char* param3,
                                   const char* param4)
{
    std::string value = key;
    DataMap::iterator it = values_.find(key);
    if (it != values_.end())
    {
        value = it->second;
    }

    if (param1 != NULL)
    {
        value = str::Replace<char>(value, "{1}", param1);
    }
    if (param2 != NULL)
    {
        value = str::Replace<char>(value, "{2}", param2);
    }
    if (param3 != NULL)
    {
        value = str::Replace<char>(value, "{3}", param3);
    }
    if (param4 != NULL)
    {
        value = str::Replace<char>(value, "{4}", param4);
    }

    return value;
}


#endif
