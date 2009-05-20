#include "teststring.h"

void Test()
{
    std::cout << std::endl << "StartWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = util::string::StartWith(s.c_str(), "JDK");
        bool b2 = util::string::StartWith(s.c_str(), "   dsa ");
        bool b3 = util::string::StartWith(s.c_str(), "   dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        assert(!b1);
        assert(b2);
        assert(b3);
        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = util::string::StartWith(ws.c_str(), L"JDK");
        bool wb2 = util::string::StartWith(ws.c_str(), L"   dsa ");
        bool wb3 = util::string::StartWith(ws.c_str(), L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        assert(!wb1);
        assert(wb2);
        assert(wb3);
    }
    
    std::cout << std::endl << "EndWith----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool b1 = util::string::EndWith(s.c_str(), "'ghg");
        bool b2 = util::string::EndWith(s.c_str(), "ghg    ");
        bool b3 = util::string::EndWith(s.c_str(), ";fJDKOG:Jd   DK;s 'ghg    ");
        assert(!b1);
        assert(b2);
        assert(b3);
        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        bool wb1 = util::string::EndWith(ws.c_str(), L"'ghg");
        bool wb2 = util::string::EndWith(ws.c_str(), L"ghg    ");
        bool wb3 = util::string::EndWith(ws.c_str(), L";fJDKOG:Jd   DK;s 'ghg    ");
        assert(!wb1);
        assert(wb2);
        assert(wb3);
    }
    
    std::cout << std::endl << "ToUpper----------------:" << std::endl;
    {
        std::string s = "   JDKhg;'    ";
        s= util::string::ToUpper(s.c_str());
        std::string ss = util::string::ToUpper("adfds");
        assert(s == "   JDKHG;'    ");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;'    ";
        ws = util::string::ToUpper(ws.c_str());
        assert(ws == L"   JDKHG;'    ");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "ToLower----------------:" << std::endl;
    {
        std::string s = "   JDKhg;'    ";
        s= util::string::ToLower(s.c_str());
        assert(s == "   jdkhg;'    ");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;'    ";
        ws = util::string::ToLower(ws.c_str());
        assert(ws == L"   jdkhg;'    ");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "Replace----------------:" << std::endl;
    {
        std::string s = "   JDKhg;''    '";
        s= util::string::Replace(s.c_str(), "'", "''");
        assert(s == "   JDKhg;''''    ''");
        std::cout << s << std::endl;
        s= util::string::Replace(s.c_str(), "'", "|");
        assert(s == "   JDKhg;||||    ||");
        std::cout << s << std::endl;
        
        std::wstring ws = L"   JDKhg;''    '";
        ws = util::string::Replace(ws.c_str(), L"'", L"''");
        assert(ws == L"   JDKhg;''''    ''");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        ws = util::string::Replace(ws.c_str(), L"'", L"|");
        assert(ws == L"   JDKhg;||||    ||");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "TrimLeft----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= util::string::TrimLeft(s.c_str());
        std::cout << s << std::endl;
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;
#ifndef __MINGW32__
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws = util::string::TrimLeft(ws.c_str());
        assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg    ");
        std::wcout << ws << std::endl;
        std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "TrimRight----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= util::string::TrimRight(s.c_str());
        assert(s == "   dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::cout << s << std::endl;
        std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;
#ifndef __MINGW32__        
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws = util::string::TrimRight(ws.c_str());
        assert(ws == L"   dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::wcout << ws << std::endl;
        std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "Trim----------------:" << std::endl;
    {
        std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        s= util::string::Trim(s.c_str());
        assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::cout << s << std::endl;
        std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;
#ifndef __MINGW32__
        std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
        ws  = util::string::Trim(ws.c_str());
        assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg");
        std::wcout << ws << std::endl;
        std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
#endif
    }
    
    std::cout << std::endl << "Join----------------:" << std::endl;
    {
        std::vector<std::string> vs;
        vs.push_back("a");
        vs.push_back("b");
        vs.push_back("c");
        std::string s = util::string::Join(vs.begin(), vs.end(), "|||");
        assert(s == "a|||b|||c");
        std::cout << s << std::endl;
        vs.clear();
        s= util::string::Join(vs.begin(), vs.end(), ",,,");
        assert(s == "");
        std::cout << s << std::endl;
        
        std::vector<std::wstring> vws;
        vws.push_back(L"a");
        vws.push_back(L"b");
        vws.push_back(L"c");
        std::wstring ws = util::string::Join(vws.begin(), vws.end(), L"|||");
        assert(ws == L"a|||b|||c");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        vws.clear();
        ws = util::string::Join(vws.begin(), vws.end(), L",,,");
        assert(ws == L"");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
    }
    
    std::cout << std::endl << "Split----------------:" << std::endl;
    {
        std::string s = "a||b||c";
        std::vector<std::string> vs =  util::string::Split(s.c_str(), "||");
        assert(vs.size() == 3);
        assert(vs[0] == "a");
        assert(vs[1] == "b");
        assert(vs[2] == "c");
        std::cout << vs.size() << " "<< vs[0] << " " << vs[1] << " "<< vs[2] << std::endl;
        
        std::wstring ws = L"a||b||c";
        std::vector<std::wstring> vws =  util::string::Split(ws.c_str(), L"||");
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
        assert(util::string::EqualsIgnoreCase("aaDddf", "AAddDf"));
        assert(util::string::EqualsIgnoreCase(L"aaDddf", L"AAddDf"));
        std::cout << "EqualsIgnoreCase end" << std::endl;
    }
    
    std::cout << std::endl << "Format----------------:" << std::endl;
    {
        std::string s = util::string::Format("%d", 4);
        assert(s == "4");
        std::cout << s << std::endl;
        s= util::string::Format("%d,%s", 4, "dafkd");
        assert(s == "4,dafkd");
        std::cout << s << std::endl;
        s= util::string::Format<2000>("%d,%s", 4, "dafkd");
        assert(s == "4,dafkd");
        std::cout << s << std::endl;
        try
        {
            s = util::string::Format<2>("%d,%s", 4, "dafkd");
//            assert(s == "4,dafkd");
            std::cout << s << std::endl;
        }
        catch (const std::length_error& e)
        {
            std::cout << e.what() << std::endl;
        }
        
        
        std::wstring ws = util::string::Format(L"%d", 4);
        assert(ws == L"4");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        ws = util::string::Format(L"%d,%s", 4, L"dafkd");
        assert(ws == L"4,dafkd");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        ws = util::string::Format<2000>(L"%d,%s", 4, L"dafkd");
        assert(ws == L"4,dafkd");
#ifndef __MINGW32__
        std::wcout << ws << std::endl;
#endif
        try
        {
            ws = util::string::Format<3>(L"%d,%s", 4, L"dafkd");
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
}
        
        
int main()
{
    Test();
    return 0;
}
