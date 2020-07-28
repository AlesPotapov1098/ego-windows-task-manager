#include <iostream>
#include <Windows.h>

typedef HWND (__stdcall * create_window)(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name);
typedef HWND (__stdcall * create_child_window)(HINSTANCE instance, HWND parent, LPCSTR class_name, LPCSTR window_name);

HWND CreateEWindow(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name);

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nShowCmd)
{
	create_window wnd = &CreateEWindow;
	HWND window = wnd(hInstance,"First Window","EGO Task Manager");
	return 0;
}

HWND CreateEWindow(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name)
{
	WNDCLASSEXA windowClass;
	std::memset(&windowClass, 0, sizeof(WNDCLASSEXA));

	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.hInstance = instance;
	windowClass.lpfnWndProc = DefWindowProcA;
	windowClass.lpszClassName = class_name;

	if (!RegisterClassExA(&windowClass)) {
		return nullptr;
	}

	HWND hwnd = CreateWindowExA(
		0,
		class_name,
		window_name,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 300, 500, 380,
		NULL, NULL, instance, NULL);

	return hwnd;
}
