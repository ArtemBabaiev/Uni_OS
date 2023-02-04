// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string>
#include "resource.h"
#define IDB_BUT 100
#define ID_LIST 103
#define ID_COMBO 104
#define ID_TEXT 105
#define ID_LABEL 106
#define IDB_BUT2 107

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Application");
HWND hList;
HWND hEdit;
HWND hLabel;

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        CW_USEDEFAULT, CW_USEDEFAULT,
        510, 500,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    HWND hwndButton = CreateWindow(
        L"BUTTON",  
        L"RESET",       
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,   
        365,         
        365,         
        100,         
        50,        
        hWnd,      
        (HMENU) IDB_BUT,      
        NULL,
        NULL);    

    hEdit = CreateWindow(L"EDIT", 0, WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 400, 70, 30, hWnd, (HMENU)ID_TEXT, NULL, NULL);
    hLabel = CreateWindow(L"STATIC", 0, WS_TABSTOP | WS_CHILD | WS_VISIBLE, 10, 350, 70, 30, hWnd, (HMENU)ID_LABEL, NULL, NULL);
    HWND hwndButtonSub = CreateWindow(
        L"BUTTON",
        L"SUBMIT",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        90,
        350,
        80,
        50,
        hWnd,
        (HMENU)IDB_BUT2,
        NULL,
        NULL);


    hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 350, 10, 100, 80, hWnd, (HMENU)ID_LIST, NULL, NULL);
    SendMessage(hList, WM_SETREDRAW, FALSE, 0L);
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 1");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 2");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 3");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 4");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 5");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 6");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 7");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 8");
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)L"Element 9");
    SendMessage(hList, WM_SETREDRAW, TRUE, 0L);
    InvalidateRect(hList, NULL, TRUE);

    HWND hWndComboBox = CreateWindow(L"COMBOBOX", TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        350, 100, 100, 8000, hWnd, (HMENU)ID_COMBO, NULL, NULL);
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 1");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 2");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 3");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 4");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 5");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 6");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 7");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 8");
    SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"Combo 9");
    SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static POINT point, ptInClient;
    static HMENU hMenuMyContext;
    HPEN hpen;

    
    switch (message)
    {
        case WM_CREATE:
            hMenuMyContext;
            hMenuMyContext = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_CONTEXT));
            hMenuMyContext = GetSubMenu(hMenuMyContext, 0);
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            // Here your application is laid out.
            // For this introduction, we just print out "Hello, Windows desktop!"
            // in the top left corner.
            TextOut(hdc,
                5, 5,
                _T("Hello, Windows desktop!"), _tcslen(_T("Hello, Windows desktop!")));
            // End application-specific layout section.

            EndPaint(hWnd, &ps);
            break;
        case WM_COMMAND:
            hdc = GetDC(hWnd);
            hpen = CreatePen(PS_SOLID, 2, RGB(250, 250, 250));
            switch (LOWORD(wParam))
            {
                case ID_OPEN_NEW:
                    MessageBox(hWnd, _T("New"), _T("File menu"), MB_OK);
                    break;
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                    break;
                case ID_CREATEFIGURE_CIRCLE:     
                    Ellipse(hdc, point.x, point.y, point.x + 70, point.y + 70);
                    break;
                case ID_CREATEFIGURE_ELIPSE:
                    Ellipse(hdc, point.x, point.y, point.x + 100, point.y + 60);
                    break;
                case ID_CREATEFIGURE_RECT:
                    Rectangle(hdc, point.x, point.y, point.x + 120, point.y + 60);
                    break;
                case ID_CREATEFIGURE_SQUARE:
                    Rectangle(hdc, point.x, point.y, point.x + 100, point.y + 100);
                    break;
                case IDB_BUT:
                    InvalidateRect(hWnd, NULL, TRUE);
                    break;
                case ID_LIST:
                    if (HIWORD(wParam)== LBN_SELCHANGE)
                    {
                        wchar_t lbvalue[30];
                        
                        int sel = (int)SendMessageW(hList, LB_GETCURSEL, 0, 0);
                        SendMessage(hList, LB_GETTEXT, sel, (LPARAM)lbvalue);
                        MessageBox(hWnd, lbvalue, _T("Вибрано"), MB_OK);
                    }
                    break;
                case IDB_BUT2:
                    //const int len = GetWindowTextLength(hLabel);
                    wchar_t lbvalue[100];
                    GetWindowText(hEdit, lbvalue, 100);
                    SetWindowText(hLabel, lbvalue);
                    break;
                /*case ID_COMBO:
                    if (HIWORD(wParam) == CBN_SELCHANGE) 
                    {
                        int sel = (int)SendMessageW(hList, CB_GETCURSEL, 0, 0);
                        MessageBox(hWnd, _T("Операція із випадаючим списком"), _T("Вибрано"), MB_OK);
                    }*/
                default:
                    break;
            }
            ReleaseDC(hWnd, hdc);
            break;
        case WM_RBUTTONDOWN:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            ptInClient = point;
            ClientToScreen(hWnd, &ptInClient);
            TrackPopupMenuEx(hMenuMyContext, 0, ptInClient.x, ptInClient.y, hWnd, NULL);
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