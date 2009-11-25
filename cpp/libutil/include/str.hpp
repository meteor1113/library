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


#ifndef STR_HPP_
#define STR_HPP_


#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
// #include <standard library headers>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <locale>
#include <algorithm>
#include <vector>
#include <stdexcept>

// #include <other library headers>

// #include "customer headers"


namespace str
{

    template<typename T>
    bool
    StartWith(const std::basic_string<T>& str,
              const std::basic_string<T>& sub)
    {
        return (str.find(sub) == 0);
    }


    template<typename T>
    bool
    EndWith(const std::basic_string<T>& str,
            const std::basic_string<T>& sub)
    {
        const std::string::size_type p = str.rfind(sub);
        const std::string::size_type len = str.length() - sub.size();
        return ((p != std::string::npos) && (p == len));
    }


    template<typename T>
    std::basic_string<T>
    ToUpper(const std::basic_string<T>& str)
    {
        std::basic_string<T> s = str;
#if _MSC_VER < 1400 // < vc8(vs2005)
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
#else
        const std::locale loc;
        std::use_facet<std::ctype<T> >(loc).toupper(s.begin(), s.end());
        //const std::ctype<T>& ct = std::use_facet<std::ctype<T> >(loc);
        //ct.toupper(&s.at(0), &s.at(s.length() - 1) + 1);
#endif
        return s;
    }


    template<typename T>
    std::basic_string<T>
    ToLower(const std::basic_string<T>& str)
    {
        std::basic_string<T> s = str;
#if _MSC_VER < 1400 // < vc8(vs2005)
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
#else
        const std::locale loc;
        std::use_facet<std::ctype<T> >(loc).tolower(s.begin(), s.end());
#endif
        return s;
    }


    template<typename T>
    bool
    EqualsIgnoreCase(const std::basic_string<T>& s1,
                     const std::basic_string<T>& s2)
    {
        return (ToLower(s1) == ToLower(s2));
    }


    template<typename T>
    std::basic_string<T>
    Replace(const std::basic_string<T>& str,
            const std::basic_string<T>& o,
            const std::basic_string<T>& n)
    {
        if (std::string::npos == str.find(o))
        {
            return str;
        }
        std::basic_string<T> s = str;
        std::string::size_type beg = 0;
        std::string::size_type tmp = 0;
        while ((beg = (s.find(o, tmp))) != std::string::npos)
        {
            s.replace(beg, o.length(), n);
            tmp = beg + n.size();
            if (tmp >= s.length())
            {
                break;
            }
        }
        return s;
    }


    template<typename T>
    std::basic_string<T>
    TrimLeft(const std::basic_string<T>& str)
    {
        const std::locale loc;
        typename std::basic_string<T>::const_iterator i = str.begin();
        while ((i != str.end()) && std::isspace(*i, loc))
        {
            ++i;
        }
        return std::basic_string<T>(i, str.end());
    }


    template<typename T>
    std::basic_string<T>
    TrimRight(const std::basic_string<T>& str)
    {
        const std::locale loc;
        typename std::basic_string<T>::const_reverse_iterator i = str.rbegin();
        while ((i != str.rend()) && std::isspace(*i, loc))
        {
            ++i;
        }
        return std::basic_string<T>(str.begin(), i.base());
    }


    template<typename T>
    std::basic_string<T>
    Trim(const std::basic_string<T>& str)
    {
        return TrimRight(TrimLeft(str));
    }


    template<typename T, typename InIt>
    std::basic_string<T>
    Join(InIt first, InIt last, const std::basic_string<T>& sep)
    {
        std::basic_string<T> s;
        for (InIt i = first; i != last; ++i)
        {
            s.append(*i);
            s.append(sep);
        }
        const std::string::size_type len = sep.size();
        if (s.size() >= len)
        {
            s.erase(s.length() - len, len);
        }
        return s;
    }


    template<typename T>
    std::vector<std::basic_string<T> >
    Split(const std::basic_string<T>& str,
          const std::basic_string<T>& sep)
    {
        std::vector<std::basic_string<T> > vs;
        if (str.empty())
        {
            return vs;
        }
        if (sep.empty() || (std::string::npos == str.find(sep)))
        {
            vs.push_back(str);
            return vs;
        }
        std::string::size_type beg = 0;
        std::string::size_type e = std::string::npos;
        while (std::string::npos != (e = str.find(sep, beg)))
        {
            vs.push_back(str.substr(beg, e - beg));
            beg = e + sep.size();
        }
        vs.push_back(str.substr(beg, str.size() - beg));
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


    template<int BUFSIZE, typename T>
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


    template<typename T>
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
     * if sep is "/":
     * "/tmp/scratch.tiff" -> "scratch.tiff"
     * "/tmp//scratch"     -> "scratch"
     * "/tmp/"             -> "tmp"
     * "scratch"           -> "scratch"
     * "/"                 -> ""(an empty string)
     *
     * if sep is "\"
     * "\tmp\scratch.tiff" -> "scratch.tiff"
     * "\tmp\\scratch"     -> "scratch"
     * "\tmp\"             -> "tmp"
     * "scratch"           -> "scratch"
     * "\"                 -> ""(an empty string)
     * "c:\aaa\bbb"        -> "bbb"
     * "c:\"               -> "c:"
     * "c:"                -> "c:"
     */
    template<typename T>
    std::basic_string<T>
    GetLastPath(const std::basic_string<T>& str,
                const std::basic_string<T>& sep)
    {
        std::basic_string<T> s = str;
        while (EndWith(s, sep))
        {
            s.resize(s.length() - sep.length());
        }
        const std::string::size_type pos = s.rfind(sep);
        return (pos == std::string::npos) ? s : s.substr(pos + 1);
    }


    inline
    std::string
    GetLastPath(const std::string& str)
    {
#ifdef _WIN32
        return GetLastPath<char>(str, "\\");
#else
        return GetLastPath<char>(str, "/");
#endif
    }


    inline
    std::wstring
    GetLastPath(const std::wstring& str)
    {
#ifdef _WIN32
        return GetLastPath<wchar_t>(str, L"\\");
#else
        return GetLastPath<wchar_t>(str, L"/");
#endif
    }


    /**
     * if sep is "/" and comp is "scratch.tiff":
     * "/tmp"              -> "/tmp/scratch.tiff"
     * "/tmp/"             -> "/tmp/scratch.tiff"
     * "/"                 -> "/scratch.tiff"
     * ""(an empty string) -> "scratch.tiff"
     * "tmp"               -> "tmp/scratch.tiff"
     * "//tmp///"          -> "//tmp///scratch.tiff"
     *
     * if sep is "\" and comp is "scratch.tiff":
     * "\tmp"              -> "\tmp\scratch.tiff"
     * "\tmp\"             -> "\tmp\scratch.tiff"
     * "\"                 -> "\scratch.tiff"
     * ""(an empty string) -> "scratch.tiff"
     * "tmp"               -> "tmp\scratch.tiff"
     * "\\tmp\\"           -> "\\tmp\\scratch.tiff"
     * "c:\aaa\bbb"        -> "c:\aaa\bbb\scratch.tiff"
     * "c:\"               -> "c:\scratch.tiff"
     * "c:"                -> "c:\scratch.tiff"
     */
    template<typename T>
    std::basic_string<T>
    AppendPath(const std::basic_string<T>& str,
               const std::basic_string<T>& comp,
               const std::basic_string<T>& sep)
    {
        if (str.empty())
        {
            return comp;
        }
        std::basic_string<T> s = str;
        if (!EndWith(s, sep))
        {
            s.append(sep);
        }
        s.append(comp);
        return s;
    }


    inline
    std::string
    AppendPath(const std::string& str, const std::string& comp)
    {
#ifdef _WIN32
        return AppendPath<char>(str, comp, "\\");
#else
        return AppendPath<char>(str, comp, "/");
#endif
    }


    inline
    std::wstring
    AppendPath(const std::wstring& str, const std::wstring& comp)
    {
#ifdef _WIN32
        return AppendPath<wchar_t>(str, comp, L"\\");
#else
        return AppendPath<wchar_t>(str, comp, L"/");
#endif
    }


    /**
     * if sep is "/":
     * "/tmp/scratch.tiff" -> "/tmp"
     * "/tmp//lock/"       -> "/tmp/"
     * "/tmp/"             -> "/"
     * "/tmp"              -> "/"
     * "/"                 -> "/"
     * "scratch.tiff"      -> ""(an empty string)
     *
     * if sep is "\"
     * "\tmp\scratch.tiff" -> "\tmp"
     * "\tmp\\lock\"       -> "\tmp\"
     * "\tmp\"             -> "\"
     * "\tmp"              -> "\"
     * "\"                 -> "\"
     * "scratch.tiff"      -> ""(an empty string)
     * "c:\aaa\bbb"        -> "c:\aaa"
     * "c:\"               -> ""(an empty string)
     * "c:"                -> ""(an empty string)
     */
    template<typename T>
    std::basic_string<T>
    DeleteLastPath(const std::basic_string<T>& str,
                   const std::basic_string<T>& sep)
    {
        std::basic_string<T> s = str;
        while (EndWith(s, sep))
        {
            s.resize(s.length() - sep.length());
        }
        const std::string::size_type pos = s.rfind(sep);
        if (pos == std::string::npos)
        {
            s = std::basic_string<T>();
        }
        else
        {
            s.resize(pos);
        }
        if (s.empty() && StartWith(str, sep))
        {
            s = sep;
        }
        return s;
    }


    inline
    std::string
    DeleteLastPath(const std::string& str)
    {
#ifdef _WIN32
        return DeleteLastPath<char>(str, "\\");
#else
        return DeleteLastPath<char>(str, "/");
#endif
    }


    inline
    std::wstring
    DeleteLastPath(const std::wstring& str)
    {
#ifdef _WIN32
        return DeleteLastPath<wchar_t>(str, L"\\");
#else
        return DeleteLastPath<wchar_t>(str, L"/");
#endif
    }


    /**
     * if sep is ".":
     * "/tmp/scratch.tiff" -> "tiff"
     * "/tmp//scratch"     -> ""(an empty string)
     * "/tmp/"             -> ""(an empty string)
     * "/scratch..tiff"    -> "tiff"
     * "/scratch.tiff/tmp" -> "tiff/tmp"
     * "/"                 -> ""(an empty string)
     *
     * if you want get path extension from full path, e.g. /scratch.tiff/tmp,
     * you must call GetLastPath() before call this function,
     * GetPathExtension(GetLastPath("/scratch.tiff/tmp")) will get "".
     */
    template<typename T>
    std::basic_string<T>
    GetPathExtension(const std::basic_string<T>& str,
                     const std::basic_string<T>& sep)
    {
        const std::string::size_type pos = str.rfind(sep);
        if (pos == std::string::npos)
        {
            return std::basic_string<T>();
        }
        return str.substr(pos + 1);
    }


    inline
    std::string
    GetPathExtension(const std::string& str)
    {
        return GetPathExtension<char>(str, ".");
    }


    inline
    std::wstring
    GetPathExtension(const std::wstring& str)
    {
        return GetPathExtension<wchar_t>(str, L".");
    }


    /**
     * if ext is "tiff" and sep is ".":
     * "/tmp/scratch.old"  -> "/tmp/scratch.old.tiff"
     * "/tmp/scratch."     -> "/tmp/scratch..tiff"
     * "/tmp//"            -> "/tmp//.tiff"
     * "/scratch"          -> "scratch.tiff"
     * "c:\a"              -> "c:\a.tiff"
     */
    template<typename T>
    std::basic_string<T>
    AppendPathExtension(const std::basic_string<T>& str,
                        const std::basic_string<T>& ext,
                        const std::basic_string<T>& sep)
    {
        std::basic_string<T> s = str;
        s.append(sep);
        s.append(ext);
        return s;
    }


    inline
    std::string
    AppendPathExtension(const std::string& str, const std::string& ext)
    {
        return AppendPathExtension<char>(str, ext, ".");
    }


    inline
    std::wstring
    AppendPathExtension(const std::wstring& str, const std::wstring& ext)
    {
        return AppendPathExtension<wchar_t>(str, ext, L".");
    }


    /**
     * if ext is "tiff" and sep is ".":
     * "/tmp/scratch.tiff" -> "/tmp/scratch"
     * "/tmp/"             -> "/tmp/"
     * "scratch.bundle/"   -> "scratch"
     * "scratch..tiff"     -> "scratch."
     * ".tiff"             -> ""(an empty string)
     * "/"                 -> "/"
     * "scratch..."        -> "scratch.."
     * "tmp.bundle/scrtch" -> "tmp"
     *
     * if you want get thin name from full path, e.g. /tmp.bundle/scratch,
     * you must call GetLastPath() before call this function,
     * DeleteLastPath(GetLastPath("/tmp.bundle/scratch")) will get "scratch".
     */
    template<typename T>
    std::basic_string<T>
    DeletePathExtension(const std::basic_string<T>& str,
                        const std::basic_string<T>& sep)
    {
        const std::string::size_type pos = str.rfind(sep);
        return (pos == std::string::npos) ? str : str.substr(0, pos);
    }


    inline
    std::string
    DeletePathExtension(const std::string& str)
    {
        return DeletePathExtension<char>(str, ".");
    }


    inline
    std::wstring
    DeletePathExtension(const std::wstring& str)
    {
        return DeletePathExtension<wchar_t>(str, L".");
    }

}


#endif
