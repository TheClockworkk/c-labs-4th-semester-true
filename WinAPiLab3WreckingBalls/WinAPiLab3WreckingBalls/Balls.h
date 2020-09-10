#pragma once
#define _USE_MATH_DEFINES
#define BUTTON_W 80
#define BUTTON_H 30
#define EDIT_W 80
#define EDIT_H 20


#include <iostream>
#include <cmath>
#include "framework.h"
#include <vector>
class Ball
{
private:
    COLORREF color;
    double mass;
    int density;
    int radius;
    std::pair <int, int> center;
    std::pair <int, int> speed;

public:
    Ball() :mass{ 0 }, density{ 0 }, radius{ 0 }, center{ 0,0 }, speed(0, 0), color{ RGB(rand() % 256, rand() % 256, rand() % 256) } {}
    Ball(std::pair <int, int> _center = std::make_pair(0, 0), int _radius = 0, std::pair <int, int> _speed = std::make_pair(0, 0), int _density = rand() % 5000 + 1000)
        :center{ _center }, radius{ _radius }, speed{ _speed }
    {
        density = _density;
        mass = M_PI * radius * radius * density;
        color = RGB(rand() % 256, rand() % 256, rand() % 256);
    }


    void SetCords(std::pair <int, int> _center)
    {
        center.first = _center.first;
        center.second = _center.second;
    }


    void SetSpeed(std::pair <int, int> _speed)
    {
        speed = _speed;
    }


    void SetRadius(int _radius)
    {
        radius = _radius;
        mass = M_PI * radius * radius * density;
    }


    void SetDensity(int _density)
    {
        density = _density;
        mass = M_PI * radius * radius * density;
    }

    std::pair<int, int> GetCenter()
    {
        return center;
    }

    int GetRadius()
    {
        return radius;
    }

    int GetDensity()
    {
        return density;
    }

    std::pair<int, int> GetSpeed()
    {
        return speed;
    }

    void GetStrRadius(wchar_t* buf)
    {
        wsprintf(buf, L"%d", radius);
    }

    void GetStrDensity(wchar_t* buf)
    {
        wsprintf(buf, L"%d", density);
    }

    void GetStrSpeedX(wchar_t* buf)
    {
        wsprintf(buf, L"%d", speed.first);
    }


    void GetStrSpeedY(wchar_t* buf)
    {
        wsprintf(buf, L"%d", speed.second);
    }

    void Move()
    {
        center.first += speed.first;
        center.second += speed.second;
    }


    void CheckWalls(int width, int height)
    {
        if ((center.first + radius) >= width)
        {
            center.first = width - radius;
            if (speed.first > 0)
                speed.first = -speed.first;

        }
        else if ((center.first - radius) <= 0)
        {
            center.first = radius;
            if (speed.first < 0)
                speed.first = -speed.first;

        }
        if ((center.second + radius) >= height)
        {

            center.second = height - radius;
            if (speed.second > 0)
                speed.second = -speed.second;

        }
        else if ((center.second - radius) <= 0)
        {
            center.second = radius;
            if (speed.second < 0)
                speed.second = -speed.second;
        }
    };


    bool IsPush(Ball& other)
    {
        int sum = (radius + other.radius) * (radius + other.radius);
        if ((center.first - other.center.first) * (center.first - other.center.first) + (center.second - other.center.second) * (center.second - other.center.second) <= sum)
        {
            int dif = sum - (center.first - other.center.first) * (center.first - other.center.first) - (center.second - other.center.second) * (center.second - other.center.second);
            if (dif > 0)
            {
                dif = sqrt(dif);
                if (other.center.first > center.first)
                    SetCords(std::make_pair(center.first - dif, center.second));
                else
                    SetCords(std::make_pair(center.first + dif, center.second));

                if (other.center.second > center.second)
                    SetCords(std::make_pair(center.first, center.second - dif));
                else
                    SetCords(std::make_pair(center.first, center.second + dif));
            }
            int SpeedX1, SpeedX2, SpeedY1, SpeedY2;
            SpeedX1 = (2 * other.mass * other.speed.first + (mass - other.mass) * speed.first) / (mass + other.mass);
            SpeedY1 = (2 * other.mass * other.speed.second + (mass - other.mass) * speed.second) / (mass + other.mass);
            SpeedX2 = (2 * mass * speed.first + (other.mass - mass) * other.speed.first) / (mass + other.mass);
            SpeedY2 = (2 * mass * speed.second + (other.mass - mass) * other.speed.second) / (mass + other.mass);

            SetSpeed(std::make_pair(SpeedX1, SpeedY1));
            other.SetSpeed(std::make_pair(SpeedX2, SpeedY2));

            return true;
        }
        return false;
    }


    void Paint(HDC& hdc)
    {
        HBRUSH Brush;
        Brush = CreateSolidBrush(color);
        SelectObject(hdc, Brush);
        Ellipse(hdc, center.first - radius, center.second - radius, center.first + radius, center.second + radius);
        DeleteObject(Brush);
    }


    bool CheckMouse(int x, int y)
    {
        return (x - center.first) * (x - center.first) + (y - center.second) * (y - center.second) <= radius * radius;
    }
};

