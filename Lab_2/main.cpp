#include <windows.h>
#include <cmath>
#include "resource.h"
#include "shape.h"

using namespace std;

#define MAX_SHAPES 123

HINSTANCE hInst;
WCHAR szTitle[] = L"Lab2";
WCHAR szWindowClass[] = L"LAB2_WINDOW_CLASS";

Shape* pcshape[MAX_SHAPES] = { nullptr };
int shapeCount = 0;

enum ShapeType { TYPE_POINT, TYPE_LINE, TYPE_RECT, TYPE_ELLIPSE };
ShapeType currentType = TYPE_RECT;
bool isDrawing = false;
long startX = 0, startY = 0;
long prevX = 0, prevY = 0;

void UpdateTitle(HWND hWnd) {
    const wchar_t* typeName = L"";
    switch (currentType) {
    case TYPE_POINT:   typeName = L"Крапка"; break;
    case TYPE_LINE:    typeName = L"Лінія"; break;
    case TYPE_RECT:    typeName = L"Прямокутник"; break;
    case TYPE_ELLIPSE: typeName = L"Еліпс"; break;
    }
    wchar_t titleBuf[128];
    wsprintfW(titleBuf, L"Lab2 - [Поточний об'єкт: %s]", typeName);
    SetWindowTextW(hWnd, titleBuf);
}

void DrawRubberTrack(HDC hdc, long x1, long y1, long x2, long y2, ShapeType type) {
    int oldRop = SetROP2(hdc, R2_NOTXORPEN);
    HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, hPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));

    switch (type) {
    case TYPE_POINT:
        break;
    case TYPE_LINE:
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        break;
    case TYPE_RECT: {
        long dx = abs(x2 - x1);
        long dy = abs(y2 - y1);
        Rectangle(hdc, x1 - dx, y1 - dy, x1 + dx, y1 + dy);
        break;
    }
    case TYPE_ELLIPSE:
        Ellipse(hdc, x1, y1, x2, y2);
        break;
    }

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hPen);
    SetROP2(hdc, oldRop);
}

Shape* CreateShape(ShapeType type, long x1, long y1, long x2, long y2) {
    Shape* s = nullptr;
    switch (type) {
    case TYPE_POINT:   s = new PointShape(); break;
    case TYPE_LINE:    s = new LineShape(); break;
    case TYPE_RECT:    s = new RectShape(); break;
    case TYPE_ELLIPSE: s = new EllipseShape(); break;
    }
    if (s) s->Set(x1, y1, x2, y2);
    return s;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        UpdateTitle(hWnd);
        break;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case IDM_SHAPE_POINT:
            currentType = TYPE_POINT;
            UpdateTitle(hWnd);
            break;
        case IDM_SHAPE_LINE:
            currentType = TYPE_LINE;
            UpdateTitle(hWnd);
            break;
        case IDM_SHAPE_RECT:
            currentType = TYPE_RECT;
            UpdateTitle(hWnd);
            break;
        case IDM_SHAPE_ELLIPSE:
            currentType = TYPE_ELLIPSE;
            UpdateTitle(hWnd);
            break;
        case IDM_ABOUT:
            MessageBoxW(hWnd, L"Лабораторна робота №2\nВаріант 23", L"Про програму", MB_OK | MB_ICONINFORMATION);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
        break;
    }

    case WM_LBUTTONDOWN: {
        long x = LOWORD(lParam);
        long y = HIWORD(lParam);

        if (currentType == TYPE_POINT) {
            if (shapeCount < MAX_SHAPES) {
                pcshape[shapeCount++] = CreateShape(TYPE_POINT, x, y, x, y);
                InvalidateRect(hWnd, NULL, TRUE);
            }
        } else {
            isDrawing = true;
            startX = prevX = x;
            startY = prevY = y;
            SetCapture(hWnd);
        }
        break;
    }

    case WM_MOUSEMOVE: {
        if (isDrawing) {
            long curX = LOWORD(lParam);
            long curY = HIWORD(lParam);

            HDC hdc = GetDC(hWnd);
            DrawRubberTrack(hdc, startX, startY, prevX, prevY, currentType);
            DrawRubberTrack(hdc, startX, startY, curX, curY, currentType);
            ReleaseDC(hWnd, hdc);

            prevX = curX;
            prevY = curY;
        }
        break;
    }

    case WM_LBUTTONUP: {
        if (isDrawing) {
            long endX = LOWORD(lParam);
            long endY = HIWORD(lParam);

            HDC hdc = GetDC(hWnd);
            DrawRubberTrack(hdc, startX, startY, prevX, prevY, currentType);
            ReleaseDC(hWnd, hdc);

            isDrawing = false;
            ReleaseCapture();

            if (shapeCount < MAX_SHAPES) {
                pcshape[shapeCount++] = CreateShape(currentType, startX, startY, endX, endY);
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        for (int i = 0; i < shapeCount; ++i) {
            if (pcshape[i]) {
                pcshape[i]->Show(hdc);
            }
        }

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        for (int i = 0; i < shapeCount; ++i) {
            delete pcshape[i];
            pcshape[i] = nullptr;
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MAIN_MENU);
    wcex.lpszClassName = szWindowClass;

    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}