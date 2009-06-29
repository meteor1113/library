#include <assert.h>

#include <iostream>

#include "algo.hpp"


void Test()
{
    std::cout << std::endl << "algo::LexicalCast----------------:" << std::endl;
    {
        int i = 0;
        double d = 0.0;
        assert(algo::LexicalCast(i, 12345));
        std::cout << "12345 ->" << i << "(true)" << std::endl;
        assert(algo::LexicalCast(d, 12345));
        std::cout << "12345 ->" << d << "(true)" << std::endl;
        assert(algo::LexicalCast(d, 0.12345));
        std::cout << "0.12345 ->" << d << "(true)" << std::endl;
        assert(!algo::LexicalCast(i, 1.12345));
        std::cout << "1.12345 ->" << i << "(false)" << std::endl;
    }

    std::cout << std::endl << "StringToNumber----------------:" << std::endl;
    {
        int i = 0;
        double d = 0.0;
        assert(algo::StringToNumber(i, "12345"));
        std::cout << "12345 ->" << i << "(true)" << std::endl;
        assert(!algo::StringToNumber(i, "a12345"));
        std::cout << "a12345 ->" << i << "(false)" << std::endl;
        assert(!algo::StringToNumber(i, "123a45"));
        std::cout << "123a45 ->" << i << "(false)" << std::endl;
        assert(!algo::StringToNumber(i, "12345a"));
        std::cout << "12345a ->" << i << "(false)" << std::endl;

        assert(algo::StringToNumber(d, "0.12345"));
        std::cout << "0.12345 ->" << d << "(true)" << std::endl;
        assert(!algo::StringToNumber(d, "a0.12345"));
        std::cout << "a0.12345 ->" << d << "(false)" << std::endl;
        assert(!algo::StringToNumber(d, "a0.12a345"));
        std::cout << "0.12a345 ->" << d << "(false)" << std::endl;
        assert(!algo::StringToNumber(d, "0.12345a"));
        std::cout << "0.12345a ->" << d << "(false)" << std::endl;

        assert(algo::StringToNumber(d, "12345"));
        std::cout << "12345 ->" << d << "(true)" << std::endl;

        assert(algo::StringToNumber(i, "0x12345a", std::hex));
        std::cout << "0x12345a ->" << i << "(true)" << std::endl;
        assert(!algo::StringToNumber(i, "0x12345a"));
        std::cout << "0x12345a ->" << i << "(false)" << std::endl;
        assert(!algo::StringToNumber(i, "0x12345g"));
        std::cout << "0x12345g ->" << i << "(false)" << std::endl;
    }

}
        
        
int main()
{
    Test();
    return 0;
}
