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


#ifndef NTSERVICE_H
#define NTSERVICE_H


// #include <standard library headers>
#include <cstdio>
#include <string>

// #include <other library headers>
#include <windows.h>
#include <tchar.h>

// #include "customer headers"


#define SERVICE_CONTROL_USER 128


namespace
{
#ifdef _UNICODE
    typedef std::wstring TString;
#else
    typedef std::string TString;
#endif
}


/**
 * description
 */
class NTService
{
public:
    NTService(const TCHAR* serviceName);
    virtual ~NTService();

public:
    bool ParseStandardArgs(int argc, TCHAR* argv[]);
    bool IsInstalled();
    virtual bool Install();
    virtual bool InstallAs(DWORD startType, TCHAR* path = NULL);
    virtual bool Uninstall();
    virtual bool StartService();
    virtual void SetStatus(DWORD state);
    virtual void Run();
    virtual bool OnInit();
    virtual bool OnUninit();
    virtual void OnStop();
    virtual void OnInterrogate();
    virtual void OnPause();
    virtual void OnContinue();
    virtual void OnShutdown();
    virtual void OnDeviceEvent(DWORD type, LPVOID data, LPVOID context);
    virtual void OnHardwareProfileChange(DWORD type, LPVOID data,
                                         LPVOID context);
    virtual void OnPowerEvent(DWORD type, LPVOID data, LPVOID context);
    virtual void OnSessionChange(DWORD type, LPVOID data, LPVOID context);
    virtual bool OnUserControl(DWORD dwControl);
    void DebugMsg(const TCHAR* pszFormat, ...);
    // static member functions
    static void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);
    static DWORD WINAPI HandlerEx(DWORD ctrl, DWORD type, LPVOID data,
                                  LPVOID context);
    void LogEvent(LPCTSTR format, ...) throw();
    void LogEventEx(int id, LPCTSTR pszMessage = NULL,
                    WORD type = EVENTLOG_INFORMATION_TYPE) throw();
    const SERVICE_STATUS& GetServiceStatus() const { return mStatus; }
    virtual bool MakeServiceStart();
    virtual bool MakeServiceStop();
    TCHAR* GetServiceName() { return mServiceName; }

public:
    bool IsInstalled(const TCHAR* serviceName);
    static bool MakeServiceStart(const TCHAR* serviceName);
    static bool MakeServiceStop(const TCHAR* serviceName);
    static bool ChangeServiceConfig(const TCHAR* serviceName,
                                    DWORD serviceType = SERVICE_NO_CHANGE,
                                    DWORD startType = SERVICE_NO_CHANGE,
                                    DWORD errorControl = SERVICE_NO_CHANGE,
                                    const TCHAR* const binaryPathName = NULL,
                                    const TCHAR* const loadOrderGroup = NULL,
                                    LPDWORD tagId = NULL,
                                    const TCHAR* const dependencies = NULL,
                                    const TCHAR* const serviceStartName = NULL,
                                    const TCHAR* const password = NULL,
                                    const TCHAR* const displayName = NULL);
    static TString GetServicePath(const TCHAR* serviceName);
    static void LogEventEx(const TCHAR* serviceName, int id,
                           LPCTSTR pszMessage = NULL,
                           WORD type = EVENTLOG_INFORMATION_TYPE) throw();


protected:
    TCHAR mServiceName[64];
    int mMajorVersion;
    int mMinorVersion;
    bool mIsRunning;
    bool mIsService;
    SERVICE_STATUS_HANDLE mServiceStatus;
    SERVICE_STATUS mStatus;
    // static data
    static NTService* mThis;    // nasty hack to get object ptr

};


#endif
