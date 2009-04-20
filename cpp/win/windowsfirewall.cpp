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
#include "windowsfirewall.h"


namespace
{
    void AutoWrap(int autoType, VARIANT* pvResult, IDispatch* pDisp,
                  LPOLESTR ptName, int cArgs...)
    {
        if (pDisp == NULL)
        {
            throw "pDisp is NULL";
        }

        // Begin variable-argument list...
        va_list marker;
        va_start(marker, cArgs);

        // Variables used...
        DISPPARAMS dp = { NULL, NULL, 0, 0 };
        DISPID dispidNamed = DISPID_PROPERTYPUT;
        DISPID dispID;

        //char buf[200];
        char szName[200];
        // Convert down to ANSI
        WideCharToMultiByte(CP_ACP, 0, ptName, -1, szName, 256, NULL, NULL);

        // Get DISPID for name passed...
        HRESULT hr = pDisp->GetIDsOfNames(IID_NULL, &ptName, 1,
                                          LOCALE_USER_DEFAULT, &dispID);
        if (FAILED(hr))
        {
            throw "GetIDsOfNames error";
        }

        // Allocate memory for arguments...
        VARIANT *pArgs = new VARIANT[cArgs+1];
        try
        {
            // Extract arguments...
            for (int i = 0; i < cArgs; i++)
            {
                pArgs[i] = va_arg(marker, VARIANT);
            }

            // Build DISPPARAMS
            dp.cArgs = cArgs;
            dp.rgvarg = pArgs;

            // Handle special-case for property-puts!
            if (autoType & DISPATCH_PROPERTYPUT)
            {
                dp.cNamedArgs = 1;
                dp.rgdispidNamedArgs = &dispidNamed;
            }

            // Make the call!
            hr = pDisp->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT,
                               autoType, &dp, pvResult, NULL, NULL);
            if (FAILED(hr))
            {
                throw "Invoke error";
            }

            // End variable-argument section...
            va_end(marker);
            delete [] pArgs;
            pArgs = NULL;
        }
        catch (...)
        {
            delete [] pArgs;
            pArgs = NULL;
            throw;
        }
    }
}

/**
 * description
 */
WindowsFirewall::WindowsFirewall()
    : fwMgr_(NULL), fwPolicy_(NULL), fwProfile_(NULL), comInit_(E_FAIL)
{
    // Initialize COM.
    DWORD dwCoInit = COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE;
    comInit_ = ::CoInitializeEx(0, dwCoInit);
    // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
    // initialized with a different mode. Since we don't care what the mode is,
    // we'll just use the existing mode.
    if (comInit_ != RPC_E_CHANGED_MODE && FAILED(comInit_))
    {
        throw _com_error(comInit_);
    }

    Initialize();
}


/**
 * description
 */
WindowsFirewall::~WindowsFirewall()
{
    Cleanup();

    // Uninitialize COM.
    if (SUCCEEDED(comInit_))
    {
        ::CoUninitialize();
    }
}


void WindowsFirewall::AddApp(const wchar_t* fwProcessImageFileName,
                             const wchar_t* fwName,
                             bool enable)
{
    assert(fwProcessImageFileName != NULL);
    assert(fwName != NULL);
    if (fwProcessImageFileName == NULL || fwName == NULL)
    {
        throw _com_error(E_FAIL);
    }

    IDispatch* app = NULL;
    CLSID appid;
    HRESULT hr = ::CLSIDFromProgID(L"HNetCfg.FwAuthorizedApplication", &appid);
    hr = ::CoCreateInstance(appid, NULL, CLSCTX_ALL, IID_IDispatch,
                            (LPVOID*)&app);
    if (FAILED(hr))
    {
        throw _com_error(hr);
    }

    _variant_t param1(fwProcessImageFileName);
    AutoWrap(DISPATCH_PROPERTYPUT, NULL, app, L"ProcessImageFileName", 1,
             (VARIANT)param1);

    _variant_t param2(fwName);
    AutoWrap(DISPATCH_PROPERTYPUT, NULL, app, L"Name", 1, (VARIANT)param2);

    VARIANT param3;
    param3.vt = VT_BOOL;
    param3.boolVal = (enable ? VARIANT_TRUE : VARIANT_FALSE);
    AutoWrap(DISPATCH_PROPERTYPUT, NULL, app, L"Enabled", 1, param3);

    VARIANT apps;
    bool ret = false;
    try
    {
        AutoWrap(DISPATCH_PROPERTYGET, &apps, fwProfile_,
                 L"AuthorizedApplications", 0);

        VARIANT param4;
        param4.vt = VT_DISPATCH;
        param4.pdispVal = app;
        AutoWrap(DISPATCH_METHOD, NULL, apps.pdispVal, L"Add", 1, param4);
        ret = true;
    }
    catch (...)
    {
    }

    if (apps.pdispVal != NULL)
    {
        apps.pdispVal->Release();
    }
    if (app != NULL)
    {
        app->Release();
    }

    if (!ret)
    {
        throw _com_error(E_FAIL);
    }
}


void WindowsFirewall::Initialize()
{
    CLSID clsid;
    HRESULT hr = ::CLSIDFromProgID(L"HNetCfg.FwMgr", &clsid);
    hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IDispatch,
                            (LPVOID*)&fwMgr_);

    VARIANT policy;
    AutoWrap(DISPATCH_PROPERTYGET, &policy, fwMgr_, L"LocalPolicy", 0);
    fwPolicy_ = policy.pdispVal;

    VARIANT profile;
    AutoWrap(DISPATCH_PROPERTYGET, &profile, fwPolicy_, L"CurrentProfile", 0);
    fwProfile_ = profile.pdispVal;
}


void WindowsFirewall::Cleanup()
{
    if (fwProfile_ != NULL)
    {
        fwProfile_->Release();
    }

    if (fwPolicy_ != NULL)
    {
        fwPolicy_->Release();
    }

    if (fwMgr_ != NULL)
    {
        fwMgr_->Release();
    }
}
