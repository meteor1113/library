#ifdef NDEBUG
#undef NDEBUG
#endif


#include <assert.h>

#include <iostream>

#include "str.hpp"

using namespace util;

std::string
AddBracket(const std::string& str)
{
    return "[" + str + "]";
}


std::wstring
AddBracket(const std::wstring& str)
{
    return L"[" + str + L"]";
}


void CoutString(const std::string& str)
{
    std::string s = Replace<char>(str, "\r", "\\r");
    s = Replace<char>(s, "\n", "\\n");
    std::cout << s << std::endl;
}


void CoutString(const std::wstring& str)
{
#ifndef __MINGW32__
    std::wstring s = Replace<wchar_t>(str, L"\r", L"\\r");
    s = Replace<wchar_t>(s, L"\n", L"\\n");
    std::wcout << L"wtring:" << s << std::endl;
#endif
}


void Test()
{
    std::cout << std::endl << "StartWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = StartWith<char>(s, "JDK");
        bool b2 = StartWith(s, std::string("   dsa "));
        bool b3 = StartWith<char>(s, "");
        assert(!b1);
        assert(b2);
        assert(b3);

        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = StartWith<wchar_t>(ws, L"JDK");
        bool wb2 = StartWith<wchar_t>(ws, L"   dsa ");
        bool wb3 = StartWith(ws, std::wstring(L""));
        assert(!wb1);
        assert(wb2);
        assert(wb3);
    }

    std::cout << std::endl << "StartOf----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = StartOf<char>(s, "JDK");
        bool b2 = StartOf(s, std::string("a "));
        bool b3 = StartOf<char>(s, "");
        assert(!b1);
        assert(b2);
        assert(!b3);

        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = StartOf<wchar_t>(ws, L"JDK");
        bool wb2 = StartOf<wchar_t>(ws, L"a ");
        bool wb3 = StartOf(ws, std::wstring(L""));
        assert(!wb1);
        assert(wb2);
        assert(!wb3);
    }

    std::cout << std::endl << "EndWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = EndWith<char>(s, "'ghg");
        bool b2 = EndWith<char>(s, "ghg    ");
        bool b3 = EndWith<char>(s, "");
        bool b4 = EndWith<char>("", "a");
        assert(!b1);
        assert(b2);
        assert(b3);
        assert(!b4);

        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = EndWith<wchar_t>(ws, L"'ghg");
        bool wb2 = EndWith<wchar_t>(ws, L"ghg    ");
        bool wb3 = EndWith<wchar_t>(ws, L"");
        bool wb4 = EndWith<wchar_t>(L"", L"a");
        assert(!wb1);
        assert(wb2);
        assert(wb3);
        assert(!wb4);
    }

    std::cout << std::endl << "EndOf----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = EndOf<char>(s, "'ghg");
        bool b2 = EndOf<char>(s, "ghg ");
        bool b3 = EndOf<char>(s, "");
        bool b4 = EndOf<char>("", "a");
        assert(!b1);
        assert(b2);
        assert(!b3);
        assert(!b4);

        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = EndOf<wchar_t>(ws, L"'ghg");
        bool wb2 = EndOf<wchar_t>(ws, L"ghg ");
        bool wb3 = EndOf<wchar_t>(ws, L"");
        bool wb4 = EndOf<wchar_t>(L"", L"a");
        assert(!wb1);
        assert(wb2);
        assert(!wb3);
        assert(!wb4);
    }

    std::cout << std::endl << "GetSubstrCount----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        unsigned int i1 = GetSubstrCount<char>(s, "DK");
        unsigned int i2 = GetSubstrCount(s, std::string(";"));
        unsigned int i3 = GetSubstrCount<char>(s, "");
        unsigned int i4 = GetSubstrCount<char>(s, "qwer");
        assert(i1 == 2);
        assert(i2 == 2);
        assert(i3 == 0);
        assert(i4 == 0);

        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        unsigned int wi1 = GetSubstrCount<wchar_t>(ws, L"DK");
        unsigned int wi2 = GetSubstrCount(ws, std::wstring(L";"));
        unsigned int wi3 = GetSubstrCount<wchar_t>(ws, L"");
        unsigned int wi4 = GetSubstrCount<wchar_t>(ws, L"qwer");
        assert(wi1 == 2);
        assert(wi2 == 2);
        assert(wi3 == 0);
        assert(wi4 == 0);
    }

    std::cout << std::endl << "ToUpper----------------:" << std::endl;
    {
        std::string s = "   JDKhg;i'    ";
        s= ToUpper<char>(s);
        CoutString(AddBracket(s));
        assert(s == "   JDKHG;I'    ");

        std::wstring ws = L"   JDKhg;i'    ";
        ws = ToUpper<wchar_t>(ws);
        CoutString(AddBracket(ws));
        assert(ws == L"   JDKHG;I'    ");
    }

    std::cout << std::endl << "ToLower----------------:" << std::endl;
    {
        std::string s = "   JDKhg;I'    ";
        s= ToLower<char>(s);
        CoutString(AddBracket(s));
        assert(s == "   jdkhg;i'    ");

        std::wstring ws = L"   JDKhg;I'    ";
        ws = ToLower<wchar_t>(ws);
        CoutString(AddBracket(ws));
        assert(ws == L"   jdkhg;i'    ");
    }

    std::cout << std::endl << "Replace----------------:" << std::endl;
    {
        std::string s = "   JDKhg;''    '";
        s= Replace<char>(s, "'", "''");
        CoutString(AddBracket(s));
        assert(s == "   JDKhg;''''    ''");
        s= Replace<char>(s, "'", "|");
        CoutString(AddBracket(s));
        assert(s == "   JDKhg;||||    ||");

        std::wstring ws = L"   JDKhg;''    '";
        ws = Replace<wchar_t>(ws, L"'", L"''");
        CoutString(AddBracket(ws));
        assert(ws == L"   JDKhg;''''    ''");
        ws = Replace<wchar_t>(ws, L"'", L"|");
        CoutString(AddBracket(ws));
        assert(ws == L"   JDKhg;||||    ||");
    }

    std::cout << std::endl << "TrimLeft----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s);
        CoutString(AddBracket(s));
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg    ");

        s = "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s);
        CoutString(AddBracket(s));
        assert(s == "dsa j;fJDK\nOG:Jd   DK;s 'ghg    ");

        s = "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s, " ");
        CoutString(AddBracket(s));
        assert(s == "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ");

        s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s, "s d");
        CoutString(AddBracket(s));
        assert(s == "a j;fJDKOG:Jd   DK;s 'ghg    ");

        s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s, " sdaD");
        CoutString(AddBracket(s));
        assert(s == "j;fJDKOG:Jd   DK;s 'ghg    ");

        s = "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
        s = TrimLeft<char>(s, "\n sda\rD");
        CoutString(AddBracket(s));
        assert(s == "j;fJDK\nOG:Jd   DK;s 'ghg    ");

#ifndef __MINGW32__             // MinGW's std::isspace() cause crash
        {
            std::wstring s = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s.c_str());
            CoutString(AddBracket(s));
            assert(s == L"dsa j;fJDKOG:Jd   DK;s 'ghg    ");

            s = L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s);
            CoutString(AddBracket(s));
            assert(s == L"dsa j;fJDK\nOG:Jd   DK;s 'ghg    ");

            s = L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s, L" ");
            CoutString(AddBracket(s));
            assert(s == L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ");

            s = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s, L"s d");
            CoutString(AddBracket(s));
            assert(s == L"a j;fJDKOG:Jd   DK;s 'ghg    ");

            s = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s, L" sdaD");
            CoutString(AddBracket(s));
            assert(s == L"j;fJDKOG:Jd   DK;s 'ghg    ");

            s = L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg    ";
            s = TrimLeft<wchar_t>(s, L"\n sda\rD");
            CoutString(AddBracket(s));
            assert(s == L"j;fJDK\nOG:Jd   DK;s 'ghg    ");
        }
#endif
    }

    std::cout << std::endl << "TrimRight----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= TrimRight(s);
        CoutString(AddBracket(s));
        assert(s == "   dsa j;fJDKOG:Jd   DK;s 'ghg");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r ";
        s = TrimRight<char>(s);
        CoutString(AddBracket(s));
        assert(s == " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r ";
        s = TrimRight<char>(s, " ");
        CoutString(AddBracket(s));
        assert(s == " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = TrimRight<char>(s, "s d");
        CoutString(AddBracket(s));
        assert(s == " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = TrimRight<char>(s, " sdaD\r");
        CoutString(AddBracket(s));
        assert(s == " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\n");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = TrimRight<char>(s, "\n sda\rD");
        CoutString(AddBracket(s));
        assert(s == " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg");

#ifndef __MINGW32__             // MinGW's std::isspace() cause crash
        {
            std::wstring s = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            s = TrimRight<wchar_t>(s.c_str());
            CoutString(AddBracket(s));
            assert(s == L"   dsa j;fJDKOG:Jd   DK;s 'ghg");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r ";
            s = TrimRight<wchar_t>(s);
            CoutString(AddBracket(s));
            assert(s == L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r ";
            s = TrimRight<wchar_t>(s, L" ");
            CoutString(AddBracket(s));
            assert(s == L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = TrimRight<wchar_t>(s, L"s d");
            CoutString(AddBracket(s));
            assert(s == L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = TrimRight<wchar_t>(s, L" sdaD\r");
            CoutString(AddBracket(s));
            assert(s == L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\n");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = TrimRight<wchar_t>(s, L"\n sda\rD");
            CoutString(AddBracket(s));
            assert(s == L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg");
        }
#endif
    }

    std::cout << std::endl << "Trim----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= Trim(s);
        CoutString(AddBracket(s));
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r";
        s = Trim<char>(s);
        CoutString(AddBracket(s));
        assert(s == "dsa j;fJDK\nOG:Jd   DK;s 'ghg");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r";
        s = Trim<char>(s, " ");
        CoutString(AddBracket(s));
        assert(s == "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = Trim<char>(s, "s d");
        CoutString(AddBracket(s));
        assert(s == "\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = Trim<char>(s, " sdaD\r");
        CoutString(AddBracket(s));
        assert(s == "\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\n");

        s = " \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
        s = Trim<char>(s, "\n sda\rD");
        CoutString(AddBracket(s));
        assert(s == "j;fJDK\nOG:Jd   DK;s 'ghg");

#ifndef __MINGW32__             // MinGW's std::isspace() cause crash
        {
            std::wstring s = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            s  = Trim(s);
            CoutString(AddBracket(s));
            assert(s == L"dsa j;fJDKOG:Jd   DK;s 'ghg");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r";
            s = Trim<wchar_t>(s);
            CoutString(AddBracket(s));
            assert(s == L"dsa j;fJDK\nOG:Jd   DK;s 'ghg");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r";
            s = Trim<wchar_t>(s, L" ");
            CoutString(AddBracket(s));
            assert(s == L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg   \r\n\r");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = Trim<wchar_t>(s, L"s d");
            CoutString(AddBracket(s));
            assert(s == L"\r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = Trim<wchar_t>(s, L" sdaD\r");
            CoutString(AddBracket(s));
            assert(s == L"\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\n");

            s = L" \r\n  \n dsa j;fJDK\nOG:Jd   DK;s 'ghg s  \r\ns\r d ";
            s = Trim<wchar_t>(s, L"\n sda\rD");
            CoutString(AddBracket(s));
            assert(s == L"j;fJDK\nOG:Jd   DK;s 'ghg");
        }
#endif
    }

    std::cout << std::endl << "Join----------------:" << std::endl;
    {
        std::vector<std::string> vs;
        vs.push_back("a");
        vs.push_back("b");
        vs.push_back("c");
        std::string s = Join<char>(vs.begin(), vs.end(), "|||");
        CoutString(AddBracket(s));
        assert(s == "a|||b|||c");
        vs.clear();
        s= Join(vs.begin(), vs.end(), std::string(",,,"));
        CoutString(AddBracket(s));
        assert(s == "");

        std::vector<std::wstring> vws;
        vws.push_back(L"a");
        vws.push_back(L"b");
        vws.push_back(L"c");
        std::wstring ws = Join(vws.begin(), vws.end(), std::wstring(L"|||"));
        CoutString(AddBracket(ws));
        assert(ws == L"a|||b|||c");
        vws.clear();
        ws = Join<wchar_t>(vws.begin(), vws.end(), L",,,");
        CoutString(AddBracket(ws));
        assert(ws == L"");
    }

    std::cout << std::endl << "Split----------------:" << std::endl;
    {
        std::string s = "a||b||c";
        std::vector<std::string> vs =  Split<char>(s, "||");
        CoutString(AddBracket(vs.size() + " " + vs[0]
                              + " " + vs[1] + " " + vs[2]));
        assert(vs.size() == 3);
        assert(vs[0] == "a");
        assert(vs[1] == "b");
        assert(vs[2] == "c");

        std::wstring ws = L"a||b||c";
        std::vector<std::wstring> vws =  Split(ws, std::wstring(L"||"));
        CoutString(AddBracket(ws));
        CoutString(AddBracket(vws.size() + L" " + vws[0]
                              + L" " + vws[1] + L" " + vws[2]));
        assert(vws.size() == 3);
        assert(vws[0] == L"a");
        assert(vws[1] == L"b");
        assert(vws[2] == L"c");
    }

    std::cout << std::endl << "EqualsIgnoreCase----------------:" << std::endl;
    {
        assert(EqualsIgnoreCase<char>("aaDddf", "AAddDf"));
        assert(EqualsIgnoreCase<wchar_t>(L"aaDddf", L"AAddDf"));
    }

    std::cout << std::endl << "Format----------------:" << std::endl;
    {
        std::string s = Format("%d", 4);
        CoutString(AddBracket(s));
        assert(s == "4");
        s= Format("%d,%s", 4, "dafkd");
        CoutString(AddBracket(s));
        assert(s == "4,dafkd");
        s= Format("%d,%s", 4, "dafkd");
        CoutString(AddBracket(s));
        assert(s == "4,dafkd");
        try
        {
            s = Format("%d,%s", 4, "dafkd");
//            assert(s == "4,dafkd");
            CoutString(AddBracket(s));
        }
        catch (const std::length_error& e)
        {
            std::cout << e.what() << std::endl;
        }


        std::wstring ws = Format(L"%d", 4);
        CoutString(AddBracket(ws));
        assert(ws == L"4");
#ifdef _WIN32
        ws = Format(L"%d,%s", 4, L"dafkd");
#else
        ws = Format(L"%d,%s", 4, "dafkd");
#endif
        CoutString(AddBracket(ws));
        assert(ws == L"4,dafkd");
#ifdef _WIN32
        ws = Format(L"%d,%s", 4, L"dafkd");
#else
        ws = Format(L"%d,%S", 4, L"dafkd");
#endif
        CoutString(AddBracket(ws));
        assert(ws == L"4,dafkd");
        try
        {
            ws = Format(L"%d,%s", 4, L"dafkd");
//            assert(ws == L"4,dafkd");
            CoutString(AddBracket(ws));
        }
        catch (const std::length_error& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << std::endl << "GetLastPath----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.tiff");
        s = "/tmp//scratch";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch");
        s = "/tmp/";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tmp");
        s = "scratch";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch");
        s = "/";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "\\tmp\\scratch.tiff";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.tiff");
        s = "\\tmp\\\\scratch";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch");
        s = "\\tmp\\";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tmp");
        s = "scratch";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch");
        s = "\\";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "c:\\aaa\\bbb";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "bbb");
        s = "c:\\";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "c:");
        s = "c:";
        s1 = GetLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "c:");
    }
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.tiff");
        s = L"/tmp//scratch";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch");
        s = L"/tmp/";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tmp");
        s = L"scratch";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch");
        s = L"/";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"\\tmp\\scratch.tiff";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.tiff");
        s = L"\\tmp\\\\scratch";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch");
        s = L"\\tmp\\";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tmp");
        s = L"scratch";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch");
        s = L"\\";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"c:\\aaa\\bbb";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"bbb");
        s = L"c:\\";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"c:");
        s = L"c:";
        s1 = GetLastPath<wchar_t>(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"c:");
    }

    std::cout << std::endl << "AppendPath----------------:" << std::endl;
    {
        std::string name = "scratch.tiff";
        std::string s = "/tmp";
        std::string s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "/tmp\\scratch.tiff");
#else
        assert(s1 == "/tmp/scratch.tiff");
#endif
        s = "/tmp/";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/scratch.tiff");
        s = "/";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/scratch.tiff");
        s = "";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.tiff");
        s = "tmp";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "tmp\\scratch.tiff");
#else
        assert(s1 == "tmp/scratch.tiff");
#endif
        s = "//tmp///";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "//tmp///scratch.tiff");
        s = "\\tmp";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "\\tmp\\scratch.tiff");
#else
        assert(s1 == "\\tmp/scratch.tiff");
#endif
        s = "\\tmp";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "\\tmp\\scratch.tiff");
#else
        assert(s1 == "\\tmp/scratch.tiff");
#endif
        s = "\\";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\scratch.tiff");
        s = "";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.tiff");
        s = "tmp";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "tmp\\scratch.tiff");
#else
        assert(s1 == "tmp/scratch.tiff");
#endif
        s = "\\\\tmp\\\\";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\\\tmp\\\\scratch.tiff");
        s = "c:\\aaa\\bbb";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "c:\\aaa\\bbb\\scratch.tiff");
#else
        assert(s1 == "c:\\aaa\\bbb/scratch.tiff");
#endif
        s = "c:\\";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "c:\\scratch.tiff");
        s = "c:";
        s1 = AppendPath<char>(s, name);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == "c:\\scratch.tiff");
#else
        assert(s1 == "c:/scratch.tiff");
#endif
    }
    {
        std::wstring name = L"scratch.tiff";
        std::wstring s = L"/tmp";
        std::wstring s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"/tmp\\scratch.tiff");
#else
        assert(s1 == L"/tmp/scratch.tiff");
#endif
        s = L"/tmp/";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp/scratch.tiff");
        s = L"/";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/scratch.tiff");
        s = L"";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.tiff");
        s = L"tmp";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"tmp\\scratch.tiff");
#else
        assert(s1 == L"tmp/scratch.tiff");
#endif
        s = L"//tmp///";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"//tmp///scratch.tiff");
        s = L"\\tmp";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"\\tmp\\scratch.tiff");
#else
        assert(s1 == L"\\tmp/scratch.tiff");
#endif
        s = L"\\tmp";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"\\tmp\\scratch.tiff");
#else
        assert(s1 == L"\\tmp/scratch.tiff");
#endif
        s = L"\\";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"\\scratch.tiff");
        s = L"";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.tiff");
        s = L"tmp";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"tmp\\scratch.tiff");
#else
        assert(s1 == L"tmp/scratch.tiff");
#endif
        s = L"\\\\tmp\\\\";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"\\\\tmp\\\\scratch.tiff");
        s = L"c:\\aaa\\bbb";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"c:\\aaa\\bbb\\scratch.tiff");
#else
        assert(s1 == L"c:\\aaa\\bbb/scratch.tiff");
#endif
        s = L"c:\\";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"c:\\scratch.tiff");
        s = L"c:";
        s1 = AppendPath<wchar_t>(s, name);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
#ifdef _WIN32
        assert(s1 == L"c:\\scratch.tiff");
#else
        assert(s1 == L"c:/scratch.tiff");
#endif
    }

    std::cout << std::endl << "DeleteLastPath----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp");
        s = "/tmp//lock/";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/");
        s = "/tmp/";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/");
        s = "/tmp";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/");
        s = "/";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/");
        s = "scratch.tiff";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "\\tmp\\scratch.tiff";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\tmp");
        s = "\\tmp\\\\lock\\";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\tmp\\");
        s = "\\tmp\\";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\");
        s = "\\tmp";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\");
        s = "\\";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\");
        s = "scratch.tiff";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "c:\\aaa\\bbb";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "c:\\aaa");
        s = "c:\\";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "c:";
        s1 = DeleteLastPath<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
    }
    {
        std::wstring ws = L"/tmp/scratch.tiff";
        std::wstring ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"/tmp");
        ws = L"/tmp//lock/";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"/tmp/");
        ws = L"/tmp/";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"/");
        ws = L"/tmp";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"/");
        ws = L"/";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"/");
        ws = L"scratch.tiff";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"");
        ws = L"\\tmp\\scratch.tiff";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"\\tmp");
        ws = L"\\tmp\\\\lock\\";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"\\tmp\\");
        ws = L"\\tmp\\";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"\\");
        ws = L"\\tmp";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"\\");
        ws = L"\\";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"\\");
        ws = L"scratch.tiff";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"");
        ws = L"c:\\aaa\\bbb";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"c:\\aaa");
        ws = L"c:\\";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"");
        ws = L"c:";
        ws1 = DeleteLastPath<wchar_t>(ws);
        CoutString(AddBracket(ws) + L"-->" + AddBracket(ws1));
        assert(ws1 == L"");
    }

    std::cout << std::endl << "GetPathExtension----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tiff");
        s = "/tmp//scratch";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/tmp/";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/scratch..tiff";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tiff");
        s = "/scratch.tiff/tmp";
        s1 = GetPathExtension<char>(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/scratch.tiff/tmp";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/scratch.tiff\\tmp";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");

#ifdef _WIN32
        s = "\\scratch.tiff\\tmp";
#else
        s = "/scratch.tiff/tmp";
#endif
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
    }
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tiff");
        s = L"/tmp//scratch";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"/tmp/";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"/scratch..tiff";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tiff");
        s = L"/scratch.tiff/tmp";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"/scratch.tiff/tmp";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"\\scratch.tiff\\tmp";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
#ifdef _WIN32
        s = L"\\scratch.tiff\\tmp";
#else
        s = L"/scratch.tiff/tmp";
#endif
        s1 = GetPathExtension(GetLastPath(s));
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"/";
        s1 = GetPathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
    }

    std::cout << std::endl << "AppendPathExtension----------------:" << std::endl;
    {
        std::string ext = "tiff";
        std::string s = "/tmp/scratch.old";
        std::string s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/scratch.old.tiff");
        s = "/tmp/scratch.";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/scratch..tiff");
        s = "/tmp//";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp//.tiff");
        s = "scratch";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.tiff");
        s = "c:\\a";
        s1 = AppendPathExtension(s, ext);
        std::cout << s << "-->" << s1 << std::endl;
        assert(s1 == "c:\\a.tiff");
    }
    {
        std::wstring ext = L"tiff";
        std::wstring s = L"/tmp/scratch.old";
        std::wstring s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp/scratch.old.tiff");
        s = L"/tmp/scratch.";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp/scratch..tiff");
        s = L"/tmp//";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp//.tiff");
        s = L"scratch";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.tiff");
        s = L"c:\\a";
        s1 = AppendPathExtension(s, ext);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"c:\\a.tiff");
    }

    std::cout << std::endl << "DeletePathExtension----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/scratch");
        s = "\\tmp\\";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "\\tmp\\");
        s = "/tmp/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/tmp/");
        s = "scratch.bundle/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch");
        s = "scratch..tiff";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch.");
        s = ".tiff";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "");
        s = "/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "/");
        s = "scratch...";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scratch..");
        s = "tmp.bundle\\scrtch";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tmp");
        s = "tmp.bundle/scrtch";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "tmp");
#ifdef _WIN32
        s = "tmp.bundle\\scrtch";
#else
        s = "tmp.bundle/scrtch";
#endif
        s1 = DeletePathExtension(GetLastPath(s));
        CoutString(AddBracket(s) + "-->" + AddBracket(s1));
        assert(s1 == "scrtch");
    }
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp/scratch");
        s = L"\\tmp\\";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"\\tmp\\");
        s = L"/tmp/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/tmp/");
        s = L"scratch.bundle/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch");
        s = L"scratch..tiff";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch.");
        s = L".tiff";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"");
        s = L"/";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"/");
        s = L"scratch...";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scratch..");
        s = L"tmp.bundle\\scrtch";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tmp");
        s = L"tmp.bundle/scrtch";
        s1 = DeletePathExtension(s);
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"tmp");
#ifdef _WIN32
        s = L"tmp.bundle\\scrtch";
#else
        s = L"tmp.bundle/scrtch";
#endif
        s1 = DeletePathExtension(GetLastPath(s));
        CoutString(AddBracket(s) + L"-->" + AddBracket(s1));
        assert(s1 == L"scrtch");
    }

    std::cout << std::endl << "FormatCurDateTime----------------:" << std::endl;
    {
        std::string s = "%Y-%m-%dT%H:%M:%S%z";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%Y-%m-%d %H:%M:%S";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%y-%m-%d %H:%M:%S";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%Y-%m-%d";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%H:%M:%S";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%Y%m%d";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%y%m%d";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
        s = "%H%M%S";
        CoutString(AddBracket(s) + "-->" + AddBracket(FormatCurDateTime(s)));
    }
}


int main()
{
    Test();
    return 0;
}
