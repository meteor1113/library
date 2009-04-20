#include "crc32.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, CRC32 &crc32)
{
    cout << "CRC32-32(\"" << str << "\") = " << crc32.ToString() << endl;
    cout << "CRC32-32(\"" << str << "\") = " << crc32.GetValue() << endl;
}

void testCRC32()
{
    cout << CRC32String("abc") << endl;
    cout << CRC32File("C:\\boot.ini") << endl;
    cout << CRC32File("C:\\a\\a.a") << endl;

    CRC32 crc32;
    crc32.Init();
    crc32.Update("", 0);
    crc32.Update("a", 1);
    crc32.Update("bc", 2);
    crc32.Update("defghijklmnopqrstuvwxyz",
                 (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    crc32.Final();
    Print("abcdefghijklmnopqrstuvwxyz", crc32);

    crc32.Init();
    crc32.Update("", 0);
    crc32.Update("a", 1);
    crc32.Update("bc", 2);
    crc32.Update("defghijklmnopqrstuvwxyz",
                 (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    crc32.Final();
    Print("abcdefghijklmnopqrstuvwxyz", crc32);

    CRC32 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);
}
