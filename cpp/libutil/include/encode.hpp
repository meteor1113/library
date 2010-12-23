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


#ifndef ENCODE_HPP_
#define ENCODE_HPP_


#ifdef _MSC_VER
#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
#endif
// #include <standard library headers>
#include <string>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
#endif

// #include "customer headers"


namespace encode
{


inline
std::string
ToUtf8(const wchar_t* str)
{
#ifdef _WIN32
    int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, str, -1, buf, len, NULL, NULL);
    std::string ret = buf;
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


inline
std::string
ToUtf8(const char* str)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wchar_t* buf = new wchar_t[len + 1];
    memset(buf, 0, (len + 1)* sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
    std::string ret = ToUtf8(buf);
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


inline
std::wstring
Utf8ToWstring(const char* str)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    wchar_t* buf = new wchar_t[len + 1];
    memset(buf, 0, (len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, len);
    std::wstring ret = buf;
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


inline
std::string
Utf8ToAscii(const char* str)
{
#ifdef _WIN32
    const std::wstring wstr = Utf8ToWstring(str);
    const wchar_t* tmp = wstr.c_str();
    int len = WideCharToMultiByte(CP_ACP, 0, tmp, -1, NULL, 0, NULL, NULL);
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, tmp, -1, buf, len, NULL, NULL);
    std::string ret = buf;
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


inline
std::wstring
ToWstring(const char* str)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wchar_t* buf = new wchar_t[len+1];
    memset(buf, 0, (len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, -1, buf, len);
    std::wstring ret = buf;
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


inline
std::string
ToAscii(const wchar_t* str)
{
#ifdef _WIN32
    int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, str, -1, buf, len, NULL, NULL);
    std::string ret = buf;
    delete[] buf;
    return ret;
#else
#error "not support"
#endif
}


} // // end of namespace encode


#endif // ENCODE_HPP_
