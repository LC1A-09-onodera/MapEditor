#include "WinAPI.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"

HWND WindowsAPI::hwnd{};
MSG WindowsAPI::msg{};
WNDCLASSEX WindowsAPI::w{};
bool WindowsAPI::Qite;
int WindowsAPI::intarval;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
float WindowsAPI::rate;

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // メッセージで分岐
    switch (msg) {
    case WM_DESTROY: // ウィンドウが破棄された
        PostQuitMessage(0); // OSに対して、アプリの終了を伝える
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    return DefWindowProc(hwnd, msg, wparam, lparam);// 標準の処理を行う
}

void WindowsAPI::Set()
{
    //WNDCLASSEX w{}; // ウィンドウクラスの設定
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
    w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
    w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
    w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定
    // ウィンドウクラスをOSに登録
    RegisterClassEx(&w);
    // ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX), false); // 自動でサイズ補正

    hwnd = CreateWindow(w.lpszClassName, // クラス名
        L"ll[thello]",         // タイトルバーの文字
        (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX),        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);
    msg;  // メッセージ
}

void WindowsAPI::Clear()
{
    UnregisterClass(w.lpszClassName, w.hInstance);
}

void WindowsAPI::CheckMsg()
{
    // メッセージがある？
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg); // キー入力メッセージの処理
        DispatchMessage(&msg); // プロシージャにメッセージを送る
    }
    Update();
}

void WindowsAPI::Update()
{
    auto hdc = GetDC(hwnd);
    rate = GetDeviceCaps(hdc, VREFRESH);
    intarval = 0;
    intarval = rate / 60.0f;
    if (msg.message == WM_QUIT)
    {
        Qite = true;
    }
}

void WindowsAPI::Gethwnd()
{

}

POINT WindowsAPI::GetMousePos()
{
    POINT result;
    GetCursorPos(&result);
    ScreenToClient(hwnd, &result);

    return result;
}


