// HelloWindowsDesktop.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string>
using namespace std;


static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("Windows Desktop Application");

HINSTANCE hInst;

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

    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500,
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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hDC;
    HDC hdc;
    //для клави
    TEXTMETRIC tm;
    static string text;
    string symb;
    static int cxChar, cyChar, cxClient, cyClient;
    static int nCharPerLine, nClientLines;
    static int xCaret = 0, yCaret = 0;
    static int curIndex;
    static int nLines = text.size();
    static int nTailChar;
    HPEN hpen = CreatePen(PS_SOLID, 2, RGB(156, 156,156));
    //Для миші
    static int x, y;
    static BOOL bTracking = FALSE;
    static int x1, y1, x2, y2;
    static BOOL dTracking = FALSE;
    static HBRUSH hOldBrush;
    static HPEN hDotPen, hOldPen;
    UINT style;

    switch (message)
    {
    case WM_CREATE:
        hDC = GetDC(hWnd);
        style = GetClassLong(hWnd, GCL_STYLE);
        SetClassLong(hWnd, GCL_STYLE, style | CS_DBLCLKS);
        hDotPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
        hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
        SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
        SelectObject(hDC, hpen);
        GetTextMetrics(hDC, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight;
        break;
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        nCharPerLine = max(1, cxClient / cxChar);
        nClientLines = max(1, cyClient / cyChar);
        if (hWnd == GetFocus())
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        break;
    case WM_SETFOCUS:
        CreateCaret(hWnd, NULL, 0, cyChar);
        SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        ShowCaret(hWnd);
        break;
    case WM_KILLFOCUS:
        HideCaret(hWnd);
        DestroyCaret();
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            xCaret = max(xCaret - 1, 0);
            break;
        case VK_RIGHT: xCaret = min(xCaret + 1, nCharPerLine - 1);
            if ((yCaret == nLines) && (xCaret > nTailChar))
                xCaret = nTailChar;
            break;
        case VK_UP: yCaret = max(yCaret - 1, 0);
            break;
        case VK_DOWN: yCaret = min(yCaret + 1, nLines);
            if ((yCaret == nLines) && (xCaret > nTailChar))
                xCaret = nTailChar;
            break;
        default:
            break;
        }
        SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        break;
    case WM_CHAR:
        curIndex = yCaret * nCharPerLine + xCaret;
        if (curIndex == text.size())
            text += (char)wParam;
        else {
            symb = (char)wParam;
            text.insert(curIndex, symb);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        if (++xCaret == nCharPerLine) {
            xCaret = 0;
            if (++yCaret == nClientLines) {
                MessageBox(hWnd, L"Немає місця у вікні", L"Error", MB_OK);
                yCaret--;
            }
            break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
        if (text.size()) {
            std::wstring widestr = std::wstring(text.begin(), text.end());
            const wchar_t* widecstr = widestr.c_str();
            nLines = text.size() / nCharPerLine;
            nTailChar = text.size() % nCharPerLine;
            for (y = 0; y < nLines; ++y)
                TextOut(hDC, 0, y * cyChar, widecstr + y * nCharPerLine,
                    nCharPerLine);
            TextOut(hDC, 0, y * cyChar, widecstr + y * nCharPerLine,
                nTailChar);
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_LBUTTONDOWN:
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
    case WM_RBUTTONDOWN:
        dTracking = TRUE;
        SetROP2(hDC, R2_NOTXORPEN);
        x1 = x2 = LOWORD(lParam);
        y1 = y2 = HIWORD(lParam);
        hOldPen = (HPEN)SelectObject(hDC, hDotPen);
        Rectangle(hDC, x1, y1, x2, y2);
        break;
    case WM_RBUTTONUP:
        if (dTracking) {
            dTracking = FALSE;
            SetROP2(hDC, R2_COPYPEN);
            x2 = LOWORD(lParam);
            y2 = HIWORD(lParam);
            SelectObject(hDC, hOldPen);
            Rectangle(hDC, x1, y1, x2, y2);
        }
        break;
    case WM_DESTROY:
        ReleaseDC(hWnd, hDC);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
        }

        return 0;
    }
}