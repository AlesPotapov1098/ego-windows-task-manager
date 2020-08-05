#include <iostream>
#include <Windows.h>

#define local static

#include "EObjectFactory.h"

typedef HWND (__stdcall * create_window)(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name);
typedef HWND (__stdcall * create_child_window)(HINSTANCE instance, HWND parent, LPCSTR class_name, LPCSTR window_name);

HWND CreateEWindow(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name);
HWND CreateEChildWindow(HINSTANCE instance, HWND parent, LPCSTR class_name, LPCSTR window_name);
BOOL RegisterWindow(HINSTANCE instance, LPCSTR classname);

LRESULT CALLBACK MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);


/*Сделать жти переменные полями будущего класса ECommonObject*/
local RECT				ClientWindowRect;
local HDC				MainWindowDC = nullptr;
local BITMAPINFO		MainWindowBitMapInfo;
local BITMAPINFOHEADER	MainWindowBitMapHeader;
local HBITMAP			MainWindowBitMap = nullptr;
local LPVOID			BitMapMemory;

struct WINDOW_RECT
{
	uint32_t SrcWidth;
	uint32_t SrcHeight;
	uint32_t SrcX;
	uint32_t SrcY;

	uint32_t DestWidth;
	uint32_t DestHeight;
	uint32_t DestX;
	uint32_t DestY;
};

WINDOW_RECT MainWindowSize;

void InitBitMapSpace(HWND window);
void UpdateWindow(HDC device);


int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nShowCmd)
{

	LPCSTR MainWindowClassName = "MainWindowClass";
	LPCSTR MainWindowName = "MainWindow";

	LPCSTR ChildWindowClassName = "ChildWindow";
	LPCSTR ChildWindowName = ChildWindowClassName;

	if (!RegisterWindow(hInstance, MainWindowClassName))
	{
		return -1;
	}

	HWND MainWindow = CreateEWindow(hInstance,MainWindowClassName,MainWindowName);

	if (MainWindow)
	{
		ShowWindow(MainWindow,SW_SHOWDEFAULT);

		MSG mess;
		while (GetMessageA(&mess, NULL, 0, 0)) {
			TranslateMessage(&mess);
			DispatchMessage(&mess);
		}
	}

	return 0;
}

HWND CreateEWindow(HINSTANCE instance, LPCSTR class_name, LPCSTR window_name)
{
	HWND hwnd = CreateWindowExA(
		0,
		class_name,
		window_name,
		WS_OVERLAPPEDWINDOW,
		500, 300, 500, 380,
		nullptr, nullptr, instance, nullptr);

	return hwnd;
}

HWND CreateEChildWindow(HINSTANCE instance, HWND parent, LPCSTR class_name, LPCSTR window_name)
{
	HWND hwnd = CreateWindowExA(
		0,
		class_name,
		window_name,
		WS_CHILDWINDOW,
		500, 300, 500, 380,
		parent,nullptr, instance, nullptr);

	return hwnd;
}

BOOL RegisterWindow(HINSTANCE instance, LPCSTR classname)
{
	WNDCLASSEXA windoclass;
	std::memset(&windoclass,0,sizeof(WNDCLASSEXA));

	windoclass.cbSize = sizeof(WNDCLASSEXA);
	windoclass.style = CS_HREDRAW | CS_VREDRAW;
	windoclass.hInstance = instance;
	windoclass.lpfnWndProc = &MainWindowProc;
	windoclass.lpszClassName = classname;

	return RegisterClassExA(&windoclass);
}

LRESULT MainWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:{
		}break;

		case WM_CLOSE:{
		}break;
	
		case WM_DESTROY:{
		}break;

		case WM_PAINT:{
			PAINTSTRUCT paint;
			HDC DevContext = BeginPaint(window,&paint);
			HDC mem = CreateCompatibleDC(DevContext);
			int width = ClientWindowRect.right - ClientWindowRect.left;
			int height = ClientWindowRect.bottom - ClientWindowRect.top;
			BITMAPINFO info;
			info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			info.bmiHeader.biWidth = width;
			info.bmiHeader.biHeight = height;
			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biCompression = BI_RGB;
			HBITMAP bitmap = CreateDIBSection(mem,&info,DIB_RGB_COLORS,&BitMapMemory,0,0);
			SelectObject(mem,bitmap);
			StretchDIBits(mem,0,0,width,height,0,0,width,height,BitMapMemory,&info,DIB_RGB_COLORS,SRCCOPY);
			//BitBlt(DevContext,0,0,width,height,mem,0,0,SRCCOPY);
			DeleteObject(bitmap);
			DeleteDC(mem);
			EndPaint(window, &paint);
		}break;

		case WM_SIZE:{
			GetClientRect(window,&ClientWindowRect);
			MainWindowSize.DestX = ClientWindowRect.top;
			MainWindowSize.DestY = ClientWindowRect.right;
			MainWindowSize.DestWidth = ClientWindowRect.right - ClientWindowRect.left;
			MainWindowSize.DestHeight = ClientWindowRect.bottom - ClientWindowRect.top;
			//InitBitMapSpace(window);
		}break;

		default:
			break;
	}

	return DefWindowProcA(window,message,wParam,lParam);
}

void InitBitMapSpace(HWND window)
{
	if(!MainWindowDC)
		MainWindowDC = GetDC(window);

	MainWindowBitMapHeader.biSize = sizeof(BITMAPINFOHEADER);
	MainWindowBitMapHeader.biWidth = MainWindowSize.DestWidth;
	MainWindowBitMapHeader.biHeight = MainWindowSize.DestHeight;
	MainWindowBitMapHeader.biBitCount = 32;
	MainWindowBitMapHeader.biCompression = BI_RGB;
	MainWindowBitMapHeader.biPlanes = 1;

	MainWindowBitMapInfo.bmiHeader = MainWindowBitMapHeader;
	
	uint32_t BitmapSize = MainWindowSize.DestHeight * MainWindowSize.DestWidth * 4;
	BitMapMemory = VirtualAlloc(0,BitmapSize,MEM_COMMIT,PAGE_READWRITE);
}

void UpdateWindow(HDC device)
{
	StretchDIBits(device,
				  MainWindowSize.DestX,MainWindowSize.DestY,MainWindowSize.DestWidth, MainWindowSize.DestHeight,
				  MainWindowSize.SrcX,MainWindowSize.SrcY,MainWindowSize.SrcWidth,MainWindowSize.SrcHeight,
				  &BitMapMemory,
				  &MainWindowBitMapInfo,
				  DIB_RGB_COLORS,SRCCOPY);
}
