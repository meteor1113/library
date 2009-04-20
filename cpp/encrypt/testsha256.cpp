#include "sha2.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, SHA256 &sha256)
{
    cout << "SHA256-32(\"" << str << "\") = " << sha256.ToString() << endl;
}

static string FileDigest(const string &file)
{
    return SHA256File(file);
}

void testSHA256()
{
    cout << SHA256String("abc") << endl;
    cout << SHA256File("C:\\boot.ini") << endl;
    cout << SHA256File("C:\\a\\a.a") << endl;
    cout << FileDigest("C:\\a\\a.a") << endl;

    SHA256 sha256;
    sha256.Init();
    sha256.Update("", 0);
    sha256.Update("a", 1);
    sha256.Update("bc", 2);
    sha256.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha256.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha256);

    sha256.Init();
    sha256.Update("", 0);
    sha256.Update("a", 1);
    sha256.Update("bc", 2);
    sha256.Update("defghijklmnopqrstuvwxyz",
                  (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    sha256.Final();
    Print("abcdefghijklmnopqrstuvwxyz", sha256);

    SHA256 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
