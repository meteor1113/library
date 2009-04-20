#include "md4.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, MD4 &md4)
{
    cout << "MD4-32(\"" << str << "\") = " << md4.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return MD4File(file);
}

void testMD4()
{
    cout << MD4String("abc") << endl;
    cout << MD4File("C:\\boot.ini") << endl;
    cout << MD4File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    MD4 md4;
    md4.Init();
    md4.Update("", 0);
    md4.Update("a", 1);
    md4.Update("bc", 2);
    md4.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md4.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md4);

    md4.Init();
    md4.Update("", 0);
    md4.Update("a", 1);
    md4.Update("bc", 2);
    md4.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md4.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md4);

    MD4 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
