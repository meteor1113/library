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


#ifndef WINDOWSFIREWALL_H
#define WINDOWSFIREWALL_H


#define _WIN32_DCOM


// #include <standard library headers>

// #include <other library headers>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>

// #include "customer headers"


/**
 * description
 */
class WindowsFirewall
{
public:
    WindowsFirewall();
    virtual ~WindowsFirewall();

private:
    WindowsFirewall(const WindowsFirewall& rhs);
    WindowsFirewall& operator=(const WindowsFirewall& rhs);

public:
    void AddApp(const wchar_t* fwProcessImageFileName,
                const wchar_t* fwName, bool enable = true);

protected:

private:
    void Initialize();
    void Cleanup();

private:
    IDispatch* fwMgr_;
    IDispatch* fwPolicy_;
    IDispatch* fwProfile_;
    HRESULT comInit_;

};


#endif
