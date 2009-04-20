#include "crc16.h"
#include <iostream>

using namespace std;
using namespace encrypt;

static void Print(const string &str, CRC16 &crc16)
{
    cout << "CRC16-32(\"" << str << "\") = " << crc16.ToString() << endl;
    cout << "CRC16-32(\"" << str << "\") = " << crc16.GetValue() << endl;
}

void testCRC16()
{
    cout << CRC16String("abc") << endl;
    cout << CRC16File("C:\\boot.ini") << endl;
    cout << CRC16File("C:\\a\\a.a") << endl;

    CRC16 crc16;
    crc16.Init();
    crc16.Update("", 0);
    crc16.Update("a", 1);
    crc16.Update("bc", 2);
    crc16.Update("defghijklmnopqrstuvwxyz",
                 (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    crc16.Final();
    Print("abcdefghijklmnopqrstuvwxyz", crc16);

    crc16.Init();
    crc16.Update("", 0);
    crc16.Update("a", 1);
    crc16.Update("bc", 2);
    crc16.Update("defghijklmnopqrstuvwxyz",
                 (unsigned int)strlen("defghijklmnopqrstuvwxyz"));
    crc16.Final();
    Print("abcdefghijklmnopqrstuvwxyz", crc16);

    CRC16 obj;
    obj.Init();
    obj.Update("abcdefghijklmnopqrstuvwxyz",
               (unsigned int)strlen("abcdefghijklmnopqrstuvwxyz"));
    obj.Final();
    Print("abcdefghijklmnopqrstuvwxyz", obj);

}
