#include "md2.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, MD2 &md2)
{
    cout << "MD2-32(\"" << str << "\") = " << md2.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return MD2File(file);
}

void testMD2()
{
    cout << MD2String("abc") << endl;
    cout << MD2File("C:\\boot.ini") << endl;
    cout << MD2File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    MD2 md2;
    md2.Init();
    md2.Update("", 0);
    md2.Update("a", 1);
    md2.Update("bc", 2);
    md2.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md2.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md2);

    md2.Init();
    md2.Update("", 0);
    md2.Update("a", 1);
    md2.Update("bc", 2);
    md2.Update("defghijklmnopqrstuvwxyz",
               (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    md2.Final();
    Print("abcdefghijklmnopqrstuvwxyz", md2);

    MD2 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
