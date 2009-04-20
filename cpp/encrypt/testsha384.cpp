#include "sha2.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, SHA384 &sha384)
{
    cout << "SHA384-32(\"" << str << "\") = " << sha384.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return SHA384File(file);
}

void testSHA384()
{
    cout << SHA384String("abc") << endl;
    cout << SHA384File("C:\\boot.ini") << endl;
    cout << SHA384File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    SHA384 sha384;
    sha384.Init();
    sha384.Update("", 0);
    sha384.Update("a", 1);
    sha384.Update("bc", 2);
    sha384.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha384.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha384);

    sha384.Init();
    sha384.Update("", 0);
    sha384.Update("a", 1);
    sha384.Update("bc", 2);
    sha384.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha384.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha384);

    SHA384 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);
}
