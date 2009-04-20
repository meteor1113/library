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
#include <comutil.h>
#include <Shlwapi.h>
#include <atlsafe.h>

// #include "customer headers"
#include "exceloperate.h"


namespace msoffice
{

    /**
     * description
     */
    ExcelOperate::ExcelOperate(const std::wstring& filename, bool visible)
        :pXlApp(NULL), pXlBooks(NULL), pXlBook(NULL), pXlSheet(NULL)
    {
        //CoInitialize(NULL);   // initialize COM
        InstanceExcelWorkbook(visible);

        if (filename.empty())
        {
            CreateNewWorkbook();
        }
        else
        {
            OpenWorkbook(filename);
        }

        if (pXlSheet == NULL || pXlBook == NULL
            || pXlBooks == NULL || pXlApp == NULL)
        {
            throw ExcelException();
        }
    }


    /**
     * description
     */
    ExcelOperate::~ExcelOperate()
    {
        if (pXlApp != NULL)
        {
            // Application.Quit()
            AutoWrap(DISPATCH_METHOD, NULL, pXlApp, L"Quit", 0);
        }

        // release all resource
        if (pXlSheet != NULL)
        {
            pXlSheet->Release();
        }
        if (pXlBook != NULL)
        {
            pXlBook->Release();
        }
        if (pXlBooks != NULL)
        {
            pXlBooks->Release();
        }
        if (pXlApp != NULL)
        {
            pXlApp->Release();
        }

        // logout COM
        //CoUninitialize();
    }


    /**
     * set single unit or a range
     * for example:
     * SetData(L"D8:D8", L"5");
     * SetData(L"E3:G5", L"kkj");
     * SetData(L"H9", L"H9");
     *
     * @param cell mark a range, e.g. "A1:B2".
     *             for a single unit, use "A1:A1" or "A1".
     * @param data all unit of rang set to this value
     */
    void ExcelOperate::SetData(const std::wstring& cell,
                               const std::wstring& data)
    {
        if (cell.empty())
        {
            throw ExcelException("cell not exist");
        }

        _variant_t parm(cell.c_str());
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlSheet, L"Range",
                 1, (VARIANT)parm);

        //full the range
        _variant_t vtData(data.c_str());
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, result.pdispVal, L"Value",
                 1, (VARIANT)vtData);
    }


    /**
     * full one row data
     *
     * @param nRow row number, base 1
     * @param nBeg column begin, base 1
     * @prarm data
     */
    void ExcelOperate::SetData(int nRow, int nBeg,
                               const std::list<std::wstring>& data)
    {
        if (data.size() <= 0)
        {
            return;
        }

        int nEnd = nBeg + static_cast<int>(data.size()) - 1;

        // create array
        CComSafeArray<VARIANT> arr(static_cast<ULONG>(0));
        for (std::list<std::wstring>::const_iterator it = data.begin();
             it != data.end(); ++it)
        {
            arr.Add(_variant_t(it->c_str()).Detach(), FALSE);
        }
        VARIANT vArr;
        vArr.vt = VT_ARRAY | VT_VARIANT;
        vArr.parray = (SAFEARRAY*)arr;

        // select range
        WCHAR szTmp[4];
        wsprintfW(szTmp, L"%i", nRow);
        std::wstring sTmp = GetColumnName(nBeg) + szTmp + L":"
            + GetColumnName(nEnd) + szTmp;
        _variant_t parm(sTmp.c_str());
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlSheet, L"Range",
                 1, (VARIANT)parm);

        //full the range
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, result.pdispVal, L"Value",
                 1, vArr);
    }


    /**
     * full one column data
     *
     * @param colTag column tag, e.g. "A", "AB", "M"
     * @param nBeg row begin, base 1
     * @prarm data
     */
    void ExcelOperate::SetData(const std::wstring& colTag, int nBeg,
                               const std::list<std::wstring>& data)
    {
        if (data.size() <= 0)
        {
            return;
        }

        int nEnd = nBeg + static_cast<int>(data.size()) - 1;

        // create array
        CComSafeArray<VARIANT> arr(static_cast<ULONG>(0));
        for (std::list<std::wstring>::const_iterator it = data.begin();
             it != data.end(); ++it)
        {
            arr.Add(_variant_t(it->c_str()).Detach(), FALSE);
        }
        VARIANT vArr;
        vArr.vt = VT_ARRAY | VT_VARIANT;
        vArr.parray = (SAFEARRAY*)arr;

        // select range
        WCHAR szTmp1[4];
        wsprintfW(szTmp1, L"%i", nBeg);
        WCHAR szTmp2[4];
        wsprintfW(szTmp2, L"%i", nEnd);
        std::wstring sTmp = colTag + szTmp1 + L":" + colTag + szTmp2;
        _variant_t parm(sTmp.c_str());
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlSheet, L"Range", 1, parm);

        //full the range
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, result.pdispVal, L"Value",
                 1, vArr);
    }


    void ExcelOperate::SaveAsFile(const std::wstring& filename)
    {
        if (!CheckFilename(filename))
        {
            throw ExcelException("save error");
        }

        _variant_t file(filename.c_str());
        AutoWrap(DISPATCH_METHOD, NULL, pXlSheet, L"SaveAs", 1, file);
    }


    void ExcelOperate::CreateNewWorkbook()
    {
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlBooks, L"Add", 0);
        pXlBook = result.Detach().pdispVal;

        GetActiveSheet();
    }


    void ExcelOperate::OpenWorkbook(const std::wstring& filename)
    {
        _variant_t file(filename.c_str());

        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlBooks, L"Open", 1,
                 (VARIANT)file);
        pXlBook = result.Detach().pdispVal;

        GetActiveSheet();
    }


    void ExcelOperate::GetActiveSheet()
    {
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"ActiveSheet", 0);
        pXlSheet = result.Detach().pdispVal;
    }


    std::wstring ExcelOperate::GetColumnName(int nIndex)
    {
        std::wstring cColumnName[] = {L"A", L"B", L"C",L"D",L"E",L"F",L"G",L"H",L"I",L"J",L"K",L"L",L"M",L"N",L"O",L"P",L"Q",L"R",L"S",L"T",L"U",L"V",L"W",L"X",L"Y",L"Z",
                                      L"AA", L"AB", L"AC",L"AD",L"AE",L"AF",L"AG",L"AH",L"AI",L"AJ",L"AK",L"AL",L"AM",L"AN",L"AO",L"AP",L"AQ",L"AR",L"AS",L"AT",L"AU",L"AV",L"AW",L"AX",L"AY",L"AZ",
                                      L"BA", L"BB", L"BC",L"BD",L"BE",L"BF",L"BG",L"BH",L"BI",L"BJ",L"BK",L"BL",L"BM",L"BN",L"BO",L"BP",L"BQ",L"BR",L"BS",L"BT",L"BU",L"BV",L"BW",L"BX",L"BY",L"BZ",
                                      L"CA", L"CB", L"CC",L"CD",L"CE",L"CF",L"CG",L"CH",L"CI",L"CJ",L"CK",L"CL",L"CM",L"CN",L"CO",L"CP",L"CQ",L"CR",L"CS",L"CT",L"CU",L"CV",L"CW",L"CX",L"CY",L"CZ",
                                      L"DA", L"DB", L"DC",L"DD",L"DE",L"DF",L"DG",L"DH",L"DI",L"DJ",L"DK",L"DL",L"DM",L"DN",L"DO",L"DP",L"DQ",L"DR",L"DS",L"DT",L"DU",L"DV",L"DW",L"DX",L"DY",L"DZ",
                                      L"EA", L"EB", L"EC",L"ED",L"EE",L"EF",L"EG",L"EH",L"EI",L"EJ",L"EK",L"EL",L"EM",L"EN",L"EO",L"EP",L"EQ",L"ER",L"ES",L"ET",L"EU",L"EV",L"EW",L"EX",L"EY",L"EZ",
                                      L"FA", L"FB", L"FC",L"FD",L"FE",L"FF",L"FG",L"FH",L"FI",L"FJ",L"FK",L"FL",L"FM",L"FN",L"FO",L"FP",L"FQ",L"FR",L"FS",L"FT",L"FU",L"FV",L"FW",L"FX",L"FY",L"FZ",
                                      L"GA", L"GB", L"GC",L"GD",L"GE",L"GF",L"GG",L"GH",L"GI",L"GJ",L"GK",L"GL",L"GM",L"GN",L"GO",L"GP",L"GQ",L"GR",L"GS",L"GT",L"GU",L"GV",L"GW",L"GX",L"GY",L"GZ",
                                      L"HA", L"HB", L"HC",L"HD",L"HE",L"HF",L"HG",L"HH",L"HI",L"HJ",L"HK",L"HL",L"HM",L"HN",L"HO",L"HP",L"HQ",L"HR",L"HS",L"HT",L"HU",L"HV",L"HW",L"HX",L"HY",L"HZ",
                                      L"IA", L"IB", L"IC",L"ID",L"IE",L"IF",L"IG",L"IH",L"II",L"IJ",L"IK",L"IL",L"IM",L"IN",L"IO",L"IP",L"IQ",L"IR",L"IS",L"IT",L"IU",L"IV",L"IW",L"IX",L"IY",L"IZ"};

        if (nIndex < 0 || nIndex > 259)
        {
            throw ExcelException("index error");
        }
        else
        {
            return cColumnName[nIndex-1];
        }
    }


    bool ExcelOperate::CheckFilename(const std::wstring& filename)
    {
        if (filename.empty())
        {
            return false;
        }

        if (PathFileExists(filename.c_str()))
        {
            if (DeleteFile(filename.c_str()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return true;
    }


    void ExcelOperate::InstanceExcelWorkbook(int visible)
    {
        // get CLSID of excel
        CLSID clsid;
        HRESULT hr = CLSIDFromProgID(L"Excel.Application", &clsid);
        if (FAILED(hr))
        {
            throw ExcelException("start excel failed. confirm excel installed");
        }

        // instance app
        hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER,
                              IID_IDispatch, (void **)&pXlApp);
        if (FAILED(hr))
        {
            throw ExcelException("start excel failed. confirm excel installed");
        }

        // display, set Application.Visible to 1
        _variant_t x((long)visible);
        AutoWrap(DISPATCH_PROPERTYPUT, NULL, pXlApp, L"Visible", 1, (VARIANT)x);

        // get workbooks
        _variant_t result;
        AutoWrap(DISPATCH_PROPERTYGET, &result, pXlApp, L"Workbooks", 0);
        pXlBooks = result.Detach().pdispVal;
    }


    void ExcelOperate::AutoWrap(int autoType, VARIANT* pvResult,
                                IDispatch *pDisp,
                                LPOLESTR ptName, int cArgs...)
    {
        assert(pDisp);
        if (pDisp == NULL)
        {
            throw ExcelException("pDisp is NULL");
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
        assert(SUCCEEDED(hr));
        if (FAILED(hr))
        {
            throw ExcelException("GetIDsOfNames error");
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
                               autoType, &dp,
                               pvResult, NULL, NULL);
            assert(SUCCEEDED(hr));
            if (FAILED(hr))
            {
                throw ExcelException("Invoke error");
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

/*
  int main()
  {
  ExcelOperate eo(L"D:\\project\\BasicReportTemplate.xls", true);
  eo.SetData(L"D8:D8", L"5");
  eo.SetData(L"E3:G5", L"kkj");
  eo.SetData(L"H9", L"H9");
  std::wstring d[] = {L"a", L"b", L"c", L"d"};
  std::list<std::wstring> l;
  for (int i = 0; i < 10; i++)
  {
  l.push_back(L"a");
  }
  eo.SetData(10, 1, l);

  eo.SetData(L"M", 9, l);
  eo.SaveAsFile(L"D:\\project\\a.xls");
  }
//*/
