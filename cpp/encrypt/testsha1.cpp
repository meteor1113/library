#include "sha1.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, SHA1 &sha1)
{
    cout << "SHA1-32(\"" << str << "\") = " << sha1.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return SHA1File(file);
}

void testSHA1()
{
    cout << SHA1String("abc") << endl;
    cout << SHA1File("C:\\boot.ini") << endl;
    cout << SHA1File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    SHA1 sha1;
    sha1.Init();
    sha1.Update("", 0);
    sha1.Update("a", 1);
    sha1.Update("bc", 2);
    sha1.Update("defghijklmnopqrstuvwxyz",
                (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha1.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha1);

    sha1.Init();
    sha1.Update("", 0);
    sha1.Update("a", 1);
    sha1.Update("bc", 2);
    sha1.Update("defghijklmnopqrstuvwxyz",
                (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha1.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha1);

    SHA1 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);
}
