//  abc.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <StrSafe.h>
#include "Resource.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "gdi32.lib")

const LPCWSTR APP_NAME = L"abc";
const LPCWSTR APP_WNDCLASS = L"abcWndClass";


// log
static FILE* logfp = NULL;      // logging
static void log(LPCWSTR fmt, ...)
{
    if (logfp == NULL) return;
    WCHAR buf[1024];
    va_list args;
    va_start(args, fmt);
    StringCchVPrintf(buf, _countof(buf)-1, fmt, args);
    va_end(args);
    buf[1023] = L'\0';
    fwprintf(logfp, L"%s\n", buf);
    fflush(logfp);
}


//  App
//
class App
{
    HWND _hWnd;
    POINT _p;
    POINT _v;
    
public:
    App();
    ~App();
    void Initialize(HWND hWnd);
    void Uninitialize();
    LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

App::App()
{
    _hWnd = NULL;
    _v.x = _v.y = 8;
}

App::~App()
{
}

void App::Initialize(HWND hWnd)
{
    log(L"Initialize");

    _hWnd = hWnd;
    SetTimer(_hWnd, (UINT_PTR)this, 50, NULL);
}

void App::Uninitialize()
{
    log(L"Uninitialize");
    KillTimer(_hWnd, (UINT_PTR)this);
};

LRESULT App::HandleMessage(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    const LPCSTR TEXT = "This is ABC.";
    
    switch (uMsg) {
    case WM_TIMER:
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            _p.x += _v.x;
            _p.y += _v.y;
            if ((_p.x < rc.left && _v.x < 0) ||
                (rc.right < _p.x && 0 < _v.x)) {
                _v.x = -_v.x;
            }
            if ((_p.y < rc.top && _v.y < 0) ||
                (rc.bottom < _p.y && 0 < _v.y)) {
                _v.y = -_v.y;
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        return FALSE;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            TextOutA(hdc, _p.x, _p.y, TEXT, strlen(TEXT));
            EndPaint(hWnd, &ps);
        }
        return FALSE;

    case WM_SIZE:
        {
            RECT rc;
            GetClientRect(hWnd, &rc);
            _p.x = (rc.left + rc.right)/2;
            _p.y = (rc.top + rc.bottom)/2;
        }
        return FALSE;

    case WM_CLOSE:
	DestroyWindow(hWnd);
	return FALSE;

    default:
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


//  appWndProc
//
static LRESULT CALLBACK appWndProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            App* app = (App*)(cs->lpCreateParams);
            if (app != NULL) {
                SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
                app->Initialize(hWnd);
            }
        }
	return FALSE;
    
    case WM_DESTROY:
        if (hWnd != NULL) {
            App* app = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (app != NULL) {
                app->Uninitialize();
            }
        }
	PostQuitMessage(0);
	return FALSE;
        
    default:
        if (hWnd != NULL) {
            App* app = (App*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            if (app != NULL) {
                return app->HandleMessage(hWnd, uMsg, wParam, lParam);
            }
        }
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


//  AppMain
// 
int AppMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    int nCmdShow,
    int argc, LPWSTR* argv)
{
    // Register the window class.
    ATOM atom;
    {
	WNDCLASS klass = {0};
	klass.lpfnWndProc = appWndProc;
	klass.hInstance = hInstance;
	klass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
        klass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	klass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        klass.lpszClassName = APP_WNDCLASS;
	atom = RegisterClass(&klass);
    }

    // Create an App object.
    App* app = new App();
    
    // Create a main window.
    HWND hWnd = CreateWindow(
	(LPCWSTR)atom,
	APP_NAME,
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT,
	CW_USEDEFAULT, CW_USEDEFAULT,
	NULL, NULL, hInstance, app);
    ShowWindow(hWnd, nCmdShow);

    // Event loop.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete app;
    
    return (int)msg.wParam;
}


// WinMain and wmain
#ifdef WINDOWS
int WinMain(HINSTANCE hInstance, 
	    HINSTANCE hPrevInstance, 
	    LPSTR lpCmdLine,
	    int nCmdShow)
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
#ifdef DEBUG
    _wfopen_s(&logfp, L"log.txt", L"a");
#endif
    return AppMain(hInstance, hPrevInstance, nCmdShow, argc, argv);
}
#else
int wmain(int argc, wchar_t* argv[])
{
    logfp = stderr;
    return AppMain(GetModuleHandle(NULL), NULL, SW_SHOWDEFAULT, argc, argv);
}
#endif
