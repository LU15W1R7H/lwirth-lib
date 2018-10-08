//
// Created by Luis on 07-Oct-18.
//

#ifndef LWIRTH_WINDOWSHTEST_HPP
#define LWIRTH_WINDOWSHTEST_HPP


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
switch(uMsg)
{
case WM_CLOSE:
PostQuitMessage(0);
break;
case WM_CHAR:
if(wParam == VK_ESCAPE)
{
SendMessage(hwnd, WM_CLOSE, 0, 0);
}
break;
case WM_PAINT:
{
PAINTSTRUCT ps;
HDC hdc = BeginPaint(hwnd, &ps);
auto text = "Hallo Windows.h";
TextOut(hdc, 10, 10, text, std::strlen(text));

EndPaint(hwnd, &ps);
}
break;
default:
break;
}
return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND g_windowHandle = nullptr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
WNDCLASSEX windowClass;
ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
windowClass.cbSize = sizeof(WNDCLASSEX);
windowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
windowClass.lpfnWndProc = WindowProc;
windowClass.hInstance = hInstance;
windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
windowClass.lpszClassName = "WindowClass";

RegisterClassEx(&windowClass);

g_windowHandle = CreateWindowEx(NULL, "WindowClass", "Hallo Windows.h", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 200, 200, 800, 600, NULL, NULL, hInstance, NULL);

if(g_windowHandle == nullptr)
{
DWORD errVal = GetLastError();
}

MSG msg;
while(true)
{
PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
if(msg.message == WM_CLOSE)break;

TranslateMessage(&msg);
DispatchMessage(&msg);
}

return 0;
}

#endif //LWIRTH_WINDOWSHTEST_HPP
