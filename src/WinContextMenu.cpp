// WinContextMenu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CmdLineParser.hpp"
#include <shlobj.h>
#include <shellapi.h>
#include <stdio.h>
#include <tchar.h>
#include <wchar.h>


#define SIZEOF(f) (sizeof(f) / sizeof(f[0]))


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR* szTitle=L"WinContextMenu";       // The title bar text
TCHAR* szWindowClass=L"WINCONTEXTMENU"; // the main window class name

bool ShowWinContextMenu(HWND hWnd);
IContextMenu2 *g_pcm2;
IContextMenu3 *g_pcm3;

CmdLineParser g_cmdLnParser;

int CountOpenWindows();
UINT nTimerID = 0;
int nExitCounter = 0;
int nExitWndWarmUp = 0;

#define EXIT_WND_COUNT (2)
#define  EXIT_WND_WARM_UP (20)

//#define TRACE(x) OutputDebugString(x)


class COleInitializer
{
public:
    COleInitializer() { m_hr = OleInitialize(NULL); }
    ~COleInitializer() { OleUninitialize(); }

    HRESULT GetHRESULT() const { return m_hr; }
private:
    HRESULT m_hr;
};

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    TRACE(L"Starting WinContextMenu with parameter [%s]", lpCmdLine);

    COleInitializer oleInit;
    TRACE(L" OleInitialize = 0X%X", oleInit.GetHRESULT());

    if (lpCmdLine == NULL || _tcslen(lpCmdLine) == 0)
        return 1;

    int argc = 0;
    LPCWSTR * pargv = const_cast<LPCWSTR*> ( CommandLineToArgvW(lpCmdLine, &argc) );

    g_cmdLnParser.Parse(argc, pargv);

    g_cmdLnParser.ConvertBackSlashes();


    if (g_cmdLnParser.GetFileNameCount() < 1) 
    {
        TRACE(L" WinContextMenu - no file names specified");
    }
    else
    {
        TRACE(L" WinContextMenu FileName[0] =  [%s]", g_cmdLnParser.GetFileName(0));
    }

    
	MSG msg;

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

    // Create timer - 50 msec
    nTimerID = (UINT)SetTimer(NULL, 0, 50, NULL);

    TRACE(L"nTimerID = %d", nTimerID);

    if (nTimerID == 0)
        return FALSE;


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{

        if (msg.message == WM_TIMER && msg.wParam == nTimerID)
        {

            if (nExitWndWarmUp < EXIT_WND_WARM_UP)
                nExitWndWarmUp ++;
            else
            {
                int nWndCount = CountOpenWindows();

//                TRACE(L"nWndCount = %d", nWndCount);

                if (nWndCount <= 1)
                    nExitCounter++;
                else
                    nExitCounter = 0;

                if (nExitCounter > EXIT_WND_COUNT)
                {
                    TRACE(L"Exiting the application");

                    PostQuitMessage(0);
                }
            }
        }



//		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //
	wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm        = NULL;

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   // hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   //    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUPWINDOW,
      300, 0, 300, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   bool bMenu = ShowWinContextMenu(hWnd);

   return bMenu ? TRUE : FALSE;
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if (g_pcm3) {
		LRESULT lres;
		if (SUCCEEDED(g_pcm3->HandleMenuMsg2(message, wParam, lParam, &lres))) {
			return lres;
		}
	} else if (g_pcm2) {
		if (SUCCEEDED(g_pcm2->HandleMenuMsg(message, wParam, lParam))) {
			return 0;
		}
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

        return DefWindowProc(hWnd, message, wParam, lParam);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


HRESULT GetUIObjectOfFileEx(HWND hwnd, const CmdLineParser& cmdLineParams, REFIID riid, void **ppv)
{
    HRESULT hr = E_INVALIDARG;
    *ppv = NULL;

    if (cmdLineParams.GetFileNameCount()==0)
        return hr;

    size_t count = cmdLineParams.GetFileNameCount();
    LPITEMIDLIST *pidlArray = new LPITEMIDLIST[count];
    LPCITEMIDLIST *pidlChildArray = new LPCITEMIDLIST[count];

    IShellFolder *psfMain = NULL;

    for( int i=0; i<count; ++i)
    {
        SFGAOF sfgao;
        if (SUCCEEDED(hr = SHParseDisplayName(cmdLineParams.GetFileName(i), NULL, &pidlArray[i], 0, &sfgao))) 
        {
            IShellFolder *psf;
            if (SUCCEEDED(hr = SHBindToParent(pidlArray[i], IID_IShellFolder, (void**)&psf, &pidlChildArray[i]))) 
            {

                if (psfMain==NULL)
                    psfMain = psf;
                else
                    psf->Release();
            }
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = psfMain->GetUIObjectOf(hwnd, (UINT)count, pidlChildArray
                                    // (LPCITEMIDLIST*)pidlArray
                                    , riid, NULL, ppv);
    }

    if ( psfMain )
        psfMain->Release();

    for( int i=0; i<count; ++i)
    {
        CoTaskMemFree(pidlArray[i]);
    }

    delete[] pidlChildArray;
    delete[] pidlArray;

	return hr;
}

#define SCRATCH_QCM_FIRST 1
#define SCRATCH_QCM_LAST  0x7FFF


WNDPROC wpOrigEditProc; 

int m_activeMenuLayers = 0;
HHOOK m_hookHandle = 0;

LRESULT CALLBACK GetMsgHookFunction(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
        return ::CallNextHookEx(m_hookHandle, code, wParam, lParam);

    LPMSG pMsg = (LPMSG)(lParam);

    // {
    //     UINT msg = pMsg->message;

    //     switch(msg)
    //     {
    //     case WM_ENTERIDLE:
    //     case 0x94:
    //         break;

    //     default:
    //         TRACE(L"HookFunc 0x%X", pMsg->message);
    //     }
    // }

    if (WM_CREATE == pMsg->message)
    {
        CREATESTRUCT *pCS = (CREATESTRUCT*)pMsg->lParam;
        HWND hWnd = pMsg->hwnd;
        TCHAR strClassName[100]={0};
        
        GetClassName(hWnd, strClassName, SIZEOF(strClassName));
        LPCTSTR strName = pCS->lpszName; // L""; //pCS->lpszName;

        TRACE(L"WM_CREATE hWnd=0x%X [%s], [%s] 0x%X 0x%X", hWnd, strClassName, strName, pCS->style, pCS->dwExStyle);
    }

    if (WM_KEYDOWN == pMsg->message  || WM_SYSKEYDOWN == pMsg->message)
    {
        UINT c = (UINT)pMsg->wParam;
        //BOOL bCTRL = (GetKeyState(VK_CONTROL) < 0);

        bool bControlShiftAlt = 
            (GetKeyState(VK_CONTROL) < 0) &&
            ! (GetKeyState(VK_SHIFT) < 0) &&
            ! (GetKeyState(VK_MENU) < 0);

        if (bControlShiftAlt) {
            switch(c)
            {
            case 'N':
                pMsg->wParam = VK_DOWN;
                break;

            case 'P':
                pMsg->wParam = VK_UP;
                break;

            case 'F':
                pMsg->wParam = VK_RIGHT;
                break;

            case 'B':
                pMsg->wParam = VK_LEFT;
                break;

            case 'G':
            case 'C':
                pMsg->wParam = VK_ESCAPE;
                break;
            }
        }
    }
    
    return ::CallNextHookEx(m_hookHandle, code, wParam, lParam);
}

void InstallHook()
{
    // m_hookHandle = SetWindowsHookEx(
    //         WH_CALLWNDPROCRET, 
    //         (HOOKPROC)HookFunction, 
    //         NULL, 
    //         GetCurrentThreadId());

    m_hookHandle = SetWindowsHookEx(
        WH_GETMESSAGE,
        (HOOKPROC)GetMsgHookFunction, 
        NULL, 
        GetCurrentThreadId());
}

void UninstallHook()
{
    UnhookWindowsHookEx(m_hookHandle);
}
 


bool OnContextMenu(HWND hwnd, UINT xPos, UINT yPos, const CmdLineParser& cmdLineParams)
{
    bool bRet = false;

	POINT pt = { xPos, yPos };
	if (pt.x == -1 && pt.y == -1) 
    {
		pt.x = pt.y = 0;
		ClientToScreen(hwnd, &pt);
	}

	IContextMenu *pcm;
	if (SUCCEEDED(GetUIObjectOfFileEx(hwnd, 
                                      cmdLineParams,
                                      IID_IContextMenu, (void**)&pcm)))
	{

		pcm->QueryInterface(IID_IContextMenu2, (void**)&g_pcm2);
		pcm->QueryInterface(IID_IContextMenu3, (void**)&g_pcm3);

		IExplorerCommand * pECmd = NULL;
		pcm->QueryInterface(IID_IExplorerCommand, (void**)&pECmd);


		IContextMenu* pcmMenu = pcm;
		if(g_pcm2 != NULL)
			pcmMenu = g_pcm3;

		if(g_pcm3 != NULL)
			pcmMenu = g_pcm3;

		HMENU hmenu = CreatePopupMenu();
		if (hmenu) 
        {
			if (SUCCEEDED(pcmMenu ->QueryContextMenu(hmenu, 0,
                                                     SCRATCH_QCM_FIRST, SCRATCH_QCM_LAST,
                                                     ///CMF_DEFAULTONLY )))
                                                     CMF_NORMAL
                                                     //CMF_CANRENAME
                                                     //|CMF_ITEMMENU|CMF_EXTENDEDVERBS|CMF_SYNCCASCADEMENU   
                              ))) 
			{

				int iCmd = TrackPopupMenuEx(hmenu, TPM_RETURNCMD,
                                            pt.x, pt.y, hwnd, NULL);


				if (iCmd > 0) 
                {

                    bool bImmediateCmd = false;

                    {
                        WCHAR szVerb[100] = {0};
                        HRESULT hRes = pcmMenu->GetCommandString(iCmd - SCRATCH_QCM_FIRST, GCS_VERBW, NULL, (LPSTR)szVerb, SIZEOF(szVerb));

                        if (SUCCEEDED(hRes))
                        {
                            TRACE(L"WinContextMenu Verb =  [%s]", szVerb);
                        
                            if (!lstrcmpW(szVerb, L"copy") ||
                                !lstrcmpW(szVerb, L"cut") ||
                                !lstrcmpW(szVerb, L"rename")
//                                !lstrcmpW(szVerb, L"paste")
                                )
                            {
                                bImmediateCmd = true;
                            }
                        }
                    }

                    if (!bImmediateCmd)
                        bRet = true;

					CMINVOKECOMMANDINFOEX info = { 0 };
					info.cbSize = sizeof(info);
					info.fMask = CMIC_MASK_UNICODE | CMIC_MASK_PTINVOKE;
					if (GetKeyState(VK_CONTROL) < 0) {
						info.fMask |= CMIC_MASK_CONTROL_DOWN;
					}
					if (GetKeyState(VK_SHIFT) < 0) {
						info.fMask |= CMIC_MASK_SHIFT_DOWN;
					}

					info.hwnd = hwnd;
					info.lpVerb  = MAKEINTRESOURCEA(iCmd - SCRATCH_QCM_FIRST);
					info.lpVerbW = MAKEINTRESOURCEW(iCmd - SCRATCH_QCM_FIRST);
					info.nShow = SW_SHOWNORMAL;
					info.ptInvoke = pt;
					pcmMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);

                    OleFlushClipboard();
				}


			}
			DestroyMenu(hmenu);
		}
		if (g_pcm2) {
			g_pcm2->Release();
			g_pcm2 = NULL;
		}
		if (g_pcm3) {
			g_pcm3->Release();
			g_pcm3 = NULL;
		}

		pcm->Release();
	}

    return bRet;
}

bool ShowWinContextMenu(HWND hWnd)
{
    InstallHook();
	bool bRet = OnContextMenu(hWnd, 100, 100, g_cmdLnParser);
    UninstallHook();
    return bRet;
}


void MyDebugOutput(const TCHAR* szFormat, ...)
{
	va_list marker;
	va_start( marker, szFormat );     /* Initialize variable arguments. */

	TCHAR buf[500]={0};

    _vsntprintf_s(buf, SIZEOF(buf), _TRUNCATE, szFormat, marker);

    OutputDebugString(buf);

	va_end( marker );              
}
