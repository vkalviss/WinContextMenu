#include "stdafx.h"

#include <stdio.h>
#include <tchar.h>
#include <wchar.h>

#define SIZEOF(f) (sizeof(f) / sizeof(f[0]))

typedef struct _ENUM_POPUPS_INFO{
    DWORD dwProcID;
    int nNextHWNDIndex;
}ENUM_POPUPS_INFO;

static BOOL CALLBACK EnumPopupWindowsProc(HWND hWnd, LPARAM lParam);

int CountOpenWindows()
{
    ENUM_POPUPS_INFO epi = {0};
    BOOL bR;
 
    //Get our process ID
    memset(&epi, 0, sizeof(epi));
    epi.dwProcID = GetCurrentProcessId();

    bR = EnumWindows(EnumPopupWindowsProc, (LPARAM)&epi);

//    TRACE(L"CountOpenWindows = %d", epi.nNextHWNDIndex);
    return epi.nNextHWNDIndex;
}    


static BOOL CALLBACK EnumPopupWindowsProc(HWND hWnd, LPARAM lParam)
{
    ENUM_POPUPS_INFO* pEPI = (ENUM_POPUPS_INFO*)lParam;

    //Get process ID of the window
    DWORD dwProcID = 0;
    GetWindowThreadProcessId(hWnd, &dwProcID);

    //We need this window only if it's our process
    if(dwProcID == pEPI->dwProcID &&
//        pEPI->hThisWnd != hWnd &&
//        ((GetWindowLongPtr(hWnd, GWL_STYLE) & (WS_VISIBLE | WS_POPUP | WS_CAPTION)) == (WS_VISIBLE | WS_POPUP | WS_CAPTION))
       ( ((GetWindowLongPtr(hWnd, GWL_STYLE) & (WS_VISIBLE)) == (WS_VISIBLE))
         && ((GetWindowLongPtr(hWnd, GWL_STYLE) & (WS_VISIBLE | WS_CHILD)) != (WS_VISIBLE | WS_CHILD))
           )
        )
    {
        pEPI->nNextHWNDIndex++;
    }

    return TRUE;
}
