#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "resource.h"

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSomething(HDC hdc);

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
    wcex.lpszMenuName = NULL;
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
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1000, 1000,
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
    TCHAR greeting[] = _T("Лабораторна робота №2");
    POINT pt[11] = { {50,0}, {55, 20},{70,20},{60,30},{65, 55},{50,40},{35,55},{40, 30},{25,20},{45,20},{50,0} };
    RECT rect;
    static HDC hdcEMF;
    HENHMETAFILE hemf;
    HBITMAP bitmap;
    static BITMAP bm;
    HDC hMemDC;
    static HDC memBit;

    switch (message)
    {
    case WM_CREATE:
        hdcEMF = CreateEnhMetaFile(NULL, L"Pict1.emf", NULL, L"CreateMetaFile\0Pict1\0");
        DrawSomething(hdcEMF);
        hemf = CloseEnhMetaFile(hdcEMF);
        DeleteEnhMetaFile(hemf);

        bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PNG1));
        GetObject(bitmap, sizeof(bm), &bm);
        hdc = GetDC(hWnd);
        memBit = CreateCompatibleDC(hdc);
        SelectObject(memBit, bitmap);
        ReleaseDC(hWnd, hdc);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetWindowRect(hWnd, &rect);

        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memBit, 0, 0, SRCCOPY);

        Polyline(hdc, pt, 11);
        RECT r;
        HPEN hPen, hOldPen;
        HBRUSH hBrush, hOldBrush;
        SetRect(&r, 100, 100, 300, 200);
        hPen = CreatePen(PS_SOLID, 10, RGB(255, 160, 140));
        hBrush = CreateSolidBrush(RGB(140, 160, 255));
        SelectObject(hdc, hPen);
        SelectObject(hdc, hBrush);

        Ellipse(hdc, r.left, r.top, r.right, r.bottom);
        SetTextColor(hdc, RGB(0, 0, 255));
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, greeting, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

        hemf = GetEnhMetaFile(L"Pict1.emf");
        SetRect(&rect, 320, 320, 500, 500);
        PlayEnhMetaFile(hdc, hemf, &rect);
        DeleteEnhMetaFile(hemf);

        EndPaint(hWnd, &ps);
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

void DrawSomething(HDC hdc)
{
    RECT r;
    HPEN hPen, hOldPen;
    HBRUSH hBrush, hOldBrush;
    SetRect(&r, 250, 250, 700, 700);
    hPen = CreatePen(PS_SOLID, 8, RGB(0, 0, 0));
    hBrush = CreateSolidBrush(RGB(0, 0, 255));
    
    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);
   
    
    Rectangle(hdc, r.left, r.top, r.right, r.bottom);

    

    DeleteObject(hPen);
    DeleteObject(hBrush);

    hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    hBrush = CreateSolidBrush(RGB(255, 255, 0));
    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);

    Pie(hdc, 350, 450, 400, 500, 500, 450, 500, 500);
    Pie(hdc, 450, 450, 500, 500, 500, 450, 500, 500);
    Pie(hdc, 550, 450, 600, 500, 650, 450, 650, 500);
}