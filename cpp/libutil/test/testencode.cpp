#include <assert.h>

#include <iostream>

#ifdef _WIN32
#include "encode.hpp"
#endif

void Test()
{
#ifdef _WIN32
    std::cout << std::endl << "ToUtf8----------------:" << std::endl;
    {
        std::cout << "abc -> " << encode::ToUtf8(L"abc") << std::endl;;
    }

    std::cout << std::endl << "ToUtf8----------------:" << std::endl;
    {
        std::cout << "abc -> " << encode::ToUtf8("abc")<< std::endl;;
    }
#ifndef __MINGW32__
    std::cout << std::endl << "Utf8ToWstring----------------:" << std::endl;
    {
        std::wcout << L"abc -> " << encode::Utf8ToWstring("abc") << std::endl;;
    }
#endif
    std::cout << std::endl << "Utf8ToAscii----------------:" << std::endl;
    {
        std::cout << "abc -> " << encode::Utf8ToAscii("abc") << std::endl;;
    }
#ifndef __MINGW32__
    std::cout << std::endl << "ToWstring----------------:" << std::endl;
    {
        std::wcout << L"abc -> " << encode::ToWstring("abc") << std::endl;;
    }
#endif
    std::cout << std::endl << "ToAscii----------------:" << std::endl;
    {
        std::cout << "abc -> " << encode::ToAscii(L"abc") << std::endl;;
    }
#endif
}
        
        
int main()
{
    Test();
    return 0;
}
