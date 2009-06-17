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


#ifndef UTIL_STRING_HPP_
#define UTIL_STRING_HPP_


#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
// #include <standard library headers>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <locale>
#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>
#include <functional>
#include <stdexcept>

// #include <other library headers>

// #include "customer headers"


namespace util
{

    namespace string
    {

        // declaration

        template <typename CharType>
        bool
        StartWith(const CharType* const str, const CharType* const sub);

        template <typename CharType>
        bool
        EndWith(const CharType* const str, const CharType* const substr);

        template <typename CharType>
        std::basic_string<CharType>
        ToUpper(const CharType* const str);

        template <typename CharType>
        std::basic_string<CharType>
        ToLower(const CharType* const str);

        template <typename CharType>
        bool
        EqualsIgnoreCase(const CharType* const s1, const CharType* const s2);

        template <typename CharType>
        std::basic_string<CharType>
        Replace(const CharType* const str, const CharType* const o,
                const CharType* const n);

        template <typename CharType>
        std::basic_string<CharType>
        TrimLeft(const CharType* str);

        template <typename CharType>
        std::basic_string<CharType>
        TrimRight(const CharType* str);

        template <typename CharType>
        std::basic_string<CharType>
        Trim(const CharType* str);

        template <typename InIt, typename CharType>
        std::basic_string<CharType>
        Join(InIt first, InIt last, const CharType* const ep);

        template <typename CharType>
        std::vector<std::basic_string<CharType> >
        Split(const CharType* const str, const CharType* const ep);

        template <int BUFSIZE, typename CharType>
        std::basic_string<CharType>
        Format(const CharType* fmt, ...);

        template <typename CharType>
        std::basic_string<CharType>
        Format(const CharType* fmt, ...);


        // implement

        template <typename CharType>
        bool
        StartWith(const CharType* const str, const CharType* const sub)
        {
            const std::basic_string<CharType> s = str;
            return (s.find(sub) == 0);
        }


        template <typename CharType>
        bool
        EndWith(const CharType* const str, const CharType* const substr)
        {
            const std::basic_string<CharType> s = str;
            const std::basic_string<CharType> sub = substr;
            return (s.rfind(sub) == (s.length() - sub.length()));
        }


        template <typename CharType>
        std::basic_string<CharType>
        ToUpper(const CharType* const str)
        {
            std::basic_string<CharType> s = str;
            std::locale loc;
#if _MSC_VER < 1400 // < vc8
            std::transform(s.begin(), s.end(), s.begin(), ::toupper);
#else
            const std::ctype<CharType>& ct =
                std::use_facet<std::ctype<CharType> >(loc);
            ct.toupper(&s.at(0), &s.at(s.length() - 1) + 1);
#endif
            return s;
        }


        template <typename CharType>
        std::basic_string<CharType>
        ToLower(const CharType* const str)
        {
            std::basic_string<CharType> s = str;
            std::locale loc;
#if _MSC_VER < 1400 // < vc8
            std::transform(s.begin(), s.end(), s.begin(), ::tolower);
#else
            const std::ctype<CharType>& ct =
                std::use_facet<std::ctype<CharType> >(loc);
            ct.tolower(&s.at(0), &s.at(s.length() - 1) + 1);
#endif
            return s;
        }


        template <typename CharType>
        bool
        EqualsIgnoreCase(const CharType* const s1, const CharType* const s2)
        {
            return (ToLower(s1) == ToLower(s2));
        }


        template <typename CharType>
        std::basic_string<CharType>
        Replace(const CharType* const str,
                const CharType* const o, const CharType* const n)
        {
            std::basic_string<CharType> s = str;
            if (std::string::npos == s.find(o))
            {
                return s;
            }

            const std::basic_string<CharType> oldS = o;
            const std::basic_string<CharType> newS = n;
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


        template <typename CharType>
        std::basic_string<CharType>
        TrimLeft(const CharType* str)
        {
            std::basic_string<CharType> s = str;
            const std::locale loc;
            typename std::basic_string<CharType>::iterator it = s.begin();
            while (it != s.end() && std::isspace(*it, loc))
            {
                ++it;
            }
            return std::basic_string<CharType>(it, s.end());
        }


        template <typename CharType>
        std::basic_string<CharType>
        TrimRight(const CharType* str)
        {
            std::basic_string<CharType> s = str;
            const std::locale loc;
            typename std::basic_string<CharType>::reverse_iterator it =
                s.rbegin();
            while (it != s.rend() && std::isspace(*it, loc))
            {
                ++it;
            }
            return std::basic_string<CharType>(s.begin(), it.base());
        }


        template <typename CharType>
        std::basic_string<CharType>
        Trim(const CharType* str)
        {
            std::basic_string<CharType> s = TrimLeft(str);
            return TrimRight(s.c_str());
        }


        template <typename InIt, typename CharType>
        std::basic_string<CharType>
        Join(InIt first, InIt last, const CharType* const ep)
        {
            std::basic_string<CharType> s;

            const std::basic_string<CharType> sep = ep;
            for (InIt it = first; it != last; ++it)
            {
                s+= *it;
                s+= sep;
            }

            const typename std::basic_string<CharType>::size_type sepLen =
                sep.size();
            if (s.size() >= sepLen)
            {
                s.erase(s.length() - sepLen, sepLen);
            }

            return s;
        }


        template <typename CharType>
        std::vector<std::basic_string<CharType> >
        Split(const CharType* const str, const CharType* const ep)
        {
            std::basic_string<CharType> s = str;
            std::vector<std::basic_string<CharType> > vs;
            const std::basic_string<CharType> sep = ep;

            if (s.empty())
            {
                return vs;
            }
            if (sep.empty() || std::string::npos == s.find(sep))
            {
                vs.push_back(s);
                return vs;
            }

            const typename std::basic_string<CharType>::size_type sepLen =
                sep.size();
            typename std::basic_string<CharType>::size_type beg = 0;
            typename std::basic_string<CharType>::size_type e =
                std::string::npos;

            while (std::basic_string<CharType>::npos != (e = s.find(sep, beg)))
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
#ifdef _MSC_VER
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
#ifdef _WIN32
            return _vsnwprintf(buf, count, fmt, ap);
#else
            return vswprintf(buf, count, fmt, ap);
#endif
        }

        template <int BUFSIZE, typename CharType>
        std::basic_string<CharType>
        Format(const CharType* fmt, ...)
        {
            CharType buf[BUFSIZE] = {0};
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

        template <typename CharType>
        std::basic_string<CharType>
        Format(const CharType* fmt, ...)
        {
            static const int MAX_LINE_LEN = 1024 * 8; // default buffer size
            static CharType buf[MAX_LINE_LEN] = {0};
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

    }

}


#endif
