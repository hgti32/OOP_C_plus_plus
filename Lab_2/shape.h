#pragma once
#include <windows.h>
#include <cmath>

class Shape {
protected:
    long xs1, ys1, xs2, ys2;
public:
    Shape() : xs1(0), ys1(0), xs2(0), ys2(0) {}
    virtual ~Shape() {}
    void Set(long x1, long y1, long x2, long y2) {
        xs1 = x1; ys1 = y1; xs2 = x2; ys2 = y2;
    }
    virtual void Show(HDC hdc) = 0;
};

// Крапка
class PointShape : public Shape {
public:
    void Show(HDC hdc) override {
        SetPixel(hdc, xs1, ys1, RGB(0, 0, 0));
        SetPixel(hdc, xs1 + 1, ys1, RGB(0, 0, 0));
        SetPixel(hdc, xs1 - 1, ys1, RGB(0, 0, 0));
        SetPixel(hdc, xs1, ys1 + 1, RGB(0, 0, 0));
        SetPixel(hdc, xs1, ys1 - 1, RGB(0, 0, 0));
    }
};
//лінія
class LineShape : public Shape {
public:
    void Show(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HGDIOBJ oldPen = SelectObject(hdc, hPen);

        MoveToEx(hdc, xs1, ys1, NULL);
        LineTo(hdc, xs2, ys2);

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
    }
};

// Прямокутник
class RectShape : public Shape {
public:
    void Show(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

        long dx = std::abs(xs2 - xs1);
        long dy = std::abs(ys2 - ys1);
        Rectangle(hdc, xs1 - dx, ys1 - dy, xs1 + dx, ys1 + dy);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hPen);
    }
};

// Еліпс
class EllipseShape : public Shape {
public:
    void Show(HDC hdc) override {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 165, 0));

        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

        Ellipse(hdc, xs1, ys1, xs2, ys2);

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
};