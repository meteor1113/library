/* -*- mode: C++; -*- */
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


#ifndef EXCELOPERATE_H
#define EXCELOPERATE_H


// #include <standard library headers>
#include <string>
#include <list>

// #include <other library headers>
#include <ole2.h>

// #include "customer headers"


namespace msoffice
{

    /**
     * description
     */
    class ExcelException : public std::exception
    {
    public:
        ExcelException(const std::string& s = "") : msg(s) {}
        virtual ~ExcelException() throw() {}

    public:
        virtual const char* what() const throw() { return msg.c_str(); }
        virtual const std::string What() const throw() { return msg; }

    private:
        std::string msg;

    };


    /**
     * description
     */
    class ExcelOperate
    {
    public:
        ExcelOperate(const std::wstring& filename = L"", bool visible = false);
        virtual ~ExcelOperate();

    public:
        void SetData(const std::wstring& cell,
                     const std::wstring& data);     // set a range
        void SetData(int nRow, int nBeg,
                     const std::list<std::wstring>& data); // set a line
        void SetData(const std::wstring& colTag, int nBeg,
                     const std::list<std::wstring>& data); // set a column
        void SaveAsFile(const std::wstring& filename);

    private:
        void CreateNewWorkbook();
        void OpenWorkbook(const std::wstring& filename);
        void GetActiveSheet();
        std::wstring GetColumnName(int nIndex);
        bool CheckFilename(const std::wstring& filename);
        void InstanceExcelWorkbook(int visible);

        static void ExcelOperate::AutoWrap(int autoType, VARIANT *pvResult,
                                           IDispatch *pDisp, LPOLESTR ptName,
                                           int cArgs...);

    private:
        IDispatch* pXlApp;
        IDispatch* pXlBooks;
        IDispatch* pXlBook;
        IDispatch* pXlSheet;

    };

}

#endif
