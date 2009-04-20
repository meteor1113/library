#include "sha2.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, SHA512 &sha512)
{
    cout << "SHA512-32(\"" << str << "\") = " << sha512.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return SHA512File(file);
}

void testSHA512()
{
    cout << SHA512String("abc") << endl;
    cout << SHA512File("C:\\boot.ini") << endl;
    cout << SHA512File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    SHA512 sha512;
    sha512.Init();
    sha512.Update("", 0);
    sha512.Update("a", 1);
    sha512.Update("bc", 2);
    sha512.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha512.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha512);

    sha512.Init();
    sha512.Update("", 0);
    sha512.Update("a", 1);
    sha512.Update("bc", 2);
    sha512.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha512.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha512);

    SHA512 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
