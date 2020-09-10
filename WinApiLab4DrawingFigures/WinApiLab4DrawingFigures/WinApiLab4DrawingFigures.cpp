#include "framework.h"
#include "WinApiLab4DrawingFigures.h"
#include <iostream>
#include <string>
#include "Figure.h"
#include <cctype>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
static int mouse_mod = MOUSE_MOD_NONE;
static std::vector<POINT> points;
static std::vector<Figure> figures;
static Figure* editing = nullptr;
static POINT point, old_point;
static int x, y, def_angle = -1;
static int id_counter = 0;
HDC hdc;
RECT windowRect;
int width, height;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Edit(HWND, UINT, WPARAM, LPARAM);
HWND hEditBox, hEdit, hDial, hWnd;

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
    LoadStringW(hInstance, IDC_FIGUREGENERATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIGUREGENERATOR));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIGUREGENERATOR));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_FIGUREGENERATOR);
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &windowRect);
        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
        break;
    case WM_MOUSEMOVE:
    {
        if (mouse_mod == MOUSE_MOD_DRAGGING && editing)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            point.x = x - old_point.x;
            point.y = y - old_point.y;
            if (editing->Move(point, width, height))
            {
                InvalidateRect(hWnd, NULL, true);
            }
            old_point.x = x;
            old_point.y = y;
        }
        if (mouse_mod == MOUSE_MOD_DEFORMING && editing)
        {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            point.x = x - old_point.x;
            point.y = y - old_point.y;
            if (editing->Deform(def_angle, point, width, height))
            {
                InvalidateRect(hWnd, NULL, true);
            }
            old_point.x = x;
            old_point.y = y;
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        point.x = x;
        point.y = y;
        switch (mouse_mod)
        {
        case MOUSE_MOD_DRAWING:
        {
            points.push_back(point);
            break;
        }
        case MOUSE_MOD_DRAGGING:
        {
            mouse_mod = MOUSE_MOD_EDITING;
            editing = nullptr;
            break;
        }
        case MOUSE_MOD_DEFORMING:
        {
            mouse_mod = MOUSE_MOD_EDITING;
            editing->UpdateCenter();
            editing = nullptr;
            def_angle = -1;
            break;
        }
        default: break;
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        point.x = x;
        point.y = y;
        switch (mouse_mod)
        {
        case MOUSE_MOD_EDITING:
        {
            for (size_t i = 0; i < figures.size(); i++)
            {
                def_angle = figures[i].CheckAngle(point);
                if (def_angle != -1)
                {
                    editing = &figures[i];
                    figures[i].SetColor();
                    editing = &figures[i];
                    old_point = point;
                    InvalidateRect(hWnd, NULL, true);
                    mouse_mod = MOUSE_MOD_DEFORMING;
                    break;
                }
                if (isInside(figures[i].points, point))
                {
                    figures[i].SetColor();
                    editing = &figures[i];
                    old_point = point;
                    InvalidateRect(hWnd, NULL, true);
                    mouse_mod = MOUSE_MOD_DRAGGING;
                    break;
                }
            }
            break;
        }
        default: break;
        }
        break;
    }
    case WM_RBUTTONDOWN:
    {
        if (mouse_mod == MOUSE_MOD_DRAWING)
        {
            if (points.size() > 2)
            {
                Figure fig(points);
                figures.push_back(fig);
            }
            points.clear();
            InvalidateRect(hWnd, NULL, true);
        }
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_RAND_FIG:
        {
            int count = rand() % 5 + 1;
            for (size_t i = 0; i < count; i++)
            {
                std::vector <POINT> points;
                POINT new1, new2, new3, new4;
                new1.x = rand() % width;
                new1.y = rand() % height;
                new3.x = abs(rand() % width - new1.x);
                new3.y = abs(rand() % height - new1.y);
                new2.x = new1.x;
                new2.y = new3.y;
                new4.x = new3.x;
                new4.y = new1.y;
                points.push_back(new1);
                points.push_back(new2);
                points.push_back(new3);
                points.push_back(new4);
                Figure fig(points);
                figures.push_back(fig);
                points.clear();

            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDial, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_EDIT:
        {
            mouse_mod = MOUSE_MOD_EDITING;
            break;
        }
        case IDM_DRAWING:
        {
            mouse_mod = MOUSE_MOD_DRAWING;
            break;
        }
        case IDM_LIST_BOX:
        {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_EDIT_BOX), hWnd, Edit);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);
        for (size_t i = 0; i < figures.size(); i++)
        {
            figures[i].Paint(hdc);
        }
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
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


INT_PTR CALLBACK Edit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    static int cur;
    static BOOL succes = false;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        hEditBox = GetDlgItem(hDlg, IDC_FIGURE_LIST);
        cur = -1;
        for (size_t i = 0; i < figures.size(); i++)
        {
            std::wstring str = L"Фигура номер ";
            str += std::to_wstring(i);
            SendMessage(hEditBox, LB_ADDSTRING, 0, (LPARAM)str.c_str());
        }
        break;
    }
    case WM_COMMAND:

        if (LOWORD(wParam) == IDCANCEL)
        {
            for (size_t i = 0; i < figures.size(); i++)
                figures[i].Show();
            InvalidateRect(hWnd, NULL, true);
            EndDialog(hDlg, LOWORD(wParam));
            cur = -1;
            return (INT_PTR)TRUE;
        }
        switch (LOWORD(wParam))
        {
        case IDC_FIGURE_LIST:
        {
            if (HIWORD(wParam) == LBN_DBLCLK)
            {
                wchar_t buf[BUFSIZ], * buf_ptr = NULL;
                std::wstring inf;
                int i = SendMessage(hEditBox, LB_GETCURSEL, 0, 0);
                SendMessage(hEditBox, LB_GETTEXT, i, (LPARAM)buf);
                i = 0;
                while (buf)
                {
                    if (iswdigit(buf[i]))
                    {
                        buf_ptr = &buf[i];
                        break;
                    }
                    i++;
                }
                cur = _wtoi(buf_ptr);
                SetDlgItemText(hDlg, IDC_EDIT_R, std::to_wstring(GetRValue(figures[cur].color)).c_str());
                SetDlgItemText(hDlg, IDC_EDIT_G, std::to_wstring(GetGValue(figures[cur].color)).c_str());
                SetDlgItemText(hDlg, IDC_EDIT_B, std::to_wstring(GetBValue(figures[cur].color)).c_str());
                SetDlgItemText(hDlg, IDC_EDIT_SHAPES, std::to_wstring(figures[cur].points.size()).c_str());
                SetDlgItemText(hDlg, IDC_EDIT_X, std::to_wstring(figures[cur].center.x).c_str());
                SetDlgItemText(hDlg, IDC_EDIT_Y, std::to_wstring(figures[cur].center.y).c_str());
                for (int i = 0; i < cur; i++)
                    figures[i].Hide();
                for (int i = cur + 1; i < figures.size(); i++)
                    figures[i].Hide();
                figures[cur].Show();
                InvalidateRect(hWnd, NULL, true);
            }
            break;
        }
        case IDOK:
        {
            if (cur != -1)
            {
                POINT point;
                int r = GetDlgItemInt(hDlg, IDC_EDIT_R, &succes, false);
                int g = GetDlgItemInt(hDlg, IDC_EDIT_G, &succes, false);
                int b = GetDlgItemInt(hDlg, IDC_EDIT_B, &succes, false);
                point.x = GetDlgItemInt(hDlg, IDC_EDIT_X, &succes, false) - figures[cur].center.x;
                point.y = GetDlgItemInt(hDlg, IDC_EDIT_Y, &succes, false) - figures[cur].center.y;
                int angle = GetDlgItemInt(hDlg, IDC_EDIT_SHAPES, &succes, false);
                figures[cur].SetColor(RGB(r, g, b));
                figures[cur].Move(point, width, height);
                if (angle > 2) figures[cur].ChangeCountAngels(angle, width, height);
                InvalidateRect(hWnd, NULL, true);
                return (INT_PTR)TRUE;
            }
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}