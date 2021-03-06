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


#ifdef _MSC_VER
#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
#endif
// #include <standard library headers>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <locale>
#include <algorithm>
#include <vector>
#include <stdexcept>

// #include <other library headers>

// #include "customer headers"


namespace util
{


/**
 * "abcdef" "ab" -> true
 * "abcdef" "ba" -> false
 */
template<typename T>
bool
StartWith(const std::basic_string<T>& str,
          const std::basic_string<T>& sub)
{
    return (str.find(sub) == 0);
}


/**
 * "abcdef" "ab" -> true
 * "abcdef" "ba" -> true
 * "abcdef" "fa" -> true
 * "abcdef" "cb" -> false
 */
template<typename T>
bool
StartOf(const std::basic_string<T>& str,
        const std::basic_string<T>& of)
{
    return (str.find_first_of(of) == 0);
}


/**
 * "abcdef" "ef" -> true
 * "abcdef" "fe" -> false
 */
template<typename T>
bool
EndWith(const std::basic_string<T>& str,
        const std::basic_string<T>& sub)
{
    const std::string::size_type p = str.rfind(sub);
    const std::string::size_type len = str.length() - sub.size();
    return ((p != std::string::npos) && (p == len));
}


/**
 * "abcdef" "ef" -> true
 * "abcdef" "fe" -> true
 * "abcdef" "fa" -> true
 * "abcdef" "ce" -> false
 */
template<typename T>
bool
EndOf(const std::basic_string<T>& str,
      const std::basic_string<T>& of)
{
    return ((!str.empty()) && (str.find_last_of(of) == (str.length() - 1)));
}


template <typename T>
unsigned int
GetSubstrCount(const std::basic_string<T>& str,
               const std::basic_string<T>& sub)
{
    std::string::size_type sublen = sub.length();
    if (sublen <= 0)
    {
        return 0;
    }

    unsigned int ret = 0;
    std::string::size_type p = str.find(sub);
    while (p != std::string::npos)
    {
        ret++;
        p = str.find(sub, p + sublen);
    }

    return ret;
}


template<typename T>
std::basic_string<T>
ToUpper(const std::basic_string<T>& str)
{
    std::basic_string<T> s = str;
// #ifdef _MSC_VER
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
// #else
//     const std::locale loc;
//     std::use_facet<std::ctype<T> >(loc).toupper(s.begin(), s.end());
//     //const std::ctype<T>& ct = std::use_facet<std::ctype<T> >(loc);
//     //ct.toupper(&s.at(0), &s.at(s.length() - 1) + 1);
// #endif
    return s;
}


template<typename T>
std::basic_string<T>
ToLower(const std::basic_string<T>& str)
{
    std::basic_string<T> s = str;
// #ifdef _MSC_VER
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
// #else
//     const std::locale loc;
//     std::use_facet<std::ctype<T> >(loc).tolower(s.begin(), s.end());
// #endif
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
TrimLeft(const std::basic_string<T>& str, const std::basic_string<T>& of)
{
    typename std::basic_string<T>::const_iterator i = str.begin();
    while ((i != str.end()) && ((of.find(*i) != std::string::npos)))
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
TrimRight(const std::basic_string<T>& str, const std::basic_string<T>& of)
{
    typename std::basic_string<T>::const_reverse_iterator i = str.rbegin();
    while ((i != str.rend()) && (of.find(*i) != std::string::npos))
    {
        ++i;
    }
    return std::basic_string<T>(str.begin(), i.base());
}


/**
 * Trim all space, include CR and LF.
 */
template<typename T>
std::basic_string<T>
Trim(const std::basic_string<T>& str)
{
    return TrimRight(TrimLeft(str));
}


/**
 * Trim all char in of.
 */
template<typename T>
std::basic_string<T>
Trim(const std::basic_string<T>& str, const std::basic_string<T>& of)
{
    return TrimRight(TrimLeft(str, of), of);
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


template<typename T>
std::basic_string<T>
Format(const T* fmt, ...)
{
    assert(fmt != NULL);

    static const size_t MAXSIZE = 2 * 1024 * 1024;
    T stackbuf[1024];
    size_t size = sizeof(stackbuf);
    T* buf = &stackbuf[0];
    std::vector<T> dynamicbuf;

    while (1)
    {
        va_list ap;
        va_start(ap, fmt);
        int needed = Vsnprintf(buf, size, fmt, ap);
        if ((needed <= (int)size) && (needed >= 0))
        {
            return std::basic_string<T>(buf, (size_t)needed);
        }
        size = (needed > 0) ? (needed + 1) : (size * 2);
        if (size > MAXSIZE)
        {
            break;
        }
        dynamicbuf.resize(size);
        buf = &dynamicbuf[0];
        va_end(ap);
    }

    return fmt;
}


template<typename T>
struct PathSep;


template<>
struct PathSep<char>
{
#ifdef _WIN32
    static std::basic_string<char> Sep() { return "\\"; }
#else
    static std::basic_string<char> Sep() { return "/"; }
#endif
    static std::basic_string<char> Seps() { return "/\\"; }
    static std::basic_string<char> Dot() { return "."; }
};


template<>
struct PathSep<wchar_t>
{
#ifdef _WIN32
    static std::basic_string<wchar_t> Sep() { return L"\\"; }
#else
    static std::basic_string<wchar_t> Sep() { return L"/"; }
#endif
    static std::basic_string<wchar_t> Seps() { return L"/\\"; }
    static std::basic_string<wchar_t> Dot() { return L"."; }
};


/**
 * "/tmp/scratch.tiff" -> "scratch.tiff"
 * "/tmp//scratch"     -> "scratch"
 * "/tmp/"             -> "tmp"
 * "scratch"           -> "scratch"
 * "/"                 -> ""(an empty string)
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
GetLastPath(const std::basic_string<T>& str)
{
    std::basic_string<T> s = str;
    while (EndOf(s, PathSep<T>::Seps()))
    {
        s.resize(s.length() - 1);
    }
    const std::string::size_type pos = s.find_last_of(PathSep<T>::Seps());
    return (pos == std::string::npos) ? s : s.substr(pos + 1);
}


/**
 * "/tmp"              -> "/tmp/scratch.tiff"
 * "/tmp/"             -> "/tmp/scratch.tiff"
 * "/"                 -> "/scratch.tiff"
 * ""(an empty string) -> "scratch.tiff"
 * "tmp"               -> "tmp/scratch.tiff"
 * "//tmp///"          -> "//tmp///scratch.tiff"
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
           const std::basic_string<T>& comp)
{
    if (str.empty())
    {
        return comp;
    }
    std::basic_string<T> s = str;
    if (!EndOf(s, PathSep<T>::Seps()))
    {
        s.append(PathSep<T>::Sep());
    }
    s.append(comp);
    return s;
}


/**
 * "/tmp/scratch.tiff" -> "/tmp"
 * "/tmp//lock/"       -> "/tmp/"
 * "/tmp/"             -> "/"
 * "/tmp"              -> "/"
 * "/"                 -> "/"
 * "scratch.tiff"      -> ""(an empty string)
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
DeleteLastPath(const std::basic_string<T>& str)
{
    std::basic_string<T> s = str;
    while (EndOf(s, PathSep<T>::Seps()))
    {
        s.resize(s.length() - 1);
    }
    const std::string::size_type pos = s.find_last_of(PathSep<T>::Seps());
    if (pos == std::string::npos)
    {
        s = std::basic_string<T>();
    }
    else
    {
        s.resize(pos);
    }
    if ((!str.empty()) &&s.empty() && StartOf(str, PathSep<T>::Seps()))
    {
        s = str[0];
    }
    return s;
}


/**
 * "/tmp/scratch.tiff" -> "tiff"
 * "/tmp//scratch"     -> ""(an empty string)
 * "/tmp/"             -> ""(an empty string)
 * "/scratch..tiff"    -> "tiff"
 * "/scratch.tiff/tmp" -> ""
 * "/"                 -> ""(an empty string)
 */
template<typename T>
std::basic_string<T>
GetPathExtension(const std::basic_string<T>& str)
{
    std::basic_string<T> s = GetLastPath(str);
    const std::string::size_type pos = s.rfind(PathSep<T>::Dot());
    if (pos == std::string::npos)
    {
        return std::basic_string<T>();
    }
    return s.substr(pos + 1);
}


/**
 * "/tmp/scratch.old"  -> "/tmp/scratch.old.tiff"
 * "/tmp/scratch."     -> "/tmp/scratch..tiff"
 * "/tmp//"            -> "/tmp//.tiff"
 * "/scratch"          -> "scratch.tiff"
 * "c:\a"              -> "c:\a.tiff"
 */
template<typename T>
std::basic_string<T>
AppendPathExtension(const std::basic_string<T>& str,
                    const std::basic_string<T>& ext)
{
    std::basic_string<T> s = str;
    s.append(PathSep<T>::Dot());
    s.append(ext);
    return s;
}


/**
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
DeletePathExtension(const std::basic_string<T>& str)
{
    const std::string::size_type pos = str.rfind(PathSep<T>::Dot());
    return (pos == std::string::npos) ? str : str.substr(0, pos);
}


/**
 * Get %z get strftime.
 */
#ifdef _WIN32                   // struct tm have not tm_gmtoff in Aix.
inline
std::string GetTimezone()
{
    time_t now;
    time(&now);
    struct tm* ptm = localtime(&now);

    // Get offset from local time to GMT time
    long gmtoff = 0;
#if defined(_MSC_VER)
#   if (_MSC_VER > 1200) // VC6 above, not including VC6
    _get_timezone(&gmtoff);
#   else
    gmtoff = _timezone;
#   endif
#elif defined(__MINGW32__)
    gmtoff = _timezone;
#elif defined(__GNUC__)
    gmtoff = ptm->tm_gmtoff;
#endif

    return Format("%+03d%02d",
#ifdef _WIN32
                       -(gmtoff / (60 * 60)),
#else
                       (gmtoff / (60 * 60)),
#endif
                       (gmtoff % (60 * 60)));
}
#endif


/**
 * Convert current time to string format
 * @param fmt use strftime's format. e.g. "%Y-%m-%dT%H:%M:%S%z"
 * @return current time's string
 */
inline
std::string FormatCurDateTime(const std::string& fmt)
{
    std::string format = fmt;
#ifdef _WIN32
    format = Replace<char>(format, "%z", GetTimezone());
#endif

    time_t clock;
    time(&clock);
    char date[512];
    memset(date, 0, 512);
    strftime(date, 512, format.c_str(), localtime(&clock));
    return date;
}


} // end of namespace


#endif
