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


#ifndef CSVFILE_HPP_
#define CSVFILE_HPP_


// #include <standard C library headers>

// #include <standard C++ library headers>
#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <stdexcept>

// #include <other library headers>

// #include "customer headers"


namespace util
{


    class CsvFile
    {
    public:
        CsvFile(const std::string& path = "")
            { Clear(); if (!path.empty()) { Read(path); } }
        ~CsvFile() {}

    private:
        CsvFile(const CsvFile& rhs);
        CsvFile& operator=(const CsvFile& rhs);

    public:
        void Read(const std::string& path);
        void Save() { Save(mFilepath); }
        void Save(const std::string& path);
        void Clear() { mData.clear(); mColCount = 0; mFilepath = ""; }
        int GetRowCount() const { return mData.size(); }
        int GetColumnCount() const { return mColCount; }
        void SetColumnCount(const int col) { mColCount = inCol; }
        const std::string& GetData(int row, int col)
            { return GetRow(row).at(col); }
        const std::vector<std::string>& GetData(int row) { return GetRow(row); }
        void SetData(int row, int col, const std::string& data)
            { GetRow(row).at(col) = data; }
        void SetData(int row, const std::vector<std::string>& data)
            { GetRow(row) = data; GetRow(row); }
        void AddRow(const std::vector<std::string>& data)
            { mData.push_back(data); }

    protected:

    private:
        std::vector<std::string>& GetRow(int row);

    private:
        std::deque<std::vector<std::string> > mData;
        int mColCount;
        std::string mFilepath;
    };


    inline void CsvFile::Read(const std::string& path)
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
            mColCount = std::max(v.size(), mColCount);
        }

        mFilepath = path;
    }

    inline void CsvFile::Save(const std::string& path)
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

    inline std::vector<std::string>& CsvFile::GetRow(int row)
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


#endif // CSVFILE_HPP_
