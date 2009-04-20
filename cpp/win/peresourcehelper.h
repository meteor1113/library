/* -*- mode: C++; -*- */
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


#ifndef PERESOURCEHELPER_H
#define PERESOURCEHELPER_H


// #include <standard library headers>

// #include <other library headers>
#include <windows.h>

// #include "customer headers"


/**
 * description
 */
class PEResourceHelper
{
private:
    PEResourceHelper();
    virtual ~PEResourceHelper();

private:
    PEResourceHelper(const PEResourceHelper& rhs);
    PEResourceHelper& operator=(const PEResourceHelper& rhs);

public:
    static bool UpdateResString(TCHAR* filename, int index, TCHAR* newStr);

protected:

private:
    static WORD ReadWordFromArray(unsigned char* p, int pos);
    static void WriteWordToArray(unsigned char* p, int pos, WORD d);

};


#endif
