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
#include <cassert>

// #include <other library headers>

// #include "customer headers"
#include "ntservice.h"


#pragma warning(push)
#pragma warning(disable:4996) // crt declared deprecated


// static variables
NTService* NTService::mThis = NULL;


/**
 * description
 */
NTService::NTService(const TCHAR* serviceName)
{
    // copy the address of the current object so we can access it from
    // the static member callback functions.
    // WARNING: This limits the application to only one NTService object.

    mThis = this;
    // Set the default service name and version
    _tcsncpy(mServiceName,
             serviceName,
             sizeof(mServiceName) / sizeof(TCHAR) - 1);
    mMajorVersion = 1;
    mMinorVersion = 0;

    // set up the initial service status
    mServiceStatus = NULL;
    mStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    mStatus.dwCurrentState = SERVICE_STOPPED;
    mStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    mStatus.dwWin32ExitCode = 0;
    mStatus.dwServiceSpecificExitCode = 0;
    mStatus.dwCheckPoint = 0;
    mStatus.dwWaitHint = 0;
    mIsRunning = false;
    mIsService = false;
}


/**
 * description
 */
NTService::~NTService()
{
    DebugMsg(_T("NTService::~NTService()"));
}


/**
 * Default command line argument parsing
 * Returns true if it found an arg it recognised, false if not
 * Note: processing some arguments causes output to stdout to be generated.
 */
bool NTService::ParseStandardArgs(int argc, TCHAR* argv[])
{
    // See if we have any command line args we recognise
    if (argc <= 1)
    {
        return false;
    }

    if (_tcsicmp(argv[1], _T("-h")) == 0
        || _tcsicmp(argv[1], _T("-?")) == 0
        || _tcsicmp(argv[1], _T("/h")) == 0
        || _tcsicmp(argv[1], _T("/?")) == 0)
    {
        _tprintf(_T("Usage: %s [-i] [-u] [-v] [-h]\n\t-i\tinstall service.\n\t-u\tuninstall service.\n\t-v\tprint version.\n\t-h\tprint this help.\n"),
                 argv[0]);
        return true;
    }
    else if (_tcsicmp(argv[1], _T("-v")) == 0
             || _tcsicmp(argv[1], _T("/v")) == 0)
    {
        // Spit out version info
        _tprintf(_T("%s Version %d.%d\n"),
                 mServiceName,
                 mMajorVersion,
                 mMinorVersion);
        _tprintf(_T("The service is %s installed\n"),
                 IsInstalled() ? _T("currently") : _T("not"));
        return true; // say we processed the argument
    }
    else if (_tcsicmp(argv[1], _T("-i")) == 0
             || _tcsicmp(argv[1], _T("/i")) == 0)
    {
        // Request to install.
        if (IsInstalled())
        {
            _tprintf(_T("%s is already installed\n"), mServiceName);
        }
        else
        {
            // Try and install the copy that's running
            if (Install())
            {
                _tprintf(_T("%s installed\n"), mServiceName);
            }
            else
            {
                _tprintf(_T("%s failed to install. Error %d\n"),
                         mServiceName,
                         GetLastError());
            }
        }
        return true; // say we processed the argument
    }
    else if (_tcsicmp(argv[1], _T("-u")) == 0
             || _tcsicmp(argv[1], _T("/u")) == 0)
    {
        // Request to uninstall.
        if (!IsInstalled())
        {
            _tprintf(_T("%s is not installed\n"), mServiceName);
        }
        else
        {
            // Try and remove the copy that's installed
            if (!Uninstall())
            {
                _tprintf(_T("Could not remove %s. Error %d\n"),
                         mServiceName,
                         GetLastError());
            }
        }
        return true; // say we processed the argument

    }

    // Don't recognise the args
    return false;
}


/**
 * Install/uninstall routines
 * Test if the service is currently installed
 */
bool NTService::IsInstalled()
{
    return IsInstalled(mServiceName);
}


bool NTService::Install()
{
    return InstallAs(SERVICE_DEMAND_START);
}


bool NTService::InstallAs(DWORD startType, TCHAR* path)
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        return false;
    }

    // Get the executable file path
    TCHAR filePath[MAX_PATH];
    ::GetModuleFileName(NULL, filePath, sizeof(filePath));

    //Copy executable file to dest directory
    if (path != NULL && _tcscmp(filePath, path) != 0)
    {
        ::SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);
        if (::CopyFile(filePath, path, FALSE) == TRUE)
        {
            _tcscpy(filePath, path);
        }
        else
        {
            DWORD err = ::GetLastError();
            _tprintf(L"err:%d\n", err);
            return false;
        }
    }

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         mServiceName,
                                         mServiceName,
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         startType,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         filePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService)
    {
        ::CloseServiceHandle(hSCM);
        return false;
    }

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    TCHAR szKey[256];
    HKEY hKey = NULL;
    _tcscpy(szKey, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"));
    _tcscat(szKey, mServiceName);
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS)
    {
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
        return false;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    _T("EventMessageFile"),
                    0,
                    REG_EXPAND_SZ,
                    (CONST BYTE*)filePath,
                    (DWORD)_tcslen(filePath) + 1);

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE
        |EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    _T("TypesSupported"),
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                    sizeof(DWORD));
    ::RegCloseKey(hKey);

    LogEvent(_T("service %s installed"), mServiceName);

    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return true;
}


bool NTService::Uninstall()
{
    TString path = GetServicePath(mServiceName);
    MakeServiceStop();

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM)
    {
        return false;
    }

    bool bResult = false;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       mServiceName,
                                       DELETE);
    if (hService)
    {
        if (::DeleteService(hService))
        {
            _tprintf(_T("service %s removed\n"), mServiceName);
            LogEvent(_T("service %s removed"), mServiceName);

            ::SetFileAttributes(path.c_str(), FILE_ATTRIBUTE_NORMAL);
            if (::DeleteFile(path.c_str()) == FALSE)
            {
                _tprintf(_T("(You must delete the file (%s) yourself.)\n"),
                         mServiceName, path.c_str());
            }

            bResult = true;
        }
        else
        {
            DWORD err = ::GetLastError();
            _tprintf(_T("service %s remove failed.\n Error Code: %d\n"),
                     mServiceName, err);
            LogEvent(_T("service %s remove failed.\n Error Code: %d"),
                     mServiceName, err);
        }
        ::CloseServiceHandle(hService);
    }

    ::CloseServiceHandle(hSCM);

    return bResult;
}


#pragma warning(push)
#pragma warning(disable : 4793)
void NTService::LogEvent(LPCTSTR format, ...) throw()
{
    const int LOG_EVENT_MSG_SIZE = 256;
    TCHAR chMsg[LOG_EVENT_MSG_SIZE];
    LPTSTR lpszStrings[1];

    va_list pArg;
    va_start(pArg, format);
#if _SECURE_ATL
    _vsntprintf_s(chMsg, LOG_EVENT_MSG_SIZE, LOG_EVENT_MSG_SIZE-1,
                  format, pArg);
#else
    _vsntprintf(chMsg, LOG_EVENT_MSG_SIZE, format, pArg);
#endif
    va_end(pArg);

    chMsg[LOG_EVENT_MSG_SIZE - 1] = 0;
    lpszStrings[0] = chMsg;

    LogEventEx(mServiceName, 0, chMsg);
}


void NTService::LogEventEx(int id, LPCTSTR pszMessage, WORD type) throw()
{
    LogEventEx(mServiceName, 0, pszMessage, type);
    ///* Get a handle to use with ReportEvent(). */
    //HANDLE hEventSource = RegisterEventSource(NULL, mServiceName);
    //if (hEventSource != NULL)
    //{
    //      /* Write to event log. */
    //      ReportEvent(hEventSource,
    //                              type,
    //                              (WORD)0,
    //                              id,
    //                              NULL,
    //                              (WORD)(pszMessage != NULL ? 1 : 0),
    //                              0,
    //                              pszMessage != NULL ? &pszMessage : NULL,
    //                              NULL);
    //      DeregisterEventSource(hEventSource);
    //}
}

#pragma warning(pop)


/**
 * Service startup and registration
 */
bool NTService::StartService()
{
    mIsService = IsInstalled();
    if (!mIsService)
    {
        Run();
        return 0;
    }

    SERVICE_TABLE_ENTRY st[] =
        {
            {mServiceName, ServiceMain},
            {NULL, NULL}
        };

    DebugMsg(_T("Calling StartServiceCtrlDispatcher()"));
    bool b = (::StartServiceCtrlDispatcher(st) == TRUE) ? true : false;
    DebugMsg(_T("Returned from StartServiceCtrlDispatcher()"));
    return b;
}


/**
 * static member function (callback)
 */
void NTService::ServiceMain(DWORD argc, LPTSTR* argv)
{
    // Get a pointer to the C++ object
    NTService* pService = mThis;

    pService->DebugMsg(_T("Entering NTService::ServiceMain()"));
    // Register the control request handler
    pService->mStatus.dwCurrentState = SERVICE_START_PENDING;
    pService->mServiceStatus =
        RegisterServiceCtrlHandlerEx(pService->mServiceName, HandlerEx, NULL);
    if (pService->mServiceStatus == NULL)
    {
        pService->LogEvent(_T("error: control handle not installed"));
        return;
    }

    // Start the initialization
    pService->SetStatus(SERVICE_START_PENDING);
    // Set final state
    pService->mStatus.dwWin32ExitCode = GetLastError();
    pService->mStatus.dwCheckPoint = 0;
    pService->mStatus.dwWaitHint = 0;

    // Do the real work.
    // When the Run function returns, the service has stopped.
    pService->mIsRunning = true;
    pService->mStatus.dwWin32ExitCode = 0;
    pService->mStatus.dwCheckPoint = 0;
    pService->mStatus.dwWaitHint = 0;
    pService->Run();

    // Tell the service manager we are stopped
    pService->SetStatus(SERVICE_STOPPED);

    pService->DebugMsg(_T("Leaving NTService::ServiceMain()"));
}


/**
 * status functions
 */
void NTService::SetStatus(DWORD state)
{
    DebugMsg(_T("NTService::SetStatus(%lu, %lu)"), mServiceStatus, state);
    mStatus.dwCurrentState = state;
    ::SetServiceStatus(mServiceStatus, &mStatus);
}


/**
 * main function to do the real work of the service
 * This function performs the main work of the service.
 * When this function returns the service has stopped.
 */
void NTService::Run()
{
    DebugMsg(_T("Entering NTService::Run()"));

    if (OnInit())
    {
        LogEvent(_T("service started"));
        SetStatus(SERVICE_RUNNING);

        while (mIsRunning)
        {
            DebugMsg(_T("Sleeping..."));
            Sleep(100);
        }
    }

    OnUninit();

    // nothing more to do
    DebugMsg(_T("Leaving NTService::Run()"));
}


/**
 * Control request handlers
 * static member function (callback) to handle commands from the
 * service control manager
 */
DWORD NTService::HandlerEx(DWORD ctrl, DWORD type, LPVOID data, LPVOID context)
{
    // Get a pointer to the object
    NTService* pService = mThis;

    pService->DebugMsg(_T("NTService::Handler(%lu)"), ctrl);
    switch (ctrl)
    {
    case SERVICE_CONTROL_STOP: // 1
        pService->SetStatus(SERVICE_STOP_PENDING);
        pService->OnStop();
        pService->mIsRunning = false;
        pService->LogEvent(_T("service stoped"));
        //        pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pService->OnShutdown();
        break;

    case SERVICE_CONTROL_DEVICEEVENT:
        pService->OnDeviceEvent(type, data, context);
        break;

    case SERVICE_CONTROL_HARDWAREPROFILECHANGE:
        pService->OnHardwareProfileChange(type, data, context);
        break;

    case SERVICE_CONTROL_POWEREVENT:
        pService->OnPowerEvent(type, data, context);
        break;

    case SERVICE_CONTROL_SESSIONCHANGE:
        pService->OnSessionChange(type, data, context);
        break;

    default:
        if (ctrl >= SERVICE_CONTROL_USER)
        {
            if (!pService->OnUserControl(ctrl))
            {
                pService->LogEvent(_T("error: bad request"));
                // pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
            }
        }
        else
        {
            pService->LogEvent(_T("error: bad request"));
            // pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
        }
        break;
    }

    // Report current status
    pService->DebugMsg(_T("Updating status (%lu, %lu)"),
                       pService->mServiceStatus,
                       pService->mStatus.dwCurrentState);
    ::SetServiceStatus(pService->mServiceStatus, &pService->mStatus);

    return NO_ERROR;
}


/**
 * Called before Run.
 */
bool NTService::OnInit()
{
    DebugMsg(_T("NTService::OnInit()"));
    return true;
}


/**
 * Called after Run.
 */
bool NTService::OnUninit()
{
    DebugMsg(_T("NTService::OnUninit()"));
    return true;
}


/**
 * Called when the service control manager wants to stop the service
 */
void NTService::OnStop()
{
    DebugMsg(_T("NTService::OnStop()"));
}


/**
 * called when the service is interrogated
 */
void NTService::OnInterrogate()
{
    DebugMsg(_T("NTService::OnInterrogate()"));
}


/**
 * called when the service is paused
 */
void NTService::OnPause()
{
    DebugMsg(_T("NTService::OnPause()"));
}


/**
 * called when the service is continued
 */
void NTService::OnContinue()
{
    DebugMsg(_T("NTService::OnContinue()"));
}


/**
 * called when the service is shut down
 */
void NTService::OnShutdown()
{
    DebugMsg(_T("NTService::OnShutdown()"));
}


void NTService::OnDeviceEvent(DWORD type, LPVOID data, LPVOID context)
{
    DebugMsg(_T("NTService::OnDeviceEvent()"));
}


void NTService::OnHardwareProfileChange(DWORD type, LPVOID data, LPVOID context)
{
    DebugMsg(_T("NTService::OnHardwareProfileChange()"));
}


void NTService::OnPowerEvent(DWORD type, LPVOID data, LPVOID context)
{
    DebugMsg(_T("NTService::OnPowerEvent()"));
}
x

void NTService::OnSessionChange(DWORD type, LPVOID data, LPVOID context)
{
    DebugMsg(_T("NTService::OnSessionChange()"));
}


/**
 * called when the service gets a user control message
 */
bool NTService::OnUserControl(DWORD dwControl)
{
    DebugMsg(_T("NTService::OnUserControl(%8.8lXH)"), dwControl);
    return false; // say not handled
}


/**
 * Debugging support
 */
void NTService::DebugMsg(const TCHAR* pszFormat, ...)
{
#ifdef _DEBUG

    TCHAR buf[1024];
#ifdef UNICODE
    swprintf(buf, 1024, L"[%s](%lu): ", mServiceName, GetCurrentThreadId());
#else
    sprintf(buf, "[%s](%lu): ", mServiceName, GetCurrentThreadId());
#endif

    va_list arglist;
    va_start(arglist, pszFormat);
#ifdef UNICODE
    size_t len = _tcslen(buf);
    vswprintf(&buf[len], len, pszFormat, arglist);
#else
    vsprintf(&buf[_tcslen(buf)], pszFormat, arglist);
#endif
    va_end(arglist);

    _tcscat(buf, _T("\n"));
    OutputDebugString(buf);

#endif
}


bool NTService::MakeServiceStart()
{
    return NTService::MakeServiceStart(mServiceName);
}


bool NTService::MakeServiceStop()
{
    return NTService::MakeServiceStop(mServiceName);
}


bool NTService::IsInstalled(const TCHAR* serviceName)
{
    bool bResult = false;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM)
    {
        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM, serviceName,
                                           SERVICE_QUERY_CONFIG);
        if (hService)
        {
            bResult = true;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }

    return bResult;
}


bool NTService::MakeServiceStart(const TCHAR* serviceName)
{
    assert(serviceName != NULL);
    if (serviceName == NULL)
    {
        return false;
    }

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        return false;
    }
    // Open the service
    SC_HANDLE hService = ::OpenService(hSCM, serviceName, SERVICE_ALL_ACCESS);
    if (!hService)
    {
        ::CloseServiceHandle(hSCM);
        return false;
    }

    bool ret = false;
    if (::StartService(hService, 0, NULL) == TRUE)
    {
        _tprintf(_T("Service start pending.\n"));

        SERVICE_STATUS_PROCESS ssStatus;
        DWORD dwBytesNeeded;

        if (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                                 (LPBYTE)&ssStatus,
                                 sizeof(SERVICE_STATUS_PROCESS),
                                 &dwBytesNeeded ) == TRUE)
        {
            // Save the tick count and initial checkpoint.
            DWORD dwStartTickCount = GetTickCount();
            DWORD dwOldCheckPoint = ssStatus.dwCheckPoint;
            DWORD dwWaitTime;

            while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
            {
                dwWaitTime = ssStatus.dwWaitHint / 10;
                dwWaitTime = (dwWaitTime < 1000) ? 1000 : dwWaitTime;
                dwWaitTime = (dwWaitTime > 10000) ? 10000 : dwWaitTime;
                Sleep(dwWaitTime);

                // Check the status again.
                if (QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                                         (LPBYTE)&ssStatus,
                                         sizeof(SERVICE_STATUS_PROCESS),
                                         &dwBytesNeeded) == FALSE)
                {
                    break;
                }

                if (ssStatus.dwCheckPoint > dwOldCheckPoint)
                {
                    // The service is making progress.
                    dwStartTickCount = GetTickCount();
                    dwOldCheckPoint = ssStatus.dwCheckPoint;
                }
                else
                {
                    if(GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
                    {
                        // No progress made within the wait hint
                        break;
                    }
                }
            }

            if (ssStatus.dwCurrentState == SERVICE_RUNNING)
            {
                printf("StartService SUCCESS.\n");
                ret = true;
            }
            else
            {
                printf("\nService not started. \n");
                printf("  Current State: %d\n", ssStatus.dwCurrentState);
                printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
                printf("  Service Specific Exit Code: %d\n",
                       ssStatus.dwServiceSpecificExitCode);
                printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
                printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
                ret = false;
            }
        }
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return ret;
}


bool NTService::MakeServiceStop(const TCHAR* serviceName)
{
    assert(serviceName != NULL);
    if (serviceName == NULL)
    {
        return false;
    }

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        return false;
    }

    // Open the service
    SC_HANDLE hService = ::OpenService(hSCM, serviceName, SERVICE_ALL_ACCESS);
    if (!hService)
    {
        ::CloseServiceHandle(hSCM);
        return false;
    }

    SERVICE_STATUS ssStatus;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &ssStatus);

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return true;
}


bool NTService::ChangeServiceConfig(const TCHAR* serviceName,
                                    DWORD serviceType,
                                    DWORD startType,
                                    DWORD errorControl,
                                    const TCHAR* const binaryPathName,
                                    const TCHAR* const loadOrderGroup,
                                    LPDWORD tagId,
                                    const TCHAR* const dependencies,
                                    const TCHAR* const serviceStartName,
                                    const TCHAR* const password,
                                    const TCHAR* const displayName)
{
    assert(serviceName != NULL);
    if (serviceName == NULL)
    {
        return false;
    }

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        return false;
    }
    SC_HANDLE hService = ::OpenService(hSCM, serviceName, SERVICE_ALL_ACCESS);
    if (!hService)
    {
        ::CloseServiceHandle(hSCM);
        return false;
    }

    BOOL ret = ::ChangeServiceConfig(hService, serviceType, startType,
                                     errorControl, binaryPathName,
                                     loadOrderGroup, tagId, dependencies,
                                     serviceStartName, password, displayName);

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return (ret == TRUE);
}


TString NTService::GetServicePath(const TCHAR* serviceName)
{
    assert(serviceName != NULL);

    TString ret;
    if (serviceName == NULL)
    {
        return ret;
    }

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM)
    {
        return ret;
    }
    SC_HANDLE hService = ::OpenService(hSCM, serviceName, SERVICE_ALL_ACCESS);
    if (!hService)
    {
        ::CloseServiceHandle(hSCM);
        return ret;
    }

    DWORD dwBytesNeeded;
    LPQUERY_SERVICE_CONFIG lpqscBuf =
        (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, 4096);
    if (lpqscBuf == NULL)
    {
        return ret;
    }

    BOOL result = ::QueryServiceConfig(hService, lpqscBuf,
                                       4096, &dwBytesNeeded);
    if (result == TRUE)
    {
        ret = lpqscBuf->lpBinaryPathName;
    }

    LocalFree(lpqscBuf);

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    return ret;
}


void NTService::LogEventEx(const TCHAR* serviceName, int id,
                           LPCTSTR pszMessage, WORD type) throw()
{
    assert(serviceName != NULL);
    if (serviceName == NULL)
    {
        return;
    }

    /* Get a handle to use with ReportEvent(). */
    HANDLE hEventSource = RegisterEventSource(NULL, serviceName);
    if (hEventSource != NULL)
    {
        /* Write to event log. */
        ReportEvent(hEventSource,
                    type,
                    (WORD)0,
                    id,
                    NULL,
                    (WORD)(pszMessage != NULL ? 1 : 0),
                    0,
                    pszMessage != NULL ? &pszMessage : NULL,
                    NULL);
        DeregisterEventSource(hEventSource);
    }
}


#pragma warning(pop)
