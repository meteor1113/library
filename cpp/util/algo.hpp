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


#ifndef UTIL_ALGO_HPP_
#define UTIL_ALGO_HPP_


// #include <standard library headers>
#include <string>
#include <stdexcept>
#include <sstream>
#include <limits>
#include <typeinfo>

// #include <other library headers>

// #include "customer headers"


namespace util
{

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
                int p = std::numeric_limits<Target>::digits10 + 1;
                stream.precision(std::numeric_limits<Target>::digits10 + 1);
            }
            else if (std::numeric_limits<Source>::is_specialized)
            {
                int p = std::numeric_limits<Source>::digits10 + 1;
                stream.precision(std::numeric_limits<Source>::digits10 + 1);
            }
            stream << s;
            stream >> t;
            bool ok = (stream && stream.get() == std::char_traits<char>::eof());
            return ok;
        }


        template <typename Target, typename Source>
        Target
        LexicalCast(const Source& s)
        {
            Target t;
            if (!LexicalCast(t, s))
            {
                throw std::bad_cast();
            }

            return t;
        }


        template <typename T>
        bool
        StringToNumber(T& t,
                       const std::string& s,
                       std::ios_base&(*f)(std::ios_base&) = std::dec)
        {
            std::stringstream stream;
            stream << s;
            stream >> f >> t;
            return (stream && stream.get() == std::char_traits<char>::eof());
        }

        /**
         *
         *@param s
         *@param f one of std::hex, std::dec, std::oct
         */
        template <typename T>
        T
        StringToNumber(const std::string& s,
                       std::ios_base&(*f)(std::ios_base&) = std::dec)
        {
            T t;
            if (!StringToNumber(t, s, f))
            {
                throw std::bad_cast();
            }
            return t;
        }

    } // end of namespace algo

} // end of namespace utility


#endif // UTIL_ALGO_HPP_
