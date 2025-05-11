#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define _UNICODE
#include "framework.h"
#include "resource.h"
#include "functions.h"
#include <windows.h>
#include <string>

// Global
HINSTANCE hInst;
HWND hEditInput, hEditOutput, hButton;

// Function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry Point
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ScannerApp";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
    RegisterClass(&wc);

    hInst = hInstance;

    HWND hWnd = CreateWindow(L"ScannerApp", L"Tiny Language Scanner",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 500,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

        // Input Edit Control
        hEditInput = CreateWindow(L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_VSCROLL,
            20, 20, 540, 180, hWnd, (HMENU)IDC_EDIT_INPUT, hInst, NULL);
        SendMessage(hEditInput, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Run Scanner Button
        hButton = CreateWindow(L"BUTTON", L"Run Scanner",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            230, 220, 120, 30, hWnd, (HMENU)IDC_BUTTON_RUN, hInst, NULL);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Output Edit Control
        hEditOutput = CreateWindow(L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | ES_WANTRETURN | WS_VSCROLL,
            20, 270, 540, 160, hWnd, (HMENU)IDC_EDIT_OUTPUT, hInst, NULL);
        SendMessage(hEditOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    break;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON_RUN) {
            wchar_t wbuffer[4096];
            GetWindowTextW(hEditInput, wbuffer, 4096);

            // Convert wide char to std::string
            char buffer[4096];
            size_t convertedChars = 0;
            wcstombs_s(&convertedChars, buffer, 4096, wbuffer, 4096);

            // Run scanner and get result
            std::string result = parseProgram(tokenize(runScanner(buffer)));

            // Convert std::string to wide string
            wchar_t wresult[8192]; // increased buffer to be safe
            mbstowcs_s(&convertedChars, wresult, 8192, result.c_str(), 8192);

            // Show result in the output box
            SetWindowTextW(hEditOutput, wresult);
            SetWindowTextW(hEditInput, L"");
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