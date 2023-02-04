// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

// Global variables
static HPEN hPen;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ColorDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 800,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    HWND hColor = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hWnd, ColorDlgProc);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsDialogMessage(hColor, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}
static HPEN hDotPen, hOldPen;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    
    static HDC hDC; 
    static int x, y;
    static BOOL bTracking = FALSE;
    static int x1, y1, x2, y2;
    static BOOL dTracking = FALSE;
    static HBRUSH hOldBrush;
    UINT style;
    // "Open", "Save As"
    static OPENFILENAME ofn;
    static char szFile[MAX_PATH];
    // "Color"
    static CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];
    //"Font"
    static CHOOSEFONT chf;
    static HFONT hFont;
    static LOGFONT lf;

    RECT rcClient;
    BOOL success;
    static COLORREF textColor;
    switch (message)
    {
    case WM_CREATE:
        hDC = GetDC(hWnd);
        style = GetClassLong(hWnd, GCL_STYLE);
        SetClassLong(hWnd, GCL_STYLE, style | CS_DBLCLKS);
        SelectObject(hDC, hPen);
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = (LPWSTR)szFile;
        ofn.nMaxFile = sizeof(szFile);

        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hWnd;
        cc.lpCustColors = (LPDWORD)acrCustClr;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

        chf.lStructSize = sizeof(CHOOSEFONT);
        chf.hwndOwner = hWnd;
        chf.lpLogFont = &lf;
        chf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
        chf.nFontType = SIMULATED_FONTTYPE;
        break;
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, textColor);
        if (hFont)
            DeleteObject(SelectObject(hDC, hFont));
        GetClientRect(hWnd, &rcClient);
        DrawText(hDC, L"Frog on the beach", -1, &rcClient,
            DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_FILE_OPEN:
            strcpy_s(szFile, "");
            success = GetOpenFileName(&ofn);
            if (success)
                MessageBox(hWnd, ofn.lpstrFile, L"Відкривається файл", MB_OK);
            else
                MessageBox(hWnd, L"GetOpenFileName",
                    L"Відмова", MB_ICONWARNING);
            break;
        case ID_FILE_SAVE:
            strcpy_s(szFile, "");
            success = GetSaveFileName(&ofn);
            if (success)
                MessageBox(hWnd, ofn.lpstrFile,
                    L"Файл зьерагіється", MB_OK);
            else
                MessageBox(hWnd, L"GetSaveFileName",
                    L"Відмова", MB_ICONWARNING);
            break;
        case ID_BGR:
            if (ChooseColor(&cc))
                SetClassLong(hWnd, GCL_HBRBACKGROUND,
                    (LONG)CreateSolidBrush(cc.rgbResult));
            InvalidateRect(hWnd, NULL, TRUE);

            break;
        case ID_TEXT:
            if (ChooseColor(&cc)) textColor = cc.rgbResult;
            InvalidateRect(hWnd, NULL, TRUE);

            break;
        
        case ID_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);
            break;
        case ID_COLOR:
            
            ShowWindow(CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, ColorDlgProc), SW_SHOW);
            break;
        case ID_FONT:
            if (ChooseFont(&chf)) hFont = CreateFontIndirect(chf.lpLogFont);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    case WM_LBUTTONDOWN:
        
        SelectObject(hDC, hPen);
        bTracking = TRUE;
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        MoveToEx(hDC, x, y, NULL);
        break;
    case WM_LBUTTONUP:
        bTracking = FALSE;
        break;
    case WM_MOUSEMOVE:
        if (bTracking) {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            LineTo(hDC, x, y);
        }
        else if (dTracking)
        {
            Rectangle(hDC, x1, y1, x2, y2);
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            Rectangle(hDC, x1, y1, x2, y2);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG:
            return TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg, 0);
                return TRUE;
            }
        break;
    }
    return FALSE;
}
static COLORREF color = RGB(255,255,255);
BOOL CALLBACK ColorDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HDC hColordc;
    int xPos, yPos;
    switch (uMsg) {
    case WM_INITDIALOG:
        hColordc = GetDC(hDlg);
        return TRUE;
    case WM_LBUTTONDOWN:
        xPos = LOWORD(lParam);
        yPos = HIWORD(lParam);

        color = GetPixel(hColordc, xPos, yPos);
        hOldPen = hPen;
        DeleteObject(hOldPen);
        hPen = CreatePen(PS_SOLID, 2, color);
        return true;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        
        case IDOK:

        case IDCANCEL:
            EndDialog(hDlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}