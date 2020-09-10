#pragma once
#include <iostream>
#include "framework.h"
#include <sstream>
#include <cmath>

constexpr double M_PI = 3.14159265358979323846;

typedef std::basic_stringstream<TCHAR> TStringStream;

class Figure
{
protected:
	std::pair <int, int> center;
	int radius;
public:
	Figure() : center{ 0, 0 }, radius{ 0 } {};
	Figure(std::pair <int, int> _center, int _radius = 0) : center{ _center }, radius{ _radius }{};

	void PrintInfo(HDC hdc, LONG height, COLORREF colorin, COLORREF colorout)
	{
		TStringStream text;
		text << L"        Тип фигуры:" << strchr(typeid(*this).name(), L' ');
		text << L".                  Цвет кисти R:" << GetRValue(colorin) << L" G:" << GetGValue(colorin) << L" B:" << GetBValue(colorin);
		text << L".                 Цвет обводки R:" << GetRValue(colorout) << L" G:" << GetGValue(colorout) << L" B:" << GetBValue(colorout);
		text << L".";
		TextOut(hdc, 0, height - 75, text.str().c_str(), lstrlen(text.str().c_str()));
	}


	virtual void SetCords(std::pair <int, int> _center, int _radius)
	{
		center.first = _center.first;
		center.second = _center.second;
		radius = _radius;
	}

	virtual void Move(std::pair <int, int> speed)
	{
		center.first += speed.first;
		center.second += speed.second;
	}


	int GetX()
	{
		return center.first;
	}
	int GetY()
	{
		return center.second;
	}
	int GetR()
	{
		return radius;
	}
	virtual void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) = 0;
};

class Circle :public Figure
{

public:
	Circle() : Figure() {};
	Circle(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius) {};
	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Ellipse(hdc, center.first - radius, center.second - radius, center.first + radius, center.second + radius);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};

class Rhombus :public Figure
{
private:
	POINT points[4];
public:
	Rhombus() : Figure(), points{ 0,0,0,0 }
	{};
	Rhombus(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius)
	{
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second;
		points[2].x = center.first;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second;
	};

	void SetCords(std::pair <int, int> _center, int _radius) override
	{
		center.first = _center.first;
		center.second = _center.second;
		radius = _radius;
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second;
		points[2].x = center.first;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second;
	}


	void Move(std::pair <int, int> speed) override
	{
		center.first += speed.first;
		center.second += speed.second;
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second;
		points[2].x = center.first;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second;
	};


	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Polygon(hdc, points, 4);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};


class Triangle :public Figure
{
private:
	POINT points[3];
public:
	Triangle() : Figure(), points{ 0, 0, 0 }
	{};
	Triangle(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius)
	{
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + (2. * sqrt(3) / 3. * radius);
		points[1].y = center.second + radius;
		points[2].x = center.first - (2. * sqrt(3) / 3. * radius);
		points[2].y = center.second + radius;
	};

	void SetCords(std::pair <int, int> _center, int _radius) override
	{
		center.first = _center.first;
		center.second = _center.second;
		radius = _radius;
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + (2. * sqrt(3) / 3. * radius);
		points[1].y = center.second + radius;
		points[2].x = center.first - (2. * sqrt(3) / 3. * radius);
		points[2].y = center.second + radius;
	}


	void Move(std::pair <int, int> speed) override
	{
		center.first += speed.first;
		center.second += speed.second;
		points[0].x = center.first;
		points[0].y = center.second - radius;
		points[1].x = center.first + (2. * sqrt(3) / 3. * radius);
		points[1].y = center.second + radius;
		points[2].x = center.first - (2. * sqrt(3) / 3. * radius);
		points[2].y = center.second + radius;
	};


	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Polygon(hdc, points, 3);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};


class Square :public Figure
{
private:
	POINT points[4];
public:
	Square() : Figure(), points{ 0,0,0,0 }
	{};
	Square(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius)
	{
		points[0].x = center.first - radius;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second - radius;
		points[2].x = center.first + radius;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second + radius;
	};

	void SetCords(std::pair <int, int> _center, int _radius) override
	{
		center.first = _center.first;
		center.second = _center.second;
		radius = _radius;
		points[0].x = center.first - radius;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second - radius;
		points[2].x = center.first + radius;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second + radius;
	}


	void Move(std::pair <int, int> speed) override
	{
		center.first += speed.first;
		center.second += speed.second;
		points[0].x = center.first - radius;
		points[0].y = center.second - radius;
		points[1].x = center.first + radius;
		points[1].y = center.second - radius;
		points[2].x = center.first + radius;
		points[2].y = center.second + radius;
		points[3].x = center.first - radius;
		points[3].y = center.second + radius;
	};


	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Polygon(hdc, points, 4);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};


class Pentagram :public Figure
{
private:
	POINT points[5];
public:
	Pentagram() : Figure(), points{ 0,0,0,0,0 }
	{};

	Pentagram(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius)
	{
		for (int i = 1; i < 6; i++)
		{
			points[i - 1].x = _center.first + _radius * cos(M_PI + 2 * M_PI * i / 5);
			points[i - 1].y = _center.second + _radius * sin(M_PI + 2 * M_PI * i / 5);
		}
	};

	void SetCords(std::pair <int, int> _center, int _radius) override
	{
		center.first = _center.first;
		center.second = _center.second;
		radius = _radius;

		for (int i = 1; i < 6; i++)
		{
			points[i - 1].x = center.first + radius * cos(M_PI + 2 * M_PI * i / 5);
			points[i - 1].y = center.second + radius * sin(M_PI + 2 * M_PI * i / 5);
		}
	}


	void Move(std::pair <int, int> speed) override
	{
		center.first += speed.first;
		center.second += speed.second;
		for (int i = 1; i < 6; i++)
		{
			points[i - 1].x = center.first + radius * cos(M_PI + 2 * M_PI * i / 5);
			points[i - 1].y = center.second + radius * sin(M_PI + 2 * M_PI * i / 5);
		}
	};


	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Polygon(hdc, points, 5);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};

class Dot :public Figure
{

public:
	Dot() : Figure() {};
	Dot(std::pair <int, int> _center, int _radius = 0) : Figure(_center, _radius) {};
	void Paint(HWND hWnd, HDC hdc, int ID_PEN = 0, COLORREF colorin = RGB(rand() % 256, rand() % 256, rand() % 256), COLORREF colorout = RGB(rand() % 256, rand() % 256, rand() % 256)) override
	{
		HBRUSH Brush;
		HPEN Pen;
		Brush = CreateSolidBrush(colorin);
		if (!ID_PEN > 5) Pen = CreatePen(ID_PEN, 2, colorout);
		else Pen = CreatePen(NULL, 2, colorout);
		SelectObject(hdc, Brush);
		SelectObject(hdc, Pen);
		Ellipse(hdc, center.first - radius, center.second - radius, center.first + radius, center.second + radius);
		DeleteObject(Brush);
		DeleteObject(Pen);
	}
};