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


#ifndef UTIL_ENCODE_HPP_
#define UTIL_ENCODE_HPP_


#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
// #include <standard library headers>
#include <string>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
#endif
// #include "customer headers"


namespace util
{

    namespace encode
    {

        // declaration
        std::string ToUtf8(const std::wstring& str);
        std::string ToUtf8(const std::string& str);
        std::wstring Utf8ToWstring(const std::string& strUtf8);
        std::string Utf8ToAscii(const std::string& strUtf8);
        std::wstring ToWstring(const std::string& str);
        std::string ToAscii(const std::wstring& str);


        // implement

        inline
        std::string
        ToUtf8(const std::wstring& str)
        {
#ifdef _WIN32
            int len = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1,
                                          NULL, 0, NULL, NULL);
            char *szUtf8=new char[len + 1];
            memset(szUtf8, 0, len + 1);
            WideCharToMultiByte (CP_UTF8, 0, str.c_str(), -1, szUtf8,
                                 len, NULL,NULL);

            std::string utf8Str = szUtf8;
            delete[] szUtf8;

            return utf8Str;
#else
#error "not support"
#endif
        }


        inline
        std::string
        ToUtf8(const std::string& str)
        {
#ifdef _WIN32
            int len=MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL,0);
            wchar_t* wszUtf8 = new wchar_t[len+1];
            memset(wszUtf8, 0, len * 2 + 2);
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszUtf8, len);

            std::string utf8Str = ToUtf8(wszUtf8);
            delete[] wszUtf8;
            wszUtf8 = NULL;

            return utf8Str;
#else
#error "not support"
#endif
        }


        inline
        std::wstring
        Utf8ToWstring(const std::string& strUtf8)
        {
#ifdef _WIN32
            int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(),
                                        -1, NULL,0);
            wchar_t* wsz = new wchar_t[len+1];
            memset(wsz, 0, len * 2 + 2);
            MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wsz, len);

            std::wstring wstr = wsz;
            delete[] wsz;
            wsz = NULL;

            return wstr;
#else
#error "not support"
#endif
        }


        inline
        std::string
        Utf8ToAscii(const std::string& strUtf8)
        {
#ifdef _WIN32
            std::wstring wStr = Utf8ToWstring(strUtf8);

            int len = WideCharToMultiByte(CP_ACP, 0, wStr.c_str(), -1,
                                          NULL, 0, NULL, NULL);
            char *szAscii=new char[len + 1];
            memset(szAscii, 0, len + 1);
            WideCharToMultiByte (CP_ACP, 0, wStr.c_str(), -1, szAscii,
                                 len, NULL,NULL);

            std::string strAscii = szAscii;
            delete[] szAscii;
            szAscii = NULL;
            //delete[] wszGBK;

            return strAscii;
#else
#error "not support"
#endif
        }


        inline
        std::wstring
        ToWstring(const std::string& str)
        {
#ifdef _WIN32
            int len=MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL,0);
            wchar_t* wszUtf8 = new wchar_t[len+1];
            memset(wszUtf8, 0, len * 2 + 2);
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszUtf8, len);

            std::wstring wstr = wszUtf8;
            delete[] wszUtf8;
            wszUtf8 = NULL;

            return wstr;
#else
#error "not support"
#endif
        }


        inline
        std::string
        ToAscii(const std::wstring& str)
        {
#ifdef _WIN32
            int len = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1,
                                          NULL, 0, NULL, NULL);
            char *szAscii=new char[len + 1];
            memset(szAscii, 0, len + 1);
            WideCharToMultiByte (CP_ACP, 0, str.c_str(), -1, szAscii,
                                 len, NULL,NULL);

            std::string strAscii = szAscii;
            delete[] szAscii;
            szAscii = NULL;

            return strAscii;
#else
#error "not support"
#endif
        }

    } // // end of namespace encode

} // // end of namespace util


#endif // UTIL_ENCODE_HPP_
