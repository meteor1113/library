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

// #include <other library headers>

// #include "customer headers"


namespace util
{

/**
 * description
 */
    class CsvFile
    {
    public:
        CsvFile(const std::string& path = "");
        ~CsvFile();

    private:
        CsvFile(const CsvFile& rhs);
        CsvFile& operator=(const CsvFile& rhs);

    public:
        void Read(const std::string& path);
        void Save();
        void Save(const std::string& path);
        void Clear();
        int GetRowCount() const;
        int GetColumnCount() const;
        void SetColumnCount(const unsigned int col);
        const std::string& GetData(unsigned int row, unsigned int col);
        const std::vector<std::string>& GetData(unsigned int row);
        void SetData(unsigned int row, unsigned int col,
                     const std::string& data);
        void SetData(unsigned int row, const std::vector<std::string>& data);
        void AddRow(const std::vector<std::string>& data);

    protected:

    private:
        std::vector<std::string>& GetRow(unsigned int row);

    private:
        std::deque<std::vector<std::string> > mData;
        unsigned int mColCount;
        std::string mFilepath;
    };

}

#endif // CSVFILE_HPP_
