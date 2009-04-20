#include "md5.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, MD5 &md5)
{
    cout << "MD5-32(\"" << str << "\") = " << md5.ToString() << endl;
    cout << "MD5-16(\"" << str << "\") =         " << md5.ToString16() << endl;
}

static string FileDigest(const string &file)
{
    return MD5File(file);
}

void testMD5()
{
    cout << MD5String("abc") << endl;
    cout << MD5String16("abc") << endl;
    cout << MD5File("C:\\boot.ini") << endl;
    cout << MD5File16("C:\\boot.ini") << endl;
    cout << MD5File("C:\\a\\a.a") << endl;
    cout << MD5File16("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    MD5 md5;
    md5.Init();
    md5.Update("", 0);
    md5.Update("a", 1);
    md5.Update("bc", 2);
    md5.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md5.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md5);

    md5.Init();
    md5.Update("", 0);
    md5.Update("a", 1);
    md5.Update("bc", 2);
    md5.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md5.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md5);

    MD5 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
