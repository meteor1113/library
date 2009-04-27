#include "teststring.h"

namespace util
{
    namespace string
    {
        void Test()
        {
            // Cut
            //{
            //    std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            //    util::string::Cut(s, "JDK");
            //    assert(s == "   dsa j;f");
            //    std::cout << s << std::endl;

            //    std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
            //    util::string::Cut(ws, L"JDK");
            //    assert(ws == L"   dsa j;f");
            //    std::wcout << ws << std::endl;
            //}


            // StartWith
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


            // EndWith
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


            // ToUpper
            {
                std::string s = "   JDKhg;'    ";
                s= util::string::ToUpper(s.c_str());
                std::string ss = util::string::ToUpper("adfds");
                assert(s == "   JDKHG;'    ");
                std::cout << s << std::endl;

                std::wstring ws = L"   JDKhg;'    ";
                ws = util::string::ToUpper(ws.c_str());
                assert(ws == L"   JDKHG;'    ");
                std::wcout << ws << std::endl;
            }


            // ToLower
            {
                std::string s = "   JDKhg;'    ";
                s= util::string::ToLower(s.c_str());
                assert(s == "   jdkhg;'    ");
                std::cout << s << std::endl;

                std::wstring ws = L"   JDKhg;'    ";
                ws = util::string::ToLower(ws.c_str());
                assert(ws == L"   jdkhg;'    ");
                std::wcout << ws << std::endl;
            }


            // Replace
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
                std::wcout << ws << std::endl;
                ws = util::string::Replace(ws.c_str(), L"'", L"|");
                assert(ws == L"   JDKhg;||||    ||");
                std::wcout << ws << std::endl;
            }


            // TrimLeft
            {
                std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                s= util::string::TrimLeft(s.c_str());
                std::cout << s << std::endl;
                assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg    ");
                std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;

                std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                ws = util::string::TrimLeft(ws.c_str());
                assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg    ");
                std::wcout << ws << std::endl;
                std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
            }


            // TrimRight
            {
                std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                s= util::string::TrimRight(s.c_str());
                assert(s == "   dsa j;fJDKOG:Jd   DK;s 'ghg");
                std::cout << s << std::endl;
                std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;

                std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                ws = util::string::TrimRight(ws.c_str());
                assert(ws == L"   dsa j;fJDKOG:Jd   DK;s 'ghg");
                std::wcout << ws << std::endl;
                std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
            }


            // Trim
            {
                std::string s = "   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                s= util::string::Trim(s.c_str());
                assert(s == "dsa j;fJDKOG:Jd   DK;s 'ghg");
                std::cout << s << std::endl;
                std::cout << util::string::Replace(s.c_str(), " ", "-") << std::endl;

                std::wstring ws = L"   dsa j;fJDKOG:Jd   DK;s 'ghg    ";
                ws  = util::string::Trim(ws.c_str());
                assert(ws == L"dsa j;fJDKOG:Jd   DK;s 'ghg");
                std::wcout << ws << std::endl;
                std::wcout << util::string::Replace(ws.c_str(), L" ", L"-") << std::endl;
            }


            // Join
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
                std::wcout << ws << std::endl;
                vws.clear();
                ws = util::string::Join(vws.begin(), vws.end(), L",,,");
                assert(ws == L"");
                std::wcout << ws << std::endl;
            }


            // Split
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
                std::wcout << vws.size() << L" "<< vws[0] << L" " << vws[1] << L" "<< vws[2] << std::endl;
            }


            // EqualsIgnoreCase
            {
                std::cout << "EqualsIgnoreCase start" << std::endl;
                assert(util::string::EqualsIgnoreCase("aaDddf", "AAddDf"));
                assert(util::string::EqualsIgnoreCase(L"aaDddf", L"AAddDf"));
                std::cout << "EqualsIgnoreCase end" << std::endl;
            }


            // Format
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
                    assert(s == "4,dafkd");
                    std::cout << s << std::endl;
                }
                catch (const std::length_error& e)
                {
                    std::cout << e.what() << std::endl;
                }


                std::wstring ws = util::string::Format(L"%d", 4);
                assert(ws == L"4");
                std::wcout << ws << std::endl;
                ws = util::string::Format(L"%d,%s", 4, L"dafkd");
                assert(ws == L"4,dafkd");
                std::wcout << ws << std::endl;
                ws = util::string::Format<2000>(L"%d,%s", 4, L"dafkd");
                assert(ws == L"4,dafkd");
                std::wcout << ws << std::endl;
                try
                {
                    ws = util::string::Format<3>(L"%d,%s", 4, L"dafkd");
                    assert(ws == L"4,dafkd");
                    std::wcout << ws << std::endl;
                }
                catch (const std::length_error& e)
                {
                    std::cout << e.what() << std::endl;
                }
            }

        }

    }

}

int main()
{
    util::string::Test();
    return 0;
}
