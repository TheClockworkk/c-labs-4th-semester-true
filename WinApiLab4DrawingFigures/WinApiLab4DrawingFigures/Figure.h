#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "framework.h"
#include <algorithm>
#define INF 100000
#define EPS 10
#define PI 3.14


class Figure
{
private:
public:
    std::vector <POINT> points;
    POINT center;
    COLORREF color;
    bool hide;
    Figure() : color{ RGB(rand() % 256, rand() % 256, rand() % 256) }, hide{ false }, center{ 0,0 } {};
    Figure(std::vector<POINT> _points, bool _hide = false, COLORREF _color = RGB(rand() % 256, rand() % 256, rand() % 256)) :
        points{ _points }, hide{ _hide }, color{ _color }, center{ 0,0 }
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            center.x += points[i].x;
            center.y += points[i].y;
        }
        center.x /= points.size();
        center.y /= points.size();
    };


    void ChangeCountAngels(int count, int width, int height)
    {
        std::vector<POINT> newpoint;
        POINT point;
        int radius = abs(center.x - std::max_element(points.begin(), points.end(), [](POINT first, POINT second) {return first.x < second.x; })->x);
        points.clear();
        for (size_t i = 0; i < count; i++)
        {
            point.x = (int)(center.x + radius * cos((2 * PI * i) / count)) % width;
            point.y = (int)(center.y + radius * sin((2 * PI * i) / count)) % height;

            if (point.x < 0)
                point.x = 0;
            if (point.y < 0)
                point.y = 0;
            points.push_back(point);
        }
    }

    void UpdateCenter()
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            center.x += points[i].x;
            center.y += points[i].y;
        }
        center.x /= points.size();
        center.y /= points.size();
    }

    int CheckAngle(POINT point)
    {
        for (int i = 0; i < points.size(); i++)
        {
            if (((point.x - points[i].x) * (point.x - points[i].x) + (point.y - points[i].y) * (point.y - points[i].y)) <= (EPS * EPS)) return i;
        }
        return -1;
    }

    bool Deform(int def_angle, POINT point, int width, int height)
    {
        if ((points[def_angle].x + point.x) >= 0 && (points[def_angle].x + point.x) <= width &&
            (points[def_angle].y + point.y) >= 0 && (points[def_angle].y + point.y) <= height)
        {
            points[def_angle].x += point.x;
            points[def_angle].y += point.y;
            return true;
        }
        return false;
    }


    bool Move(POINT point, int width, int height)
    {
        int min_x = std::max_element(points.begin(), points.end(), [](POINT first, POINT second) {return first.x > second.x; })->x;
        int max_x = std::max_element(points.begin(), points.end(), [](POINT first, POINT second) {return first.x < second.x; })->x;
        int min_y = std::max_element(points.begin(), points.end(), [](POINT first, POINT second) {return first.y > second.y; })->y;
        int max_y = std::max_element(points.begin(), points.end(), [](POINT first, POINT second) {return first.y < second.y; })->y;
        if ((min_x + point.x) >= 0 && (max_x + point.x) <= width &&
            (min_y + point.y) >= 0 && (max_y + point.y) <= height)
        {
            for (size_t i = 0; i < points.size(); i++)
            {
                points[i].x += point.x;
                points[i].y += point.y;
            }
            center.x += point.x;
            center.y += point.y;
            return true;
        }
        return false;
    }

    void Paint(HDC& hdc)
    {
        if (!hide)
        {
            HBRUSH brush = CreateSolidBrush(color);
            SelectObject(hdc, brush);
            Polygon(hdc, &points[0], points.size());
            DeleteObject(brush);
        }
    }

    void Hide()
    {
        hide = true;
    }


    void Show()
    {
        hide = false;
    }


    void SetColor(COLORREF _color = RGB(rand() % 256, rand() % 256, rand() % 256))
    {
        color = _color;
    }

};