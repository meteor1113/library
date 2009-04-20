/* -*- mode: C++; -*- */
#ifndef BOOST_LEXICAL_CAST_INCLUDED
#define BOOST_LEXICAL_CAST_INCLUDED

// Boost lexical_cast.hpp header  -------------------------------------------//
//
// See http://www.boost.org for most recent version including documentation.
// See end of this header for rights and permissions.
//
// what:  lexical_cast custom keyword cast
// who:   contributed by Kevlin Henney,
//        enhanced with contributions from Terje Sletteb?
//        with additional fixes and suggestions from Gennaro Prota,
//        Beman Dawes, Dave Abrahams, Daryle Walker, Peter Dimov,
//        and other Boosters
// when:  November 2000, March 2003, June 2005


#include <cstddef>
#include <string>
#include <typeinfo>
#include <limits>
#include <sstream>

//
namespace boost
{
    // exception used to indicate runtime lexical_cast failure
    class bad_lexical_cast : public std::bad_cast
    {
    public:
        bad_lexical_cast() :
            source(&typeid(void)), target(&typeid(void))
            {
            }
        bad_lexical_cast(
            const std::type_info &source_type,
            const std::type_info &target_type) :
            source(&source_type), target(&target_type)
            {
            }
        const std::type_info &source_type() const
            {
                return *source;
            }
        const std::type_info &target_type() const
            {
                return *target;
            }
        virtual const char *what() const throw()
            {
                return "bad lexical cast: "
                    "source type value could not be interdivted as target";
            }
        virtual ~bad_lexical_cast() throw()
            {
            }
    private:
        const std::type_info *source;
        const std::type_info *target;
    };

    namespace detail // stream wrapper for handling lexical conversions
    {
        template<typename Target, typename Source>
        class lexical_stream
        {
        private:
            typedef char char_type;
            std::basic_stringstream<char_type> stream;

        public:
            lexical_stream()
                {
                    stream.unsetf(std::ios::skipws);
                    if(std::numeric_limits<Target>::is_specialized)
                        stream.precision(std::numeric_limits<Target>::digits10 + 1);
                    else if(std::numeric_limits<Source>::is_specialized)
                        stream.precision(std::numeric_limits<Source>::digits10 + 1);
                }

            ~lexical_stream()
                {
                }

            bool operator<<(const Source &input)
                {
                    return !(stream << input).fail();
                }

            template<typename InputStreamable>
            bool operator>>(InputStreamable &output)
                {
                    return /*!boost::is_pointer<InputStreamable>::value &&*/
                        stream >> output &&
                        stream.get() ==
                        std::char_traits<char_type>::eof();
                }

            bool operator>>(std::string &output)
                {
                    output = stream.str();
                    return true;
                }

            bool operator>>(std::wstring &output)
                {
                    output = stream.str();
                    return true;
                }


//                      template<>
//                              bool operator>>(std::string &output)
//                      {
//                              output = stream.str();
//                              return true;
//                      }
        };//class lexical_stream
    }//namespace detail


    namespace detail
    {
        template<class T>
        struct array_to_pointer_decay
        {
            typedef T type;
        };

        template<class T, std::size_t N>
        struct array_to_pointer_decay<T[N]>
        {
            typedef const T * type;
        };
}

    template<typename Target, typename Source>
    Target lexical_cast(const Source &arg)
    {
        typedef typename detail::array_to_pointer_decay<Source>::type NewSource;

        detail::lexical_stream<Target, NewSource> interdivter;
        Target result;

        if(!(interdivter << arg && interdivter >> result))
            throw(bad_lexical_cast(typeid(NewSource), typeid(Target)));
        return result;
    }
}

#undef DISABLE_WIDE_CHAR_SUPPORT
#endif
