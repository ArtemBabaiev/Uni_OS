// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <process.h>
#include <commctrl.h>
#pragma comment (lib, "comctl32")
#include "resource.h"

static TCHAR szWindowClass[] = _T("DesktopApp");


static TCHAR szTitle[] = _T("Windows Desktop Application");

HINSTANCE hInst;
HANDLE hSemaphore;
INT_PTR CALLBACK Dialog1(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND SetElement(HWND hDlg, HDC& mem, WORD IdBitmap, WORD IdProgress);
unsigned __stdcall MyThread1(void* param);
unsigned __stdcall MyThread2(void* param);
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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

    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
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

    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
TCHAR CommandLine[256] = _T("notepad");

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;


    switch (message)
    {
    case WM_CREATE:

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc,
            5, 5,
            _T("Hello, Windows desktop!"), _tcslen(_T("Hello, Windows desktop!")));
        EndPaint(hWnd, &ps);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_SEMAPHORE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog1);
            break;
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
HWND SetElement(HWND hDlg, HDC& mem, WORD IdBitmap, WORD IdProgress)
{
    HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IdBitmap));
    HDC hdc = GetDC(hDlg);
    mem = CreateCompatibleDC(hdc);
    ReleaseDC(hDlg, hdc);
    SelectObject(mem, hBitmap);
    HWND handle = GetDlgItem(hDlg, IdProgress);
    SendMessage(handle, PBM_SETRANGE, 0, 30 << 16);
    SendMessage(handle, PBM_SETSTEP, 1, 0);
    SendMessage(handle, PBM_SETPOS, 0, 0);
    return handle;
}

unsigned __stdcall MyThread1(void* param)
{
    HWND hWnd = (HWND)param;
    HDC hdc, mem;
    int t = 0;
    HWND hProgress = SetElement(hWnd, mem, IDB_BITMAP1, IDC_PROGRESS1);
    while (1)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        Sleep(500);
        hdc = GetDC(hWnd);
        BitBlt(hdc, 200, 150, 25, 50, mem, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdc);
        if (++t > 30) t = 0;
        SendMessage(hProgress, PBM_SETPOS, t, 0);
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
    return 0;
}
unsigned __stdcall MyThread2(void* param)
{
    HWND hWnd = (HWND)param;
    HDC hdc, mem;
    int t = 0;
    HWND hProgress = SetElement(hWnd, mem, IDB_BITMAP2, IDC_PROGRESS2);
    while (1)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        Sleep(500);
        hdc = GetDC(hWnd);
        BitBlt(hdc, 200, 150, 25, 50, mem, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdc);
        if (++t > 30) t = 0;
        SendMessage(hProgress, PBM_SETPOS, t, 0);
        ReleaseSemaphore(hSemaphore, 1, NULL);
    }
    return 0;
}
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HANDLE hThread1, hThread2;
    switch (message)
    {
    case WM_INITDIALOG:
        InitCommonControls();
        hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
        hThread1 = (HANDLE)_beginthreadex(NULL, 0, MyThread1, hDlg, 0, NULL);
        hThread2 = (HANDLE)_beginthreadex(NULL, 0, MyThread2, hDlg, 0, NULL);
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            TerminateThread(hThread1, 0);
            TerminateThread(hThread2, 0);
            CloseHandle(hThread1);
            CloseHandle(hThread2);
            EndDialog(hDlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}