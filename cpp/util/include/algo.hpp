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


#ifndef ALGO_HPP_
#define ALGO_HPP_


#pragma warning(disable: 4786)
#pragma warning(disable: 4996)
// #include <standard library headers>
#include <string>
#include <sstream>
#include <limits>

// #include <other library headers>

// #include "customer headers"


namespace algo
{

    template <typename T>
    void
    Delete(T* t)
    {
        delete t;
    }


    template <typename Target, typename Source>
    bool
    LexicalCast(Target& t, const Source& s)
    {
        std::stringstream stream;
        if (std::numeric_limits<Target>::is_specialized)
        {
            stream.precision(std::numeric_limits<Target>::digits10 + 1);
        }
        else if (std::numeric_limits<Source>::is_specialized)
        {
            stream.precision(std::numeric_limits<Source>::digits10 + 1);
        }
        stream << s;
        stream >> t;
        bool ret = !stream.fail();
        bool eof = (stream.get() == std::char_traits<char>::eof());
        return (ret && eof);
    }


    /**
     *
     * @param s
     * @param f one of std::hex, std::dec, std::oct
     */
    template <typename T, typename C>
    bool
    StringToNumber(T& t,
                   const C* s,
                   std::ios_base&(*f)(std::ios_base&) = std::dec)
    {
        std::basic_stringstream<C> stream;
        stream << s;
        stream >> f >> t;
        bool ret = !stream.fail();
        bool eof = (stream.get() == std::char_traits<C>::eof());
        return (ret && eof);
    }

} // end of namespace algo


#endif // ALGO_HPP_
