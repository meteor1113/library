/**
 * Copyright (C) 2008 Meteor Liu
 *
 * This code has been released into the Public Domain.
 * You may do whatever you like with it.
 *
 * @file
 * @author Meteor Liu <meteor1113@gmail.com>
 * @date 2009-01-01
 */


// #include <standard library headers>
#include <vector>
#include <cassert>

// #include <other library headers>

// #include "customer headers"
#include "peresourcehelper.h"


/**
 * description
 */
PEResourceHelper::PEResourceHelper()
{
}


/**
 * description
 */
PEResourceHelper::~PEResourceHelper()
{
}


bool PEResourceHelper::UpdateResString(TCHAR* filename, int index,
                                       wchar_t* newStr)
{
    if (filename == NULL || newStr == NULL)
    {
        return false;
    }

    HMODULE hExe = ::LoadLibrary(filename);
    if (hExe == NULL)
    {
        return false;
    }

    int session = index /16 + 1;
    int idx = index % 16;
    HRSRC hRes = ::FindResource(hExe, MAKEINTRESOURCE(session), RT_STRING);
    std::vector<unsigned char> data;
    if (hRes != 0)
    {
        HGLOBAL hResLoad = ::LoadResource(hExe, hRes);
        if (hResLoad == NULL)
        {
            return false;
        }
        void* lpResLock = ::LockResource(hResLoad);
        if (lpResLock == NULL)
        {
            return false;
        }
        int len = ::SizeofResource(hExe, hRes);
        char* buf = (char*)lpResLock;
        data.insert(data.end(), &buf[0], &buf[len]);
    }
    else
    {
        data.insert(data.end(), 16 * 2, 0);
    }
    FreeLibrary(hExe);

    unsigned int pos = 0;
    for (int i = 0; i <= idx; ++i)
    {
        if (pos >= data.size())  // >= ?
        {
            data.push_back(0);
            data.push_back(0);
        }
        if (i != idx)
        {
            int skip = (ReadWordFromArray(&data[0], pos) + 1) * 2;
            pos += skip;
        }
    }

    std::vector<unsigned char> newStrData;
    WORD newStrLen = (WORD)wcslen(newStr);
    newStrData.resize((newStrLen + 1) * 2);
    WriteWordToArray(&newStrData[0], 0, newStrLen);
    for (int i = 0; i < newStrLen; ++i)
    {
        WriteWordToArray(&newStrData[0], (i + 1) * 2, newStr[i]);
    }

    int skip = (ReadWordFromArray(&data[0], pos) + 1) * 2;
    std::vector<unsigned char> newData;
    newData.insert(newData.end(), &data[0], &data[pos]);
    newData.insert(newData.end(), newStrData.begin(), newStrData.end());
    newData.insert(newData.end(), &data[pos + skip],
                   (&data[data.size() - 1]) + 1);

    HANDLE hUpdateRes = ::BeginUpdateResource(filename, FALSE);
    BOOL result = UpdateResource(hUpdateRes,
                                 RT_STRING,
                                 MAKEINTRESOURCE(session),
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
                                 &newData[0],
                                 (DWORD)newData.size());
    result = EndUpdateResource(hUpdateRes, FALSE);

    return (result == TRUE);
}


WORD PEResourceHelper::ReadWordFromArray(unsigned char* p, int pos)
{
    assert(p != NULL);
    return p[pos] + p[pos +1] * 256;
}


void PEResourceHelper::WriteWordToArray(unsigned char* p, int pos, WORD d)
{
    assert(p != NULL);
    p[pos] = d % 256;
    p[pos +1] = d / 256;
}
