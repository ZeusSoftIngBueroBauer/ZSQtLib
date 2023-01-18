/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer, Germany
                         Gewerbepark 28
                         D-83670 Bad Heilbrunn
                         Tel: 0049 8046 9488
                         www.zeussoft.de
                         E-Mail: mailbox@zeussoft.de

--------------------------------------------------------------------------------

Content: This file is part of the ZSQtLib.

This file may be used with no license restrictions for your needs. But it is not
allowed to resell any modules of the ZSQtLib veiling the original developer of
the modules. Therefore the copyright link to ZeusSoft, Ing. Buero Bauer must not
be removed from the header of the source code modules.

ZeusSoft, Ing. Buero Bauer provides the source code as is without any guarantee
that the code is written without faults.

ZeusSoft, Ing. Buero Bauer does not assume any liability for any damages which
may result in using the software modules.

*******************************************************************************/

#include "stdafx.h"

#include <vector>

#include "ZSAppTestIpcTraceDllIfWinApp.h"
#include "TestModule1.h"
#include "TestModule2.h"

#include "ZSIpcTrace/ZSIpcTrcDllIf.h"

using namespace ZS::Apps::Test::IpcTraceDllIfWinApp;

#define MAX_LOADSTRING 100

/*******************************************************************************
Global Variables
*******************************************************************************/

static HINSTANCE s_hInst;
static WCHAR s_szTitle[MAX_LOADSTRING];
static WCHAR s_szWindowClass[MAX_LOADSTRING];

static bool s_bDllLoaded = false;
static ZS::Trace::DllIf::CIpcTrcServer* s_pTrcServer = nullptr;
static CTestModule1* s_pTestModule1 = nullptr;
static CTestModule2* s_pTestModule2 = nullptr;

/*******************************************************************************
Forward declarations of functions included in this code module
*******************************************************************************/

ATOM             MyRegisterClass( HINSTANCE i_hInstance );
BOOL             InitInstance( HINSTANCE i_hInstance, int i_iCmdShow );
LRESULT CALLBACK WndProc( HWND i_hWnd, UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam );
INT_PTR CALLBACK About( HWND hDlg, UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam );
void             showLastError( LPTSTR i_lpszFunction );


/*******************************************************************************
Functions Implementation
*******************************************************************************/

//------------------------------------------------------------------------------
std::wstring s2ws( const std::string& s )
//------------------------------------------------------------------------------
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}


//------------------------------------------------------------------------------
int APIENTRY wWinMain(
    HINSTANCE i_hInstance,
    HINSTANCE i_hPrevInstance,
    LPWSTR    i_lpCmdLine,
    int       i_nCmdShow )
//------------------------------------------------------------------------------
{
    UNREFERENCED_PARAMETER(i_hPrevInstance);
    UNREFERENCED_PARAMETER(i_lpCmdLine);

    LoadStringW(i_hInstance, IDS_APP_TITLE, s_szTitle, MAX_LOADSTRING);
    LoadStringW(i_hInstance, IDC_ZSAPPTESTIPCTRACEDLLIF, s_szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(i_hInstance);

    if (!InitInstance(i_hInstance, i_nCmdShow) )
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(i_hInstance, MAKEINTRESOURCE(IDC_ZSAPPTESTIPCTRACEDLLIF));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;

} // wWinMain

//------------------------------------------------------------------------------
ATOM MyRegisterClass( HINSTANCE i_hInstance )
//------------------------------------------------------------------------------
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = i_hInstance;
    wcex.hIcon          = LoadIcon(i_hInstance, MAKEINTRESOURCE(IDI_ZSAPPTESTIPCTRACEDLLIF));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ZSAPPTESTIPCTRACEDLLIF);
    wcex.lpszClassName  = s_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ZSAPPTESTIPCTRACEDLLIF));

    return RegisterClassExW(&wcex);

} // MyRegisterClass

//------------------------------------------------------------------------------
BOOL InitInstance( HINSTANCE i_hInstance, int i_iCmdShow )
//------------------------------------------------------------------------------
{
    s_hInst = i_hInstance; // Instanzenhandle in der globalen Variablen speichern

    HWND hWnd = CreateWindowW(
        /* lpClassName  */ s_szWindowClass,
        /* lpWindowName */ s_szTitle,
        /* dwStyle      */ WS_OVERLAPPEDWINDOW,
        /* x            */ CW_USEDEFAULT,
        /* y            */ 0,
        /* nWidth       */ CW_USEDEFAULT,
        /* nHeight      */ 0,
        /* hWndParent   */ nullptr,
        /* hMenu        */ nullptr,
        /* hInstance    */ i_hInstance,
        /* lpParam      */ nullptr );

   if( !hWnd )
   {
      return FALSE;
   }

    ShowWindow(hWnd, i_iCmdShow);
    UpdateWindow(hWnd);

    return TRUE;

} // InitInstance

//------------------------------------------------------------------------------
void doTestStepLoadDll( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    ZS::Trace::DllIf::CIpcTrcServer::SetOrganizationName("ZeusSoft");
    ZS::Trace::DllIf::CIpcTrcServer::SetApplicationName("ZSAppTestIpcTraceDllIfWinApp");

    s_bDllLoaded = ZS::Trace::DllIf::loadDll();

    if( s_bDllLoaded )
    {
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_LoadDll,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
            /* uEnable       */ MF_ENABLED );
    }
    else
    {
        LPTSTR  lpFunctionBuf = nullptr;
        SIZE_T  uFunctionBufSize;
        LPCTSTR lpszFunction = _TEXT("LoadLibrary");
        std::wstring wstrDllFileName = s2ws(ZS::Trace::DllIf::getDllFileName());

        uFunctionBufSize = ( lstrlen(lpszFunction) + lstrlen(wstrDllFileName.c_str()) + 10 ) * sizeof(TCHAR);
        lpFunctionBuf = (LPTSTR)LocalAlloc( LMEM_ZEROINIT, uFunctionBufSize );

        #pragma warning( disable : 4995 )
        wsprintf( lpFunctionBuf, _TEXT("%s %s"), lpszFunction, wstrDllFileName.c_str() );
        #pragma warning( default : 4995 )

        showLastError(lpFunctionBuf);

        LocalFree(lpFunctionBuf);
    }
} // doTestStepLoadDll

//------------------------------------------------------------------------------
void doTestStepReleaseDll( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    s_bDllLoaded = ZS::Trace::DllIf::releaseDll();

    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_LoadDll,
        /* uEnable       */ MF_ENABLED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
        /* uEnable       */ MF_ENABLED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
        /* uEnable       */ MF_ENABLED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );

} // doTestStepReleaseDll

//------------------------------------------------------------------------------
void doTestStepTraceServerCreateInstance( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    ZS::Trace::DllIf::CIpcTrcServer::SetOrganizationName("ZeusSoft");
    ZS::Trace::DllIf::CIpcTrcServer::SetApplicationName("ZSAppTestIpcTraceDllIfWinApp");

    s_pTrcServer = ZS::Trace::DllIf::CIpcTrcServer::CreateInstance();

    if( s_pTrcServer != nullptr )
    {
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_LoadDll,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
            /* uEnable       */ MF_ENABLED );
    }
} // doTestStepTraceServerCreateInstance

//------------------------------------------------------------------------------
void doTestStepTraceServerDestroyInstance( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTrcServer != nullptr )
    {
        ZS::Trace::DllIf::CIpcTrcServer::ReleaseInstance(s_pTrcServer);
        s_pTrcServer = nullptr;

        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_LoadDll,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
            /* uEnable       */ MF_ENABLED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
        EnableMenuItem(
            /* hMenu         */ i_hMenu,
            /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
            /* uEnable       */ MF_DISABLED|MF_GRAYED );
    }
} // doTestStepTraceServerDestroyInstance

//------------------------------------------------------------------------------
void doTestStepTraceServerStartup( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTrcServer != nullptr )
    {
        bool bOk = s_pTrcServer->startup(10000, true);

        if( bOk )
        {
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_LoadDll,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
                /* uEnable       */ MF_ENABLED );
        }
    }
} // doTestStepTraceServerStartup

//------------------------------------------------------------------------------
void doTestStepTraceServerShutdown( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTrcServer != nullptr )
    {
        bool bOk = s_pTrcServer->shutdown(10000, true);

        if( bOk )
        {
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_LoadDll,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_ReleaseDll,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerCreateInstance,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerDestroyInstance,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerStartup,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerShutdown,
                /* uEnable       */ MF_DISABLED|MF_GRAYED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerRecallAdminObjs,
                /* uEnable       */ MF_ENABLED );
            EnableMenuItem(
                /* hMenu         */ i_hMenu,
                /* uIDEnableItem */ IDM_TestStep_TraceServerSaveAdminObjs,
                /* uEnable       */ MF_ENABLED );
        }
    }
} // doTestStepTraceServerShutdown

//------------------------------------------------------------------------------
void doTestStepTraceServerRecallAdminObjs( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTrcServer != nullptr )
    {
        s_pTrcServer->recallAdminObjs();
    }
} // doTestStepTraceServerRecallAdminObjs

//------------------------------------------------------------------------------
void doTestStepTraceServerSaveAdminObjs( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTrcServer != nullptr )
    {
        s_pTrcServer->saveAdminObjs();
    }
} // doTestStepTraceServerSaveAdminObjs

//------------------------------------------------------------------------------
void doTestStepTraceClientConnect( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    int iRet = MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ (LPCWSTR)L"Is the trace client connected?",
        /* lpCaption */ (LPCWSTR)L"Please start and connect the trace method client.",
        /* uType     */ MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );

    if( iRet == IDYES )
    {
        if( s_pTrcServer != nullptr )
        {
            if( s_pTrcServer->isConnected() )
            {
                EnableMenuItem(
                    /* hMenu         */ i_hMenu,
                    /* uIDEnableItem */ IDM_TestStep_TraceClientConnect,
                    /* uEnable       */ MF_DISABLED|MF_GRAYED );
                EnableMenuItem(
                    /* hMenu         */ i_hMenu,
                    /* uIDEnableItem */ IDM_TestStep_TraceClientDisconnect,
                    /* uEnable       */ MF_ENABLED );
            }
        }
    }

} // doTestStepTraceClientConnect

//------------------------------------------------------------------------------
void doTestStepTraceClientDisconnect( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    int iRet = MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ (LPCWSTR)L"Is the trace client disconnected?",
        /* lpCaption */ (LPCWSTR)L"Please disconnect the trace method client.",
        /* uType     */ MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );

    if( iRet == IDYES )
    {
        if( s_pTrcServer != nullptr )
        {
            if( !s_pTrcServer->isConnected() )
            {
                EnableMenuItem(
                    /* hMenu         */ i_hMenu,
                    /* uIDEnableItem */ IDM_TestStep_TraceClientConnect,
                    /* uEnable       */ MF_ENABLED );
                EnableMenuItem(
                    /* hMenu         */ i_hMenu,
                    /* uIDEnableItem */ IDM_TestStep_TraceClientDisconnect,
                    /* uEnable       */ MF_DISABLED|MF_GRAYED );
            }
        }
    }

} // doTestStepTraceClientDisconnect

//------------------------------------------------------------------------------
void doTestStepCreateModule1( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTestModule1 == nullptr )
    {
        s_pTestModule1 = new CTestModule1("TestModule1", "TestModule2");
    }

    std::wstring strNameSpace  = s2ws(CTestModule1::NameSpace());
    std::wstring strClassName  = s2ws(CTestModule1::ClassName());
    std::wstring strObjName    = L"TestModule1";
    std::wstring strMthName    = L"ctor";
    std::wstring strMthInArgs  = L"ObjName: TestModule1, TestModule2ObjName: TestModule2";
    std::wstring strMthOutArgs = L"";

    std::vector<std::wstring> strlstMthTraces;

    strlstMthTraces.push_back(L"-> <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthInArgs + L")");
    strlstMthTraces.push_back(L"<- <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthOutArgs + L")");

    std::wstring strInfoText;

    strInfoText = L"Do you see the following lines?";

    for( const auto& strTrc : strlstMthTraces )
    {
        strInfoText += L"\n" + strTrc;
    }

    int iRet = MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ strInfoText.c_str(),
        /* lpCaption */ (LPCWSTR)L"Please have a look at the trace output widget of the method trace client.",
        /* uType     */ MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );

    if( iRet == IDYES )
    {
    }

    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_CreateModule1,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_DeleteModule1,
        /* uEnable       */ MF_ENABLED );

} // doTestStepCreateModule1

//------------------------------------------------------------------------------
void doTestStepDeleteModule1( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    delete s_pTestModule1;
    s_pTestModule1 = nullptr;
    s_pTestModule2 = nullptr;

    std::wstring strNameSpace  = s2ws(CTestModule1::NameSpace());
    std::wstring strClassName  = s2ws(CTestModule1::ClassName());
    std::wstring strObjName    = L"TestModule1";
    std::wstring strMthName    = L"dtor";
    std::wstring strMthInArgs  = L"";
    std::wstring strMthOutArgs = L"";

    std::vector<std::wstring> strlstMthTraces;

    strlstMthTraces.push_back(L"-> <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthInArgs + L")");
    strlstMthTraces.push_back(L"<- <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthOutArgs + L")");

    std::wstring strInfoText;

    strInfoText = L"Do you see the following lines?";

    for( const auto& strTrc : strlstMthTraces )
    {
        strInfoText += L"\n" + strTrc;
    }

    int iRet = MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ strInfoText.c_str(),
        /* lpCaption */ (LPCWSTR)L"Please have a look at the trace output widget of the method trace client.",
        /* uType     */ MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );

    if( iRet == IDYES )
    {
    }

    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_CreateModule1,
        /* uEnable       */ MF_ENABLED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_DeleteModule1,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );

} // doTestStepDeleteModule1

//------------------------------------------------------------------------------
void doTestStepCreateModule2( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTestModule1 != nullptr )
    {
        s_pTestModule2 = s_pTestModule1->createModule2();
    }

    std::wstring strNameSpace  = s2ws(CTestModule2::NameSpace());
    std::wstring strClassName  = s2ws(CTestModule2::ClassName());
    std::wstring strObjName    = L"TestModule12";
    std::wstring strMthName    = L"dtor";
    std::wstring strMthInArgs  = L"";
    std::wstring strMthOutArgs = L"";

    std::vector<std::wstring> strlstMthTraces;

    strlstMthTraces.push_back(L"   -> <" + strNameSpace + L"::" + strClassName + L"> TestModule2.ctor(TestModule2Thread: TestModule2, ObjName: TestModule2)");
    strlstMthTraces.push_back(L"   <- <" + strNameSpace + L"::" + strClassName + L"> TestModule2.ctor()");
    strlstMthTraces.push_back(L"                                 <- <" + strNameSpace + L"::" + strClassName + L"> TestModule2.recursiveTraceMethod(): 10");
    strlstMthTraces.push_back(L"   <- <" + strNameSpace + L"::" + strClassName + L"> TestModule2.recursiveTraceMethod(): 0");

    std::wstring strInfoText;

    strInfoText = L"Do you see the following lines?";

    for( const auto& strTrc : strlstMthTraces )
    {
        strInfoText += L"\n" + strTrc;
    }

    int iRet = MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ strInfoText.c_str(),
        /* lpCaption */ (LPCWSTR)L"Please have a look at the trace output widget of the method trace client.",
        /* uType     */ MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 );

    if( iRet == IDYES )
    {
    }

    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_CreateModule2,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_DeleteModule2,
        /* uEnable       */ MF_ENABLED );

} // doTestStepCreateModule2

//------------------------------------------------------------------------------
void doTestStepDeleteModule2( HMENU i_hMenu )
//------------------------------------------------------------------------------
{
    if( s_pTestModule1 != nullptr )
    {
        s_pTestModule1->deleteModule2();
        s_pTestModule2 = nullptr;
    }

    std::wstring strNameSpace  = s2ws(CTestModule2::NameSpace());
    std::wstring strClassName  = s2ws(CTestModule2::ClassName());
    std::wstring strObjName    = L"TestModule2";
    std::wstring strMthName    = L"dtor";
    std::wstring strMthInArgs  = L"";
    std::wstring strMthOutArgs = L"";

    std::vector<std::wstring> strlstMthTraces;

    strlstMthTraces.push_back(L"-> <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthInArgs + L")");
    strlstMthTraces.push_back(L"<- <" + strNameSpace + L"::" + strClassName + L"> " + strObjName + L"." + strMthName + L"(" + strMthOutArgs + L")");

    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_CreateModule2,
        /* uEnable       */ MF_ENABLED );
    EnableMenuItem(
        /* hMenu         */ i_hMenu,
        /* uIDEnableItem */ IDM_TestStep_DeleteModule2,
        /* uEnable       */ MF_DISABLED|MF_GRAYED );

} // doTestStepDeleteModule2

//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND i_hWnd, UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam)
//------------------------------------------------------------------------------
{
    switch (i_uMessage)
    {
        case WM_COMMAND:
        {
            int   wmId    = LOWORD(i_wParam);
            int   wmEvent = HIWORD(i_wParam);
            HMENU hMenu   = GetMenu(i_hWnd);

            // Menüauswahl analysieren:
            switch (wmId)
            {
                case IDM_ABOUT:
                {
                    DialogBox(s_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), i_hWnd, About);
                    break;
                }
                case IDM_TestStep_LoadDll:
                {
                    if( !s_bDllLoaded )
                    {
                        doTestStepLoadDll(hMenu);
                    }
                    break;
                }
                case IDM_TestStep_ReleaseDll:
                {
                    if( s_pTrcServer != nullptr )
                    {
                        if( s_pTrcServer->isListening() )
                        {
                            doTestStepTraceServerShutdown(hMenu);
                        }
                        doTestStepTraceServerDestroyInstance(hMenu);
                    }
                    doTestStepReleaseDll(hMenu);
                    s_bDllLoaded = false;
                    break;
                }
                case IDM_TestStep_TraceServerCreateInstance:
                {
                    if( !s_bDllLoaded )
                    {
                        doTestStepLoadDll(hMenu);
                    }
                    doTestStepTraceServerCreateInstance(hMenu);
                    break;
                }
                case IDM_TestStep_TraceServerDestroyInstance:
                {
                    if( s_pTrcServer != nullptr )
                    {
                        if( s_pTrcServer->isListening() )
                        {
                            doTestStepTraceServerShutdown(hMenu);
                        }
                    }
                    doTestStepTraceServerDestroyInstance(hMenu);
                    break;
                }
                case IDM_TestStep_TraceServerStartup:
                {
                    if( !s_bDllLoaded )
                    {
                        doTestStepLoadDll(hMenu);
                    }
                    if( s_pTrcServer == nullptr )
                    {
                        doTestStepTraceServerCreateInstance(hMenu);
                    }
                    doTestStepTraceServerStartup(hMenu);
                    break;
                }
                case IDM_TestStep_TraceServerShutdown:
                {
                    doTestStepTraceServerShutdown(hMenu);
                    break;
                }
                case IDM_TestStep_TraceServerRecallAdminObjs:
                {
                    doTestStepTraceServerRecallAdminObjs(hMenu);
                    break;
                }
                case IDM_TestStep_TraceServerSaveAdminObjs:
                {
                    doTestStepTraceServerSaveAdminObjs(hMenu);
                    break;
                }
                case IDM_TestStep_TraceClientConnect:
                {
                    doTestStepTraceClientConnect(hMenu);
                    break;
                }
                case IDM_TestStep_TraceClientDisconnect:
                {
                    doTestStepTraceClientDisconnect(hMenu);
                    break;
                }
                case IDM_TestStep_CreateModule1:
                {
                    doTestStepCreateModule1(hMenu);
                    break;
                }
                case IDM_TestStep_DeleteModule1:
                {
                    doTestStepDeleteModule1(hMenu);
                    break;
                }
                case IDM_TestStep_CreateModule2:
                {
                    doTestStepCreateModule2(hMenu);
                    break;
                }
                case IDM_TestStep_DeleteModule2:
                {
                    doTestStepDeleteModule2(hMenu);
                    break;
                }
                case IDM_EXIT:
                {
                    DestroyWindow(i_hWnd);
                    break;
                }
                default:
                {
                    return DefWindowProc(i_hWnd, i_uMessage, i_wParam, i_lParam);
                }
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(i_hWnd, &ps);
            // TODO: Zeichencode, der hdc verwendet, hier einfügen...
            EndPaint(i_hWnd, &ps);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(i_hWnd, i_uMessage, i_wParam, i_lParam);
        }
    }
    return 0;

}  // WndProc

//------------------------------------------------------------------------------
INT_PTR CALLBACK About( HWND hDlg, UINT i_uMessage, WPARAM i_wParam, LPARAM i_lParam )
//------------------------------------------------------------------------------
{
    UNREFERENCED_PARAMETER(i_lParam);

    switch (i_uMessage)
    {
        case WM_INITDIALOG:
        {
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            if (LOWORD(i_wParam) == IDOK || LOWORD(i_wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(i_wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
    }
    return (INT_PTR)FALSE;

} // About

//------------------------------------------------------------------------------
void showLastError( LPTSTR i_lpszFunction )
//------------------------------------------------------------------------------
{
    LPTSTR lpMsgBuf = nullptr;
    LPTSTR lpDisplayBuf = nullptr;
    SIZE_T uDisplayBufSize;
    DWORD  dw = GetLastError();

    FormatMessage(
        /* dwFlags      */ FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        /* lpSource     */ nullptr,
        /* dwMessageId  */ dw,
        /* dwLanguageId */ MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
        /* lpBuffer     */ (LPTSTR)&lpMsgBuf,
        /* nSize        */ 0,
        /* pArguments   */ nullptr );

    uDisplayBufSize = ( lstrlen((LPCTSTR)i_lpszFunction) + lstrlen((LPCTSTR)lpMsgBuf) + 60 ) * sizeof(TCHAR);
    lpDisplayBuf = (LPTSTR)LocalAlloc( LMEM_ZEROINIT, uDisplayBufSize );

    #pragma warning( disable : 4995 )
    wsprintf( lpDisplayBuf, _TEXT("%s failed with error %d: %s"), i_lpszFunction, dw, lpMsgBuf );
    #pragma warning( default : 4995 )

    MessageBox(
        /* hWnd      */ nullptr,
        /* lpText    */ (LPCTSTR)lpDisplayBuf,
        /* lpCaption */ TEXT("Error"),
        /* uType     */ MB_OK );

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

} // showLastError
