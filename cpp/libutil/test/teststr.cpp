#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#include <iostream>

#include "str.hpp"


void Test()
{
    std::cout << std::endl << "StartWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = str::StartWith<char>(s, "JDK");
        bool b2 = str::StartWith(s, std::string("   dsa "));
        bool b3 = str::StartWith<char>(s, "   dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        assert(!b1);
        assert(b2);
        assert(b3);
        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = str::StartWith<wchar_t>(ws, L"JDK");
        bool wb2 = str::StartWith<wchar_t>(ws, L"   dsa ");
        bool wb3 = str::StartWith(ws, std::wstring(L"   dsa j;fJDKOG:Jd   DK;s 'ghg    "));
        assert(!wb1);
        assert(wb2);
        assert(wb3);
    }
    
    std::cout << std::endl << "EndWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = str::EndWith<char>(s, "'ghg");
        bool b2 = str::EndWith<char>(s, "ghg    ");
        bool b3 = str::EndWith<char>(s, ";fJDKOG:Jd   DK;s 'ghg    ");
        bool b4 = str::EndWith<char>("", "a");
        assert(!b1);
        assert(b2);
        assert(b3);
        assert(!b4);
        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = str::EndWith<wchar_t>(ws, L"'ghg");
        bool wb2 = str::EndWith<wchar_t>(ws, L"ghg    ");
        bool wb3 = str::EndWith<wchar_t>(ws, L";fJDKOG:Jd   DK;s 'ghg    ");
        bool wb4 = str::EndWith<wchar_t>(L"", L"a");
        assert(!wb1);
        assert(wb2);
        assert(wb3);
        assert(!wb4);
    }
    
    std::cout << std::endl << "ToUpper----------------:" << std::endl;
    {
        std::string s = "   JDKhg;i'    ";
        s= str::ToUpper<char>(s);
        assert(s == "   JDKHG;I'    ");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;i'    ";
        ws = str::ToUpper<wchar_t>(ws);
        assert(ws == L"   JDKHG;I'    ");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "ToLower----------------:" << std::endl;
    {
        std::string s = "   JDKhg;I'    ";
        s= str::ToLower<char>(s);
        assert(s == "   jdkhg;i'    ");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;I'    ";
        ws = str::ToLower<wchar_t>(ws);
        assert(ws == L"   jdkhg;i'    ");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "Replace----------------:" << std::endl;
    {
        std::string s = "   JDKhg;''    '";
        s= str::Replace<char>(s, "'", "''");
        assert(s == "   JDKhg;''''    ''");
        std::cout << s << std::endl;
        s= str::Replace<char>(s, "'", "|");
        assert(s == "   JDKhg;||||    ||");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;''    '";
        ws = str::Replace<wchar_t>(ws, L"'", L"''");
        assert(ws == L"   JDKhg;''''    ''");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        ws = str::Replace<wchar_t>(ws, L"'", L"|");
        assert(ws == L"   JDKhg;||||    ||");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "TrimLeft----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= str::TrimLeft<char>(s);
        std::cout << s << std::endl;
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        std::cout << str::Replace<char>(s, " ", "-") << std::endl;
#ifndef __MINGW32__
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws = str::TrimLeft<wchar_t>(ws.c_str());
        assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        std::wcout << ws << std::endl;
        std::wcout << str::Replace<wchar_t>(ws, L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "TrimRight----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= str::TrimRight(s);
        assert(s == "   dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::cout << s << std::endl;
        std::cout << str::Replace<char>(s, " ", "-") << std::endl;
#ifndef __MINGW32__        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws = str::TrimRight<wchar_t>(ws.c_str());
        assert(ws == L"   dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::wcout << ws << std::endl;
        std::wcout << str::Replace<wchar_t>(ws, L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "Trim----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= str::Trim(s);
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::cout << s << std::endl;
        std::cout << str::Replace<char>(s, " ", "-") << std::endl;
#ifndef __MINGW32__
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws  = str::Trim(ws);
        assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::wcout << ws << std::endl;
        std::wcout << str::Replace<wchar_t>(ws, L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "Join----------------:" << std::endl;
    {
        std::vector<std::string> vs;
        vs.push_back("a");
        vs.push_back("b");
        vs.push_back("c");
        std::string s = str::Join<char>(vs.begin(), vs.end(), "|||");
        assert(s == "a|||b|||c");
        std::cout << s << std::endl;
        vs.clear();
        s= str::Join(vs.begin(), vs.end(), std::string(",,,"));
        assert(s == "");
        std::cout << s << std::endl;
        
        std::vector<std::wstring> vws;
        vws.push_back(L"a");
        vws.push_back(L"b");
        vws.push_back(L"c");
        std::wstring ws = str::Join(vws.begin(), vws.end(), std::wstring(L"|||"));
        assert(ws == L"a|||b|||c");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        vws.clear();
        ws = str::Join<wchar_t>(vws.begin(), vws.end(), L",,,");
        assert(ws == L"");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "Split----------------:" << std::endl;
    {
        std::string s = "a||b||c";
        std::vector<std::string> vs =  str::Split<char>(s, "||");
        assert(vs.size() == 3);
        assert(vs[0] == "a");
        assert(vs[1] == "b");
        assert(vs[2] == "c");
        std::cout << vs.size() << " "<< vs[0] << " " << vs[1] << " "<< vs[2] << std::endl;
        
        std::wstring ws = L"a||b||c";
        std::vector<std::wstring> vws =  str::Split(ws, std::wstring(L"||"));
        assert(vws.size() == 3);
        assert(vws[0] == L"a");
        assert(vws[1] == L"b");
        assert(vws[2] == L"c");
#ifndef __MINGW32__
        std::wcout << vws.size() << L" "<< vws[0] << L" " << vws[1] << L" "<< vws[2] << std::endl;
#endif
    }
    
    std::cout << std::endl << "EqualsIgnoreCase----------------:" << std::endl;
    {
        std::cout << "EqualsIgnoreCase start" << std::endl;
        assert(str::EqualsIgnoreCase<char>("aaDddf", "AAddDf"));
        assert(str::EqualsIgnoreCase<wchar_t>(L"aaDddf", L"AAddDf"));
        std::cout << "EqualsIgnoreCase end" << std::endl;
    }
    
    std::cout << std::endl << "Format----------------:" << std::endl;
    {
        std::string s = str::Format("%d", 4);
        std::cout << s << std::endl;
        assert(s == "4");
        s= str::Format("%d,%s", 4, "dafkd");
        std::cout << s << std::endl;
        assert(s == "4,dafkd");
        s= str::Format<2000>("%d,%s", 4, "dafkd");
        std::cout << s << std::endl;
        assert(s == "4,dafkd");
        try
        {
            s = str::Format<2>("%d,%s", 4, "dafkd");
//            assert(s == "4,dafkd");
            std::cout << s << std::endl;
        }
        catch (const std::length_error& e)
        {
            std::cout << e.what() << std::endl;
        }
        
        
        std::wstring ws = str::Format(L"%d", 4);
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        assert(ws == L"4");
#ifdef _WIN32
        ws = str::Format(L"%d,%s", 4, L"dafkd");
#else
        ws = str::Format(L"%d,%s", 4, "dafkd");
#endif
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        assert(ws == L"4,dafkd");
#ifdef _WIN32
        ws = str::Format<2000>(L"%d,%s", 4, L"dafkd");
#else
        ws = str::Format<2000>(L"%d,%S", 4, L"dafkd");
#endif
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        assert(ws == L"4,dafkd");
        try
        {
            ws = str::Format<3>(L"%d,%s", 4, L"dafkd");
//            assert(ws == L"4,dafkd");
#ifndef __MINGW32__
            std::wcout << ws << std::endl;
#endif
        }
        catch (const std::length_error& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << std::endl << "GetLastPath----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = str::GetLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.tiff");
        s = "/tmp//scratch";
        s1 = str::GetLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch");
        s = "/tmp/";
        s1 = str::GetLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp");
        s = "scratch";
        s1 = str::GetLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch");
        s = "/";
        s1 = str::GetLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "\\tmp\\scratch.tiff";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.tiff");
        s = "\\tmp\\\\scratch";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch");
        s = "\\tmp\\";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp");
        s = "scratch";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch");
        s = "\\";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "c:\\aaa\\bbb";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "bbb");
        s = "c:\\";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:");
        s = "c:";
        s1 = str::GetLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:");
    }
#ifndef __MINGW32__
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = str::GetLastPath<wchar_t>(s, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.tiff");
        s = L"/tmp//scratch";
        s1 = str::GetLastPath<wchar_t>(s, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch");
        s = L"/tmp/";
        s1 = str::GetLastPath<wchar_t>(s, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp");
        s = L"scratch";
        s1 = str::GetLastPath<wchar_t>(s, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch");
        s = L"/";
        s1 = str::GetLastPath<wchar_t>(s, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"\\tmp\\scratch.tiff";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.tiff");
        s = L"\\tmp\\\\scratch";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch");
        s = L"\\tmp\\";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp");
        s = L"scratch";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch");
        s = L"\\";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"c:\\aaa\\bbb";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"bbb");
        s = L"c:\\";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:");
        s = L"c:";
        s1 = str::GetLastPath<wchar_t>(s, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:");
    }
#endif

    std::cout << std::endl << "AppendPath----------------:" << std::endl;
    {
        std::string name = "scratch.tiff";
        std::string s = "/tmp";
        std::string s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/scratch.tiff");
        s = "/tmp/";
        s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/scratch.tiff");
        s = "/";
        s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/scratch.tiff");
        s = "";
        s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.tiff");
        s = "tmp";
        s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp/scratch.tiff");
        s = "//tmp///";
        s1 = str::AppendPath<char>(s, name, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "//tmp///scratch.tiff");
        s = "\\tmp";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\tmp\\scratch.tiff");
        s = "\\tmp";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\tmp\\scratch.tiff");
        s = "\\";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\scratch.tiff");
        s = "";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.tiff");
        s = "tmp";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp\\scratch.tiff");
        s = "\\\\tmp\\\\";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\\\tmp\\\\scratch.tiff");
        s = "c:\\aaa\\bbb";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:\\aaa\\bbb\\scratch.tiff");
        s = "c:\\";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:\\scratch.tiff");
        s = "c:";
        s1 = str::AppendPath<char>(s, name, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:\\scratch.tiff");
    }
#ifndef __MINGW32__
    {
        std::wstring name = L"scratch.tiff";
        std::wstring s = L"/tmp";
        std::wstring s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/scratch.tiff");
        s = L"/tmp/";
        s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/scratch.tiff");
        s = L"/";
        s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/scratch.tiff");
        s = L"";
        s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.tiff");
        s = L"tmp";
        s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp/scratch.tiff");
        s = L"//tmp///";
        s1 = str::AppendPath<wchar_t>(s, name, L"/");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"//tmp///scratch.tiff");
        s = L"\\tmp";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"\\tmp\\scratch.tiff");
        s = L"\\tmp";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"\\tmp\\scratch.tiff");
        s = L"\\";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"\\scratch.tiff");
        s = L"";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.tiff");
        s = L"tmp";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp\\scratch.tiff");
        s = L"\\\\tmp\\\\";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"\\\\tmp\\\\scratch.tiff");
        s = L"c:\\aaa\\bbb";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:\\aaa\\bbb\\scratch.tiff");
        s = L"c:\\";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:\\scratch.tiff");
        s = L"c:";
        s1 = str::AppendPath<wchar_t>(s, name, L"\\");
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:\\scratch.tiff");
    }
#endif

    std::cout << std::endl << "DeleteLastPath----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp");
        s = "/tmp//lock/";
        s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/");
        s = "/tmp/";
        s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/");
        s = "/tmp";
        s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/");
        s = "/";
        s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/");
        s = "scratch.tiff";
        s1 = str::DeleteLastPath<char>(s, "/");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "\\tmp\\scratch.tiff";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\tmp");
        s = "\\tmp\\\\lock\\";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\tmp\\");
        s = "\\tmp\\";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\");
        s = "\\tmp";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\");
        s = "\\";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\");
        s = "scratch.tiff";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "c:\\aaa\\bbb";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:\\aaa");
        s = "c:\\";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "c:";
        s1 = str::DeleteLastPath<char>(s, "\\");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
    }
#ifndef __MINGW32__
    {
        std::wstring ws = L"/tmp/scratch.tiff";
        std::wstring ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"/tmp");
        ws = L"/tmp//lock/";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"/tmp/");
        ws = L"/tmp/";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"/");
        ws = L"/tmp";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"/");
        ws = L"/";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"/");
        ws = L"scratch.tiff";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"/");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"");
        ws = L"\\tmp\\scratch.tiff";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"\\tmp");
        ws = L"\\tmp\\\\lock\\";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"\\tmp\\");
        ws = L"\\tmp\\";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"\\");
        ws = L"\\tmp";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"\\");
        ws = L"\\";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"\\");
        ws = L"scratch.tiff";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"");
        ws = L"c:\\aaa\\bbb";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"c:\\aaa");
        ws = L"c:\\";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"");
        ws = L"c:";
        ws1 = str::DeleteLastPath<wchar_t>(ws, L"\\");
        std::wcout << ws << L" --> " << ws1 << std::endl;
        assert(ws1 == L"");
    }
#endif

    std::cout << std::endl << "GetPathExtension----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tiff");
        s = "/tmp//scratch";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "/tmp/";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "/scratch..tiff";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tiff");
        s = "/scratch.tiff/tmp";
        s1 = str::GetPathExtension<char>(s, ".");
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tiff/tmp");
        s = "/scratch.tiff/tmp";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tiff/tmp");
        s = "/scratch.tiff\\tmp";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tiff\\tmp");

#ifdef _WIN32
        s = "\\scratch.tiff\\tmp";
#else
        s = "/scratch.tiff/tmp";
#endif
        s1 = str::GetPathExtension(str::GetLastPath(s.c_str()).c_str());
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "/";
        s1 = str::GetPathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
    }
#ifndef __MINGW32__
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tiff");
        s = L"/tmp//scratch";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"/tmp/";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"/scratch..tiff";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tiff");
        s = L"/scratch.tiff/tmp";
        s1 = str::GetPathExtension(s, std::wstring(L"."));
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tiff/tmp");
        s = L"/scratch.tiff/tmp";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tiff/tmp");
        s = L"\\scratch.tiff\\tmp";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tiff\\tmp");
#ifdef _WIN32
        s = L"\\scratch.tiff\\tmp";
#else
        s = L"/scratch.tiff/tmp";
#endif
        s1 = str::GetPathExtension(str::GetLastPath(s));
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"/";
        s1 = str::GetPathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
    }
#endif

    std::cout << std::endl << "AppendPathExtension----------------:" << std::endl;
    {
        std::string ext = "tiff";
        std::string s = "/tmp/scratch.old";
        std::string s1 = str::AppendPathExtension(s, ext);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/scratch.old.tiff");
        s = "/tmp/scratch.";
        s1 = str::AppendPathExtension(s, ext);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/scratch..tiff");
        s = "/tmp//";
        s1 = str::AppendPathExtension(s, ext);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp//.tiff");
        s = "scratch";
        s1 = str::AppendPathExtension(s, ext);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.tiff");
        s = "c:\\a";
        s1 = str::AppendPathExtension(s, ext);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "c:\\a.tiff");
    }
#ifndef __MINGW32__
    {
        std::wstring ext = L"tiff";
        std::wstring s = L"/tmp/scratch.old";
        std::wstring s1 = str::AppendPathExtension(s, ext);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/scratch.old.tiff");
        s = L"/tmp/scratch.";
        s1 = str::AppendPathExtension(s.c_str(), ext);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/scratch..tiff");
        s = L"/tmp//";
        s1 = str::AppendPathExtension(s, ext);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp//.tiff");
        s = L"scratch";
        s1 = str::AppendPathExtension(s, ext);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.tiff");
        s = L"c:\\a";
        s1 = str::AppendPathExtension(s, ext);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"c:\\a.tiff");
    }
#endif

    std::cout << std::endl << "DeletePathExtension----------------:" << std::endl;
    {
        std::string s = "/tmp/scratch.tiff";
        std::string s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/scratch");
        s = "\\tmp\\";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "\\tmp\\");
        s = "/tmp/";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/tmp/");
        s = "scratch.bundle/";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch");
        s = "scratch..tiff";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch.");
        s = ".tiff";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "");
        s = "/";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "/");
        s = "scratch...";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scratch..");
        s = "tmp.bundle\\scrtch";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp");
        s = "tmp.bundle/scrtch";
        s1 = str::DeletePathExtension(s);
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "tmp");
#ifdef _WIN32
        s = "tmp.bundle\\scrtch";
#else
        s = "tmp.bundle/scrtch";
#endif
        s1 = str::DeletePathExtension(str::GetLastPath(s));
        std::cout << s << " --> " << s1 << std::endl;
        assert(s1 == "scrtch");
    }
#ifndef __MINGW32__
    {
        std::wstring s = L"/tmp/scratch.tiff";
        std::wstring s1 = str::DeletePathExtension(s.c_str());
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/scratch");
        s = L"\\tmp\\";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"\\tmp\\");
        s = L"/tmp/";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/tmp/");
        s = L"scratch.bundle/";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch");
        s = L"scratch..tiff";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch.");
        s = L".tiff";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"");
        s = L"/";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"/");
        s = L"scratch...";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scratch..");
        s = L"tmp.bundle\\scrtch";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp");
        s = L"tmp.bundle/scrtch";
        s1 = str::DeletePathExtension(s);
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"tmp");
#ifdef _WIN32
        s = L"tmp.bundle\\scrtch";
#else
        s = L"tmp.bundle/scrtch";
#endif
        s1 = str::DeletePathExtension(str::GetLastPath(s));
        std::wcout << s << L" --> " << s1 << std::endl;
        assert(s1 == L"scrtch");
    }
#endif
}
        
        
int main()
{
    Test();
    return 0;
}
