#include <assert.h>

#include <stdio.h>
#include <string.h>

#include <iostream>

#include "encode.hpp"


const char* TEST_STR = "各C/C++编译器对wchar_t字符和字符串的正确支持程度";
const wchar_t* TEST_WSTR = L"各C/C++编译器对wchar_t字符和字符串的正确支持程度";
// const char* TEST_STR = "传统的C风格的字符串是以字符为结尾的一系列字节的集合，其一个单元为char类型，\
// 可以保存世界上的几百种不同的字符集，包括ASCII、ISO-8859、GBK、BIG-5、SHIFT-JIS、\
// UTF-8等等。通常这些字符集都是兼容ASCII字符集的。我们可以把这些兼容ASCII字符集统称\
// 为ANSI字符集。正因为各种不同的字符集的存在，所以在实际的使用中会产生各种缺字和乱码\
// 的现象。\
//         Unicode是包含世界上各种语言和符号的编码。\
//         在实际应用中，Unicode以UTF-8和UTF-16、UTF-32这三种形式存在，这三种形式都可以完\
// 美地表示Unicode的各个码位。\
//         例如：中文的中字，其Unicode码位为U+4E2D，用UTF-8用三个8位的字节来表示为：0xE4\
// 0xB8   0xAD，UTF-16则使用一个16位的整数0x4E2D来表示，UTF-32则使用一个32位的整数\
// 0x00004E2D来表示。(当字符码位大于0xFFFF时，UTF-8要用四个8位的字节来表示，UTF-16需要\
// 使用2个16位整数来表示)\
//         UTF-16与UTF-32相比，UTF-16显得更加简洁。Windows下普遍使用UTF-16，而Linux和其它\
// Unix类的系统由于历史上的设计原因，则比较倾向于使用UTF-8和UTF-32这两种传输方式。\
//         C/C++标准均已经提供了wchat_t关键字来实现对Unicode的支持，而且wchat_t类型只用于\
// Unicode编码。在Windows平台下，wchat_t类型是16位的；而在Linux下，wchat_t类型是32位的。";
// const wchar_t* TEST_WSTR = L"传统的C风格的字符串是以字符为结尾的一系列字节的集合，其一个单元为char类型，\
// 可以保存世界上的几百种不同的字符集，包括ASCII、ISO-8859、GBK、BIG-5、SHIFT-JIS、\
// UTF-8等等。通常这些字符集都是兼容ASCII字符集的。我们可以把这些兼容ASCII字符集统称\
// 为ANSI字符集。正因为各种不同的字符集的存在，所以在实际的使用中会产生各种缺字和乱码\
// 的现象。\
//         Unicode是包含世界上各种语言和符号的编码。\
//         在实际应用中，Unicode以UTF-8和UTF-16、UTF-32这三种形式存在，这三种形式都可以完\
// 美地表示Unicode的各个码位。\
//         例如：中文的中字，其Unicode码位为U+4E2D，用UTF-8用三个8位的字节来表示为：0xE4\
// 0xB8   0xAD，UTF-16则使用一个16位的整数0x4E2D来表示，UTF-32则使用一个32位的整数\
// 0x00004E2D来表示。(当字符码位大于0xFFFF时，UTF-8要用四个8位的字节来表示，UTF-16需要\
// 使用2个16位整数来表示)\
//         UTF-16与UTF-32相比，UTF-16显得更加简洁。Windows下普遍使用UTF-16，而Linux和其它\
// Unix类的系统由于历史上的设计原因，则比较倾向于使用UTF-8和UTF-32这两种传输方式。\
//         C/C++标准均已经提供了wchat_t关键字来实现对Unicode的支持，而且wchat_t类型只用于\
// Unicode编码。在Windows平台下，wchat_t类型是16位的；而在Linux下，wchat_t类型是32位的。";

void Test()
{
    std::cout << std::endl << "WideToUtf8----------------:" << std::endl;
    {
        std::string str = util::WideToUtf8(TEST_WSTR);
        std::cout << TEST_STR << " -> " << str << std::endl;
        std::cout << "srclen=" << strlen(TEST_STR) << "    "
                  << "len=" << str.length() << std::endl;
    }

    std::cout << std::endl << "GbkToUtf8----------------:" << std::endl;
    {
        std::string str = util::GbkToUtf8(TEST_STR);
        std::cout << TEST_STR << " -> " << str << std::endl;
        std::cout << "srclen=" << strlen(TEST_STR) << "    "
                  << "len=" << str.length() << std::endl;
    }
#ifndef __MINGW32__
    std::cout << std::endl << "Utf8ToWide----------------:" << std::endl;
    {
		std::string utf8 = util::WideToUtf8(TEST_WSTR);
        std::wstring str = util::Utf8ToWide(utf8.c_str());
        // std::wcout << TEST_WSTR << " -> " << util::Utf8ToWide(util::WideToUtf8(TEST_WSTR).c_str()) << std::endl;
        std::wcout << TEST_WSTR << L" -> " << str << std::endl;
        std::cout << "srclen=" << utf8.length() << "    "
                  << "len=" << str.length() << std::endl;
    }
#endif
    std::cout << std::endl << "Utf8ToGbk----------------:" << std::endl;
    {
		std::string utf8 = util::WideToUtf8(TEST_WSTR);
        std::string str = util::Utf8ToGbk(utf8.c_str());
        // std::cout << TEST_STR << " -> " << util::Utf8ToGbk(util::WideToUtf8(TEST_WSTR).c_str()) << std::endl;
        std::cout << TEST_STR << " -> " << str << std::endl;
        std::cout << "srclen=" << utf8.length() << "    "
                  << "len=" << str.length() << std::endl;
    }
#ifndef __MINGW32__
    std::cout << std::endl << "GbkToWide----------------:" << std::endl;
    {
        std::wstring str = util::GbkToWide(TEST_STR);
        std::wcout << TEST_WSTR << L" -> " << str << std::endl;
        std::cout << "srclen=" << strlen(TEST_STR) << "    "
                  << "len=" << str.length() << std::endl;
    }
#endif
    std::cout << std::endl << "WideToGbk----------------:" << std::endl;
    {
        std::string str = util::WideToGbk(TEST_WSTR);
        std::cout << TEST_STR << " -> " << str << std::endl;
        std::cout << "srclen=" << wcslen(TEST_WSTR) << "    "
                  << "len=" << str.length() << std::endl;
    }
}


int main()
{
    Test();
#ifdef _WIN32
    getchar();
#endif
    return 0;
}
