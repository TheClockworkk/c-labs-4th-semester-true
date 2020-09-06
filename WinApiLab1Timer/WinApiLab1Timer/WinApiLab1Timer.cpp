// lab3_1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WinApiLab1Timer.h"
#include <string>
#define MAX_LOADSTRING 100


bool CheckTime(SYSTEMTIME left, SYSTEMTIME right) //очевидно, возвращает год месяц день час минуту секунду и миллисеку
{
    return left.wYear == right.wYear && left.wMonth == right.wMonth &&
        left.wDay == right.wDay && left.wHour == right.wHour &&
        left.wMinute == right.wMinute && left.wSecond == right.wSecond &&
        left.wMilliseconds == right.wMilliseconds;
}



// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр (дискриптор приложения)
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, //мэйн. Первое - дескриптор приложения, второе и третье не нужно и не юзается, а четвёртое - отображение
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance); //это вот как рахз брак 2 и 3 параметров мейна
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); //LoadString загружает ресурс строки из исполняемого файла связанного с заданным модулем, копирует строку в буфер и добавляет в конец символ завершающего нуля
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    //прилетает туда короче 1)дескриптор экземпляра модуля, исполняемый файл которого содержит в себе ресурс строки 
    //2)целочисленный идентификатор строки, которая будет загружена 
    //3)Указатель на буфер, который примет строку 
    //4) размер буфера (Строка обрезается и завершается NULL, если она длиннее, чем указанное число символов.)
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1)); //загружает заданную таблицу клавиш-ускорителей. 
    //Туда прилетает Дескриптор модуля, исполняемый файл которого содержит загружаемую таблицу клавиш-ускорителей и 
    //Указатель на символьную строку с нулем в конце, которая содержит имя загружаемой таблицы клавиш - ускорителей / идентификатор ресурса таблицы клавиш-ускорителей в младшем слове и заполненное нулями старшее слово

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
    WNDCLASSEXW wcex; //содержит информацию о классе окна

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW; //Устанавливает стиль(и) класса. Этот член структуры может быть любой комбинацией стилей класса(коих 12).
    //в нашем случае 1) Перерисовывает все окно, если перемещение или регулировка размера изменяют ширину рабочей области 2) Перерисовывает все окно, если перемещение или регулировка размера изменяют высоту рабочей области.
    wcex.lpfnWndProc = WndProc;//Указатель на оконную процедуру.Функция CallWindowProc - вызов оконной процедуры.
    wcex.cbClsExtra = 0;//Устанавливает число дополнительных байт, которые размещаются вслед за структурой класса окна. Система инициализирует эти байты нулями.
    wcex.cbWndExtra = 0;//Устанавливает число дополнительных байтов, которые размещаются вслед за экземпляром окна. Система инициализирует байты нулями.
    wcex.hInstance = hInstance;//Дескриптор экземпляра, который содержит оконную процедуру для класса.
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));//Дескриптор значка класса. Этот член структуры должен быть дескриптором ресурса значка.
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);//Дескриптор курсора класса. Этот член структуры должен быть дескриптором ресурса курсора
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//Дескриптор кисти фона класса. Этот член структуры может быть дескриптором физической кисти, которая используется, чтобы красить цветом фона, или это может быть кодом цвета(как в нашем случае). 
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB1);//Указатель на символьную строку с символом конца строки, устанавлевающей имя ресурса меню класса, которое как имя показывается в файле ресурса. И у нас тут макрос для целых чисел во
    wcex.lpszClassName = szWindowClass;//Указатель на символьную строку с нулем в конце или  атом. Если этот параметр - атом, это должен быть атом класса, созданный предыдущим вызовом функции RegisterClass или RegisterClassEx. Атом должен находиться в младшем слове lpszClassName; старшее слово должно быть нуль.
   //Если lpszClassName - символьная строка, она задает имя класса окна. Имя класса может быть любым именем, зарегистрированным функцией RegisterClass или RegisterClassEx, или какое - либо из предопределенных имен класса органов управления.
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));//Дескриптор маленького значка, который связан с классом окна.

    return RegisterClassExW(&wcex);
}

//
//  InitInstance(HINSTANCE, int)
//
//   Сохраняет маркер экземпляра и создает главное окно
//
//    В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//    создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE,
        200, 70, 295, 140, nullptr, nullptr, hInstance, nullptr); //параметры окна:
    //по параметрам 1)указатель на имя класса 2)на имя окна 3)стиль окна
    //4) горизонтальная позиция окна 5)вертикальная 6) ширина 7) высота
    //8)дескриптор родительского/окна владельца 9) дескриптом меню или дочернего
    //10) дескриптор экземпляра приложения 11)указатель на данные создания окна

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

SYSTEMTIME upd_time(SYSTEMTIME s, unsigned long long mseconds)
{
    FILETIME f;
    SystemTimeToFileTime(&s, &f);//системное время в файловое

    ULARGE_INTEGER u;
    u.LowPart = f.dwLowDateTime;
    u.HighPart = f.dwHighDateTime;

    u.QuadPart += mseconds * 10000ull;

    f.dwLowDateTime = u.LowPart;
    f.dwHighDateTime = u.HighPart;

    FileTimeToSystemTime(&f, &s);

    return s;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static SYSTEMTIME st, puf;
    static HWND hDay, hMonth, hYear, hHour, hMinute, hSecond, hMsecond, hTimer, hApply, hStop, hError;
    static int RefreshTimer;
    static wchar_t buf[20];
    static int ref = 0;
    switch (message)
    {
    case WM_TIMER:
    {
        if (wParam == RefreshTimer)
        {
            puf = st;
            st = upd_time(st, ref);
            if (CheckTime(puf, st))
            {
                SetWindowText(hDay, L"00");
                SetWindowText(hMonth, L"00");
                SetWindowText(hYear, L"0000");
                SetWindowText(hHour, L"00");
                SetWindowText(hMinute, L"00");
                SetWindowText(hSecond, L"00");
                SetWindowText(hMsecond, L"0000");
                KillTimer(hWnd, 1);
                break;
            }
            else
            {
                wsprintf(buf, L"%d", st.wDay);
                SetWindowText(hDay, buf);
                wsprintf(buf, L"%d", st.wMonth);
                SetWindowText(hMonth, buf);
                wsprintf(buf, L"%d", st.wYear);
                SetWindowText(hYear, buf);
                wsprintf(buf, L"%d", st.wHour);
                SetWindowText(hHour, buf);
                wsprintf(buf, L"%d", st.wMinute);
                SetWindowText(hMinute, buf);
                wsprintf(buf, L"%d", st.wSecond);
                SetWindowText(hSecond, buf);
                wsprintf(buf, L"%d", st.wMilliseconds);
                SetWindowText(hMsecond, buf);
            }
        }
        break;
    }
    case WM_CREATE:
    {
        hDay = CreateWindow(L"edit", L"00",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 5, 20, 25, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hDay, EM_SETLIMITTEXT, 2, 0);
        ShowWindow(hDay, SW_SHOWNORMAL);
        hMonth = CreateWindow(L"edit", L"00",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 35, 20, 25, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hMonth, EM_SETLIMITTEXT, 2, 0);
        ShowWindow(hMonth, SW_SHOWNORMAL);
        hYear = CreateWindow(L"edit", L"0000",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 65, 20, 35, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hYear, EM_SETLIMITTEXT, 5, 0);
        ShowWindow(hYear, SW_SHOWNORMAL);
        hHour = CreateWindow(L"edit", L"00",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 5, 57, 25, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hHour, EM_SETLIMITTEXT, 2, 0);
        ShowWindow(hHour, SW_SHOWNORMAL);
        hMinute = CreateWindow(L"edit", L"00",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 35, 57, 25, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hMinute, EM_SETLIMITTEXT, 2, 0);
        ShowWindow(hMinute, SW_SHOWNORMAL);
        hSecond = CreateWindow(L"edit", L"00",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 65, 57, 25, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hSecond, EM_SETLIMITTEXT, 2, 0);
        ShowWindow(hSecond, SW_SHOWNORMAL);
        hMsecond = CreateWindow(L"edit", L"0000",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 95, 57, 35, 20,
            hWnd, 0, hInst, NULL);
        SendMessage(hMsecond, EM_SETLIMITTEXT, 4, 0);
        ShowWindow(hMsecond, SW_SHOWNORMAL);
        hTimer = CreateWindow(L"edit", L"0",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_NUMBER, 105, 20, 170, 20,
            hWnd, 0, hInst, NULL);
        ShowWindow(hTimer, SW_SHOWNORMAL);
        hApply = CreateWindow(L"button", L"Старт",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            135, 57, 70, 20, hWnd, 0, hInst, NULL);
        ShowWindow(hApply, SW_SHOWNORMAL);
        hStop = CreateWindow(L"button", L"Стоп",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            205, 57, 70, 20, hWnd, 0, hInst, NULL);
        ShowWindow(hApply, SW_SHOWNORMAL);
    }

    case WM_COMMAND:
    {
        if (lParam == (LPARAM)hApply)    // если нажали на кнопку
        {
            int cur;
            GetWindowText(hTimer, buf, 12);
            ref = _wtoi(buf);
            RefreshTimer = SetTimer(hWnd, 1, ref, NULL);
            GetWindowText(hDay, buf, 3);
            cur = _wtoi(buf);
            st.wDay = cur;
            GetWindowText(hMonth, buf, 3);
            cur = _wtoi(buf);
            st.wMonth = cur;
            GetWindowText(hYear, buf, 5);
            cur = _wtoi(buf);
            st.wYear = cur;
            GetWindowText(hHour, buf, 3);
            cur = _wtoi(buf);
            st.wHour = cur;
            GetWindowText(hMinute, buf, 3);
            cur = _wtoi(buf);
            st.wMinute = cur;
            GetWindowText(hSecond, buf, 3);
            cur = _wtoi(buf);//приведение типой
            st.wSecond = cur;
            GetWindowText(hMsecond, buf, 5);
            cur = _wtoi(buf);
            st.wMilliseconds = cur;
        }

        if (lParam == (LPARAM)hStop)    // если нажали на кнопку
        {
            KillTimer(hWnd, 1);
        }

        int wmId = LOWORD(wParam);

        // Разобрать выбор в меню:
        switch (wmId)
        {

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_32774:
        {
            GetLocalTime(&st);
            wsprintf(buf, L"%d", st.wDay);
            SetWindowText(hDay, buf);
            wsprintf(buf, L"%d", st.wMonth);
            SetWindowText(hMonth, buf);
            wsprintf(buf, L"%d", st.wYear);
            SetWindowText(hYear, buf);
            wsprintf(buf, L"%d", st.wHour);
            SetWindowText(hHour, buf);
            wsprintf(buf, L"%d", st.wMinute);
            SetWindowText(hMinute, buf);
            wsprintf(buf, L"%d", st.wSecond);
            SetWindowText(hSecond, buf);
            wsprintf(buf, L"%d", st.wMilliseconds);
            SetWindowText(hMsecond, buf);
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
        HDC hdc = BeginPaint(hWnd, &ps);//готовит окошко к окрашиванию и заполняет ps инфой о нём

        TextOut(hdc, 31, 21, L".", 1);
        TextOut(hdc, 61, 21, L".", 1);
        TextOut(hdc, 31, 58, L":", 1);
        TextOut(hdc, 61, 58, L":", 1);
        TextOut(hdc, 91, 58, L":", 1);
        TextOut(hdc, 100, 21, L"--", 2);
        TextOut(hdc, 14, 5, L"dd", 2);
        TextOut(hdc, 36, 5, L"mm", 2);
        TextOut(hdc, 67, 5, L"yyyy", 4);
        TextOut(hdc, 14, 42, L"hh", 2);
        TextOut(hdc, 36, 42, L"min", 3);
        TextOut(hdc, 66, 42, L"sec", 3);
        TextOut(hdc, 95, 42, L"msec", 4);
        TextOut(hdc, 122, 5, L"И Н Т Е Р В А Л И Щ Е", 21);
        
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);//указывает системе шо поток сделал запрос на завершение раьоты
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);//для обработки любой невероятной хуйни-сообщения от окна, которую приложение само по себе не обрабатывает
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