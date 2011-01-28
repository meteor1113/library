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
#include <errno.h>
#include <string>

// #include <other library headers>
#ifdef _WIN32
#include <Windows.h>
#define CP_GBK 936
#else
#include <iconv.h>
#endif

// #include "customer headers"


namespace util
{


class bad_convert : public std::exception
{
public:
    bad_convert() : _descript("convert error: Unexpected.") {}
    bad_convert(char const* descript) : _descript(descript) {}
    bad_convert(std::string const& descript) : _descript(descript) {}
    virtual ~bad_convert() throw() {}

public:
    virtual const char* what() const throw() { return _descript.c_str(); }

private:
    const std::string _descript;
};


#ifndef _WIN32
std::string Convert(const std::string& src,
                    const std::string& tocode,
                    const std::string& fromcode)
{
#if _LIBICONV_VERSION >= 0x0109
    const char* ps = src.c_str();
#else
    char* ps = const_cast<char*>(src.c_str());
#endif

    std::string result;
    std::string::size_type len1 = src.length();
    do
    {
        char buff[1024] = "";
        char* pd = buff;
        size_t len2 = sizeof(buff);

        iconv_t cd=iconv_open(tocode.c_str(), fromcode.c_str());
        if (cd == (iconv_t)-1)
        {
            std::string s;
            if (errno == EINVAL)
            {
                s = "iconv_open error(EINVAL): fromcode to tocode not support.";
            }
            else
            {
                s = "iconv_open error: Unexpected.";
            }
            throw bad_convert(s);
        }

        int r = iconv(cd, &ps, &len1, &pd, &len2);
        iconv_close(cd);

        if ((r == -1) && (E2BIG != errno))
        {
            std::string s;
            if (errno == EILSEQ)
            {
                s = "iconv error(EILSEQ): An invalid multibyte sequence has been encountered in the input.";
            }
            else if (errno == EINVAL)
            {
                s = "iconv error(EINVAL): An incomplete multibyte sequence has been encountered in the input.";
            }
            else
            {
                s = "iconv error: Unexpected.";
            }
            throw bad_convert(s);
        }

        int count = sizeof(buff) - len2;
        result.append(buff, buff + count);
    }
    while (len1 > 0);

    return result;
}
#endif


inline
std::string
WideToUtf8(const wchar_t* str)
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
    std::string input((const char*)str, sizeof(wchar_t) * wcslen(str));
    return Convert(input, "UTF-8", "WCHAR_T");
#endif
}


inline
std::string
GbkToUtf8(const char* str)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_GBK, 0, str, -1, NULL, 0);
    wchar_t* buf = new wchar_t[len + 1];
    memset(buf, 0, (len + 1)* sizeof(wchar_t));
    MultiByteToWideChar(CP_GBK, 0, str, -1, buf, len);
    std::string ret = WideToUtf8(buf);
    delete[] buf;
    return ret;
#else
    return Convert(str, "UTF-8", "GBK");
#endif
}


inline
std::wstring
Utf8ToWide(const char* str)
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
    std::string wstr = Convert(str, "WCHAR_T", "UTF-8");
    std::wstring result((const wchar_t*)wstr.data(),
                        wstr.length() / sizeof(wchar_t));
    return result;
#endif
}


inline
std::string
Utf8ToGbk(const char* str)
{
#ifdef _WIN32
    const std::wstring wstr = Utf8ToWide(str);
    const wchar_t* tmp = wstr.c_str();
    int len = WideCharToMultiByte(CP_GBK, 0, tmp, -1, NULL, 0, NULL, NULL);
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    WideCharToMultiByte(CP_GBK, 0, tmp, -1, buf, len, NULL, NULL);
    std::string ret = buf;
    delete[] buf;
    return ret;
#else
    return Convert(str, "GBK", "UTF-8");
#endif
}


inline
std::wstring
GbkToWide(const char* str)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(CP_GBK, 0, str, -1, NULL, 0);
    wchar_t* buf = new wchar_t[len+1];
    memset(buf, 0, (len + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_GBK, 0, str, -1, buf, len);
    std::wstring ret = buf;
    delete[] buf;
    return ret;
#else
    std::string wstr = Convert(str, "WCHAR_T", "GBK");
    std::wstring result((const wchar_t*)wstr.data(),
                        wstr.length() / sizeof(wchar_t));
    return result;
#endif
}


inline
std::string
WideToGbk(const wchar_t* str)
{
#ifdef _WIN32
    int len = WideCharToMultiByte(CP_GBK, 0, str, -1, NULL, 0, NULL, NULL);
    char* buf = new char[len + 1];
    memset(buf, 0, len + 1);
    WideCharToMultiByte(CP_GBK, 0, str, -1, buf, len, NULL, NULL);
    std::string ret = buf;
    delete[] buf;
    return ret;
#else
    std::string input((const char*)str, sizeof(wchar_t) * wcslen(str));
    return Convert(input, "GBK", "WCHAR_T");
#endif
}


} // end of namespace


#endif // ENCODE_HPP_
