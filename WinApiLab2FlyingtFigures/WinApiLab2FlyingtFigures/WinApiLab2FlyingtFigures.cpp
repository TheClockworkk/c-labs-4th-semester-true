#include "framework.h"
#include "Figure.h"
#include "Resource.h"

#define MAX_LOADSTRING 100



HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

typedef std::basic_stringstream<TCHAR> TStringStream;
std::pair<int, int> cords, speed = std::make_pair(5, 5);
int radius = rand() % 100 + 20;
int timerId;
int width, height;
int horSpeed, verSpeed;
int horSize, verSize;
RECT windowRect;
HBRUSH circleBrush;
COLORREF color;
Circle circ(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
Rhombus rhomb(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
Triangle trian(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
Square sqr(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
Pentagram pent(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
Dot dt(std::make_pair(2 * (radius+20), 2 * (radius + 20)), radius);
Figure* fig = NULL;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//        InitInstance(HINSTANCE, int)
//
//        Сохраняет маркер экземпляра и создает главное окно
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
//  WndProc(HWND, UINT, WPARAM, LPARAM)
//  Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int BrownTimer = SetTimer(hWnd, 2, rand() % 5000 + 1000, NULL);
    static int ChangeTimer = SetTimer(hWnd, 3, rand() % 10000 + 1000, NULL);
    static int r = rand() % 256, g = rand() % 256, b = rand() % 256, r1 = rand() % 256, g1 = rand() % 256, b1 = rand() % 256;
    static int PEN_TYPE = 0;
    static COLORREF colorin = RGB(r, g, b);
    static COLORREF colorout = RGB(r1, g1, b1);
    static bool brown = 0, stopmove = 0, stopchange = 0;
    static HDC hdc;
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT: {
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        }
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;

        case ID_CIRCLE:
            if (fig)
            {
                circ.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &circ;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &circ;
            break;
        case ID_RHOMBUS:
            if (fig)
            {
                rhomb.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &rhomb;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &rhomb;
            break;
        case ID_TRIANGLE:
            if (fig)
            {
                trian.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &trian;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &trian;
            break;
        case ID_SQUARE:
            if (fig)
            {
                sqr.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &sqr;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &sqr;
            break;
        case ID_PENTAGRAM:
            if (fig)
            {
                pent.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &pent;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &pent;
            break;
        case ID_DOT:
            if (fig)
            {
                dt.SetCords(std::make_pair(fig->GetX(), fig->GetY()), fig->GetR());
                fig = &dt;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else fig = &dt;
        case ID_BROWN:
            brown = !brown;
            break;
        case ID_STOPMOVE:
            stopmove = !stopmove;
            break;
        case ID_STOPCHANGE:
            stopchange = !stopchange;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SIZE:
    {
        GetClientRect(hWnd, &windowRect);
        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
        if (fig)
            fig->SetCords(std::make_pair(2 * (radius + 20), 2 * (radius + 20)), radius);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_CREATE:
    {
        timerId = SetTimer(hWnd, 1, 10, NULL);
        GetClientRect(hWnd, &windowRect);

        break;
    }
    case WM_TIMER:
    {
        if (brown)
            if (wParam == BrownTimer)
            {
                BrownTimer = SetTimer(hWnd, 2, rand() % 5000 + 1000, NULL);
                speed = std::make_pair(rand() % 20, rand() % 20);
            }
        if (!stopchange)
        {
            if (wParam == ChangeTimer)
            {
                ChangeTimer = SetTimer(hWnd, 3, rand() % 10000 + 1000, NULL);
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
                r1 = rand() % 256;
                g1 = rand() % 256;
                b1 = rand() % 256;
                colorin = RGB(r, g, b);
                colorout = RGB(r1, g1, b1);
                PEN_TYPE = rand() % 6;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        if (!stopmove)
        {
            if (wParam == timerId)
            {
                if ((fig->GetX() + fig->GetR()) >= width)
                {
                    speed.first = -speed.first;
                }
                else if ((fig->GetX() - fig->GetR()) <= 0)
                {
                    speed.first = -speed.first;
                }
                if ((fig->GetY() + fig->GetR()) >= height - 30) //удары граница
                {
                    speed.second = -speed.second;
                }
                else if ((fig->GetY() - fig->GetR()) <= 0)
                {
                    speed.second = -speed.second;
                }
            }
            if (!stopmove || !stopchange) InvalidateRect(hWnd, NULL, true);
        }

        break;
    }
    case WM_GETMINMAXINFO:
    {
        LPMINMAXINFO lpMNI = (LPMINMAXINFO)lParam;
        lpMNI->ptMinTrackSize.y = 300;
        lpMNI->ptMinTrackSize.x = 250;
    }
    case WM_PAINT:
    {
        if (fig)
        {

            PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            RECT rect;
            GetWindowRect(hWnd, &rect);
            LONG height = rect.bottom - rect.top;
            fig->PrintInfo(hdc, height, colorin, colorout);
            if (!stopmove) fig->Move(speed);
            fig->Paint(hWnd, hdc, PEN_TYPE, colorin, colorout);
            MoveToEx(hdc, 0, height - 80, NULL);
            LineTo(hdc, width, height - 80);
            EndPaint(hWnd, &ps);
        }
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
