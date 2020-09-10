// WinAPiLab3WreckingBalls.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Balls.h"
#include "Resource.h"
#include <commdlg.h>
#include <iostream>
#include <fstream>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
WCHAR szFileName[MAX_PATH] = L"";
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
OPENFILENAME ofn;
std::ofstream LogOut("log.txt");
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FLYINGBALLS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FLYINGBALLS));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FLYINGBALLS));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FLYINGBALLS);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//


int GetWindowInt(HWND hWnd, size_t size, bool* success, bool is_neg = false)
{
    wchar_t buf[BUFSIZ];
    GetWindowText(hWnd, buf, size);
    if (!wcscmp(buf, L""))
    {
        *success = false;
        return 0;
    }
    int res = 0;
    if (is_neg && buf[0] == L'-')
    {
        is_neg = true;
    }
    else is_neg = false;
    for (size_t i = is_neg ? 1 : 0; i < size && buf[i] != 0; i++)
    {
        wchar_t c = buf[i];
        if (iswdigit(c))
        {
            res = res * 10 + _wtoi(&c);
        }
        else
        {
            *success = false;
            return 0;
        }
    }
    if (is_neg) res *= -1;
    *success = true;
    return res;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hStop, hStart, hAddBall, hDensity, hRadius, hSpeedX, hSpeedY;
    static int UpdateTimer = SetTimer(hWnd, 1, 10, NULL);
    static std::vector<Ball> balls;
    static RECT windowRect;
    static int width, height;
    static bool stopmove = 1, moving = 0, adding = 0, check = 0;
    static int x, y, cur;
    static wchar_t buf[22];
    int radius, density, speedX, speedY;
    switch (message)
    {

    case WM_RBUTTONDOWN:
    {
        if (moving)
        {
            moving = false;
        }
    }
    case WM_LBUTTONDOWN:
    {
        if (adding)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            radius = GetWindowInt(hRadius, 3, &check, false);
            if (check)
                density = GetWindowInt(hDensity, 5, &check, false);
            if (check)
                speedX = GetWindowInt(hSpeedX, 3, &check, true);
            if (check)
                speedY = GetWindowInt(hSpeedY, 3, &check, true);
            if (check)
            {
                balls.push_back(Ball(std::make_pair(x, y), radius, std::make_pair(speedX, speedY), density));
                ShowWindow(hDensity, SW_HIDE);
                ShowWindow(hRadius, SW_HIDE);
                ShowWindow(hSpeedX, SW_HIDE);
                ShowWindow(hSpeedY, SW_HIDE);
                adding = false;
                InvalidateRect(hWnd, NULL, TRUE);
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                for (size_t i = 0; i < balls.size(); i++)
                {
                    balls[i].Paint(hdc);
                }
                EndPaint(hWnd, &ps);
            }
            else
            {
                SetWindowText(hDensity, L"INCORRECT");
                SetWindowText(hRadius, L"INCORRECT");
                SetWindowText(hSpeedX, L"INCORRECT");
                SetWindowText(hSpeedY, L"INCORRECT");
            }
        }
        else if (!moving)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            for (int i = 0; i < balls.size(); i++)
            {
                if (balls[i].CheckMouse(x, y))
                {
                    cur = i;
                    stopmove = true;
                    moving = true;
                    ShowWindow(hDensity, SW_SHOW);
                    ShowWindow(hRadius, SW_SHOW);
                    ShowWindow(hSpeedX, SW_SHOW);
                    ShowWindow(hSpeedY, SW_SHOW);
                    balls[i].GetStrDensity(buf);
                    SetWindowText(hDensity, buf);
                    balls[i].GetStrRadius(buf);
                    SetWindowText(hRadius, buf);
                    balls[i].GetStrSpeedX(buf);
                    SetWindowText(hSpeedX, buf);
                    balls[i].GetStrSpeedY(buf);
                    SetWindowText(hSpeedY, buf);
                    break;
                }
            }
        }
        else
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            radius = GetWindowInt(hRadius, 3, &check, false);
            if (check)
                density = GetWindowInt(hDensity, 5, &check, false);
            if (check)
                speedX = GetWindowInt(hSpeedX, 3, &check, true);
            if (check)
                speedY = GetWindowInt(hSpeedY, 3, &check, true);
            if (check)
            {
                balls[cur].SetCords(std::make_pair(x, y));
                balls[cur].SetRadius(radius);
                balls[cur].SetDensity(density);
                balls[cur].SetSpeed(std::make_pair(speedX, speedY));
                InvalidateRect(hWnd, NULL, true);
            }
            else
            {
                balls[cur].GetStrDensity(buf);
                SetWindowText(hDensity, buf);
                balls[cur].GetStrRadius(buf);
                SetWindowText(hRadius, buf);
                balls[cur].GetStrSpeedX(buf);
                SetWindowText(hSpeedX, buf);
                balls[cur].GetStrSpeedY(buf);
                SetWindowText(hSpeedY, buf);
            }
        }
        break;
    }
    case WM_CREATE:
    {
        ShowWindow(hWnd, SW_SHOWMAXIMIZED);
        GetClientRect(hWnd, &windowRect);
        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
        hStop = CreateWindow(L"button", L"STOP", WS_CHILD | WS_VISIBLE | WS_BORDER,
            BUTTON_W / 2, height - 2 * BUTTON_H, BUTTON_W, BUTTON_H,
            hWnd, 0, hInst, NULL);
        hStart = CreateWindow(L"button", L"START", WS_CHILD | WS_VISIBLE | WS_BORDER,
            BUTTON_W / 2, height - 3 * BUTTON_H, BUTTON_W, BUTTON_H,
            hWnd, 0, hInst, NULL);
        hAddBall = CreateWindow(L"button", L"ADD BALL", WS_CHILD | WS_VISIBLE | WS_BORDER,
            BUTTON_W / 2, height - 4 * BUTTON_H, BUTTON_W, BUTTON_H,
            hWnd, 0, hInst, NULL);
        hDensity = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, BUTTON_W / 2, height - 4 * BUTTON_H - EDIT_H, EDIT_W, EDIT_H,
            hWnd, 0, hInst, NULL);
        SetWindowText(hDensity, L"density");
        hRadius = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, BUTTON_W / 2, height - 4 * BUTTON_H - 2 * EDIT_H, EDIT_W, EDIT_H,
            hWnd, 0, hInst, NULL);
        SetWindowText(hRadius, L"radius");
        hSpeedY = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, BUTTON_W / 2, height - 4 * BUTTON_H - 3 * EDIT_H, EDIT_W, EDIT_H,
            hWnd, 0, hInst, NULL);
        SetWindowText(hSpeedY, L"speed_y");
        hSpeedX = CreateWindow(L"edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, BUTTON_W / 2, height - 4 * BUTTON_H - 4 * EDIT_H, EDIT_W, EDIT_H,
            hWnd, 0, hInst, NULL);
        SetWindowText(hSpeedX, L"speed_x");

        SendMessage(hDensity, EM_SETLIMITTEXT, 4, 0);
        SendMessage(hRadius, EM_SETLIMITTEXT, 2, 0);
        SendMessage(hSpeedX, EM_SETLIMITTEXT, 3, 0);
        SendMessage(hSpeedY, EM_SETLIMITTEXT, 3, 0);
        ShowWindow(hRadius, SW_HIDE);
        ShowWindow(hDensity, SW_HIDE);
        ShowWindow(hSpeedX, SW_HIDE);
        ShowWindow(hSpeedY, SW_HIDE);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        for (size_t i = 0; i < balls.size(); i++)
        {
            balls[i].Paint(hdc);
        }
        EndPaint(hWnd, &ps);
    }

    case WM_COMMAND:
    {
        if (stopmove && lParam == (LPARAM)hStart)
        {
            stopmove = false;
            moving = false;
        }

        if (!stopmove && lParam == (LPARAM)hStop)
        {
            stopmove = true;
        }

        if (!adding && lParam == (LPARAM)hAddBall)
        {
            adding = true;
            ShowWindow(hDensity, SW_SHOW);
            ShowWindow(hRadius, SW_SHOW);
            ShowWindow(hSpeedX, SW_SHOW);
            ShowWindow(hSpeedY, SW_SHOW);
        }
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case ID_SAVEIN:
        {
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = (LPCWSTR)L"txt";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;

            GetSaveFileNameW(&ofn);

            std::ofstream config_fs(ofn.lpstrFile, std::ios_base::out | std::ios_base::trunc);

            for (size_t i = 0; i < balls.size(); i++) {
                config_fs << balls[i].GetCenter().first << ' '
                    << balls[i].GetCenter().second << ' '
                    << balls[i].GetRadius() << ' '
                    << balls[i].GetSpeed().first << ' '
                    << balls[i].GetSpeed().second << ' '
                    << balls[i].GetDensity() << '\n';
            }
            break;
        }
        case ID_LOADOUT:
        {
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;
            ofn.lpstrFilter = (LPCWSTR)L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            ofn.lpstrDefExt = (LPCWSTR)L"txt";
            ofn.lpstrFile = szFileName;
            ofn.nMaxFile = MAX_PATH;

            GetOpenFileNameW(&ofn);

            std::ifstream config_fs(ofn.lpstrFile, std::ios_base::in);

            int id;
            double x, y, vx, vy, mass, radius;

            balls.clear();

            // parse file by lines
            while (!config_fs.eof())
            {
                int _x, _y, _speedX, _speedY, _radius, _density;
                config_fs >> _x;
                config_fs >> _y;
                config_fs >> _radius;
                config_fs >> _speedX;
                config_fs >> _speedY;
                config_fs >> _density;
                balls.push_back(Ball(std::make_pair(_x, _y), _radius, std::make_pair(_speedX, _speedY), _density));
            }
            InvalidateRect(hWnd, NULL, true);
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_TIMER:
    {
        if (!stopmove)
        {
            for (size_t i = 0; i < balls.size(); i++)
            {
                balls[i].CheckWalls(width, height);
            }
            for (size_t i = 0; i < balls.size(); i++)
            {
                for (size_t j = i + 1; j < balls.size(); j++)
                {
                    std::pair<int, int> speed1 = balls[i].GetSpeed(), speed2 = balls[j].GetSpeed();
                    if (balls[i].IsPush(balls[j]))
                    {
                        LogOut << i << ":center(" << balls[i].GetCenter().first << ',' << balls[i].GetCenter().second << ") speed before:("
                            << speed1.first << ',' << speed1.second << ") speed after:(" << balls[i].GetSpeed().first << ',' << balls[i].GetSpeed().second << ") vs "
                            << j << ":center(" << balls[j].GetCenter().first << ',' << balls[j].GetCenter().second << ") speed before:("
                            << speed2.first << ',' << speed2.second << ") speed after:(" << balls[j].GetSpeed().first << ',' << balls[j].GetSpeed().second << ")\n";

                    }
                }
            }
            InvalidateRect(hWnd, NULL, true);
        }
        break;
    }
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (moving)
        {
            for (size_t i = 0; i < balls.size(); i++)
            {
                balls[i].Paint(hdc);
            }
        }
        if (!stopmove)
        {
            for (size_t i = 0; i < balls.size(); i++)
            {
                balls[i].Move();
                balls[i].Paint(hdc);
            }
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        LogOut.close();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}