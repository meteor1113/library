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


#include "csvfile.hpp"
// #include <standard C library headers>

// #include <standard C++ library headers>
#include <fstream>
#include <stdexcept>
#include <iostream>

// #include <other library headers>

// #include "customer headers"
#include "string.h"


namespace util
{

    CsvFile::CsvFile(const std::string& path)
    {
        Clear();
        if (!path.empty())
        {
            Read(path);
        }
    }


    CsvFile::~CsvFile()
    {
    }


    void CsvFile::Read(const std::string& path)
    {
        std::ifstream file(path.c_str());
        if (!file)
        {
            throw std::domain_error("file not found!");
        }
        Clear();

        std::string line;
        while (std::getline(file, line))
        {
            std::vector<std::string> v = util::string::Split(line.c_str(), ",");
            mData.push_back(v);
            mColCount = std::max(static_cast<unsigned int>(v.size()),
                                 mColCount);
        }

        mFilepath = path;
    }


    void CsvFile::Save()
    {
        Save(mFilepath);
    }


    void CsvFile::Save(const std::string& path)
    {
        std::ofstream file(path.c_str());
        if (!file)
        {
            throw std::domain_error("open file failed!");
        }

        for (unsigned int i = 0; i < mData.size(); ++i)
        {
            std::vector<std::string>& line = GetRow(i);
            std::string str = util::string::Join(line.begin(), line.end(), ",");
            file << str << std::endl;
        }
    }


    void CsvFile::Clear()
    {
        mData.clear();
        mColCount = 0;
        mFilepath = "";
    }


    int CsvFile::GetRowCount() const
    {
        return mData.size();
    }


    int CsvFile::GetColumnCount() const
    {
        return mColCount;
    }


    void CsvFile::SetColumnCount(unsigned int inCol)
    {
        mColCount = inCol;
    }


    const std::string& CsvFile::GetData(unsigned int row, unsigned int col)
    {
        std::vector<std::string>& line = GetRow(row);
        return line.at(col);
    }


    const std::vector<std::string>& CsvFile::GetData(unsigned int row)
    {
        return GetRow(row);
    }


    void CsvFile::SetData(unsigned int row, unsigned int col,
                          const std::string& data)
    {
        std::vector<std::string>& line = GetRow(row);
        line.at(col) == data;
    }


    void CsvFile::SetData(unsigned int row,
                          const std::vector<std::string>& data)
    {
        std::vector<std::string>& line = GetRow(row);
        line = data;
        GetRow(row);
    }


    void CsvFile::AddRow(const std::vector<std::string>& data)
    {
        mData.push_back(data);
    }


    std::vector<std::string>& CsvFile::GetRow(unsigned int row)
    {
        std::vector<std::string>& line = mData.at(row);
        while (line.size() < mColCount)
        {
            line.push_back("");
        }
        while (line.size() > mColCount)
        {
            line.pop_back();
        }

        assert(line.size() == mColCount);
        return line;
    }

}
