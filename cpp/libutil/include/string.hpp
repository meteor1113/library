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


#ifndef STRING_HPP_
#define STRING_HPP_


#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
// #include <standard library headers>
#include <assert.h>
#include <stdarg.h>
#include <string>
#include <locale>
#include <algorithm>
#include <vector>
#include <stdexcept>

// #include <other library headers>

// #include "customer headers"


namespace string
{

    template <typename T>
    bool
    StartWith(const T* str, const T* sub)
    {
        assert(str != NULL);
        assert(sub != NULL);

        const std::basic_string<T> s = str;
        return (s.find(sub) == 0);
    }


    template <typename T>
    bool
    EndWith(const T* str, const T* sub)
    {
        assert(str != NULL);
        assert(sub != NULL);

        const std::basic_string<T> s = str;
        const std::basic_string<T> d = sub;
        if (s.length() < d.length())
        {
            return false;
        }
        return (s.rfind(d) == (s.length() - d.length()));
    }


    template <typename T>
    std::basic_string<T>
    ToUpper(const T* str)
    {
        assert(str != NULL);

        std::basic_string<T> s = str;
        std::locale loc;
#if _MSC_VER < 1400 // < vc8
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
#else
        const std::ctype<T>& ct = std::use_facet<std::ctype<T> >(loc);
        ct.toupper(&s.at(0), &s.at(s.length() - 1) + 1);
#endif
        return s;
    }


    template <typename T>
    std::basic_string<T>
    ToLower(const T* str)
    {
        assert(str != NULL);

        std::basic_string<T> s = str;
        std::locale loc;
#if _MSC_VER < 1400 // < vc8
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
#else
        const std::ctype<T>& ct = std::use_facet<std::ctype<T> >(loc);
        ct.tolower(&s.at(0), &s.at(s.length() - 1) + 1);
#endif
        return s;
    }


    template <typename T>
    bool
    EqualsIgnoreCase(const T* s1, const T* s2)
    {
        assert(s1 != NULL);
        assert(s2 != NULL);

        return (ToLower(s1) == ToLower(s2));
    }


    template <typename T>
    std::basic_string<T>
    Replace(const T* str, const T* o, const T* n)
    {
        assert(str != NULL);
        assert(o != NULL);
        assert(n != NULL);

        std::basic_string<T> s = str;
        if (std::string::npos == s.find(o))
        {
            return s;
        }

        const std::basic_string<T> oldS = o;
        const std::basic_string<T> newS = n;
        const std::string::size_type oldLen = oldS.size();
        const std::string::size_type newLen = newS.size();
        std::string::size_type beg = 0;
        std::string::size_type tmp = 0;
        while ((beg = (s.find(oldS, tmp))) != std::string::npos)
        {
            s.replace(beg, oldLen, newS);
            tmp = beg + newLen;
            if (tmp >= s.length())
            {
                break;
            }
        }
        return s;
    }


    template <typename T>
    std::basic_string<T>
    TrimLeft(const T* str)
    {
        assert(str != NULL);

        std::basic_string<T> s = str;
        const std::locale loc;
        typename std::basic_string<T>::iterator it = s.begin();
        while (it != s.end() && std::isspace(*it, loc))
        {
            ++it;
        }
        return std::basic_string<T>(it, s.end());
    }


    template <typename T>
    std::basic_string<T>
    TrimRight(const T* str)
    {
        assert(str != NULL);

        std::basic_string<T> s = str;
        const std::locale loc;
        typename std::basic_string<T>::reverse_iterator it = s.rbegin();
        while (it != s.rend() && std::isspace(*it, loc))
        {
            ++it;
        }
        return std::basic_string<T>(s.begin(), it.base());
    }


    template <typename T>
    std::basic_string<T>
    Trim(const T* str)
    {
        assert(str != NULL);

        std::basic_string<T> s = TrimLeft(str);
        return TrimRight(s.c_str());
    }


    template <typename InIt, typename T>
    std::basic_string<T>
    Join(InIt first, InIt last, const T* ep)
    {
        assert(ep != NULL);

        std::basic_string<T> s;
        const std::basic_string<T> sep = ep;
        for (InIt it = first; it != last; ++it)
        {
            s+= *it;
            s+= sep;
        }
        const typename std::basic_string<T>::size_type sepLen = sep.size();
        if (s.size() >= sepLen)
        {
            s.erase(s.length() - sepLen, sepLen);
        }
        return s;
    }


    template <typename T>
    std::vector<std::basic_string<T> >
    Split(const T* str, const T* ep)
    {
        std::basic_string<T> s = str;
        std::vector<std::basic_string<T> > vs;
        const std::basic_string<T> sep = ep;

        if (s.empty())
        {
            return vs;
        }
        if (sep.empty() || std::string::npos == s.find(sep))
        {
            vs.push_back(s);
            return vs;
        }

        const typename std::basic_string<T>::size_type sepLen = sep.size();
        typename std::basic_string<T>::size_type beg = 0;
        typename std::basic_string<T>::size_type e = std::string::npos;

        while (std::basic_string<T>::npos != (e = s.find(sep, beg)))
        {
            vs.push_back(s.substr(beg, e - beg));
            beg = e + sepLen;
        }
        vs.push_back(s.substr(beg, s.size() - beg));

        return vs;
    }


    inline
    int
    Vsnprintf(char* buf, size_t count, const char* fmt, va_list ap)
    {
        assert(buf != NULL);
        assert(fmt != NULL);

#ifdef _WIN32
#if _MSC_VER < 1400 // < vc8
        return _vsnprintf(buf, count, fmt, ap);
#else
        return vsnprintf(buf, count, fmt, ap);
#endif
#else
        return vsnprintf(buf, count, fmt, ap);
#endif
    }


    inline
    int
    Vsnprintf(wchar_t* buf, size_t count, const wchar_t* fmt, va_list ap)
    {
        assert(buf != NULL);
        assert(fmt != NULL);

#ifdef _WIN32
        return _vsnwprintf(buf, count, fmt, ap);
#else
        return vswprintf(buf, count, fmt, ap);
#endif
    }


    template <int BUFSIZE, typename T>
    std::basic_string<T>
    Format(const T* fmt, ...)
    {
        assert(fmt != NULL);

        T buf[BUFSIZE] = {0};
        va_list ap;
        va_start(ap, fmt);
        int count = Vsnprintf(buf, BUFSIZE, fmt, ap);
        va_end(ap);
        if (count == -1)
        {
            char b[100] = {0};
            sprintf(b, "bufsize[%d] is less", BUFSIZE);
            throw std::length_error(b);
        }
        return buf;
    }


    template <typename T>
    std::basic_string<T>
    Format(const T* fmt, ...)
    {
        assert(fmt != NULL);

        static const int MAX_LINE_LEN = 1024 * 8; // default buffer size
        static T buf[MAX_LINE_LEN] = {0};
        buf[0] = 0;
        va_list ap;
        va_start(ap, fmt);
        int count = Vsnprintf(buf, MAX_LINE_LEN, fmt, ap);
        va_end(ap);
        if (count == -1)
        {
            char b[100] = {0};
            sprintf(b, "bufsize[%d] is less", MAX_LINE_LEN);
            throw std::length_error(b);
        }
        return buf;
    }


    /**
     * if sep is "/" and comp is "scratch.tiff":
     * "/tmp"              -> "/tmp/scratch.tiff"
     * "/tmp/"             -> "/tmp/scratch.tiff"
     * "/"                 -> "/scratch.tiff"
     * ""(an empty string) -> "scratch.tiff"
     * "tmp"               -> "tmp/scratch.tiff"
     *
     * if sep is "\" and comp is "scratch.tiff":
     * "\tmp"              -> "\tmp\scratch.tiff"
     * "\tmp\"             -> "\tmp\scratch.tiff"
     * "\"                 -> "\scratch.tiff"
     * ""(an empty string) -> "scratch.tiff"
     * "tmp"               -> "tmp\scratch.tiff"
     * "c:\aaa\bbb"        -> "c:\aaa\bbb\scratch.tiff"
     * "c:\"               -> "c:\scratch.tiff"
     * "c:"                -> "c:\scratch.tiff"
     */
    template <typename T>
    std::basic_string<T>
    AppendPathComponent(const T* str, const T* comp, const T* sep)
    {
        assert(str != NULL);
        assert(comp != NULL);
        assert(sep != NULL);

        std::basic_string<T> s = str;
        if (s.empty())
        {
            return comp;
        }
        if (!EndWith(str, sep))
        {
            s.append(sep);
        }
        s.append(comp);
        return s;
    }


    /**
     * if sep is "/":
     * ¡°/tmp/scratch.tiff¡± -> "/tmp¡±
     * "/tmp/lock/¡±        -> "/tmp"
     * "/tmp/"             -> "/"
     * "/tmp"              -> "/"
     * "/"                 -> "/"
     * "scratch.tiff"      -> ""(an empty string)
     *
     * is sep is "\"
     * "\tmp\scratch.tiff" -> "\tmp"
     * "\tmp\lock\"        -> "\tmp"
     * "\tmp\"             -> "\"
     * "\tmp"              -> "\"
     * "\"                 -> "\"
     * "scratch.tiff"      -> ""(an empty string)
     * "c:\aaa\bbb"        -> "c:\aaa"
     * "c:\"               -> ""(an empty string)
     * "c:"                -> ""(an empty string)
     */
    template <typename T>
    std::basic_string<T>
    DeleteLastPathComponent(const T* str, const T* sep)
    {
        assert(str != NULL);
        assert(sep != NULL);

        std::basic_string<T> s = str;
        const std::string::size_type len = std::basic_string<T>(sep).length();
        while (EndWith(s.c_str(), sep))
        {
            s.resize(s.length() - len);
        }
        std::string::size_type pos = s.rfind(sep);
        if (pos == std::string::npos)
        {
            s = std::basic_string<T>();
        }
        else
        {
            s.resize(pos);
        }
        if (StartWith(str, sep) && s.empty())
        {
            s = sep;
        }
        return s;
    }

}


#endif
