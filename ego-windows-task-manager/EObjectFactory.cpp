#include "EObjectFactory.h"

namespace common
{

	HWND __stdcall ECreateWindow(HINSTANCE inst,LPCSTR clname, LPCSTR wndname);

	EObjectFactory::EObjectFactory(HINSTANCE instance)
	{
	}

	EObjectFactory::~EObjectFactory()
	{
	}

	void EObjectFactory::OnCreateWindow(LPCSTR class_name, LPCSTR window_name)
	{
		HWND window = ECreateWindow(hInstance,class_name,window_name);
		if (!window) {
			return;
		}

		_storage.insert(std::pair<HWND,LPCSTR>(window,window_name));
	}

	HWND __stdcall ECreateWindow(HINSTANCE inst, LPCSTR clname, LPCSTR wndname)
	{
		WNDCLASSEXA windowClass;
		std::memset(&windowClass, 0, sizeof(WNDCLASSEXA));

		windowClass.cbSize = sizeof(WNDCLASSEXA);
		windowClass.hInstance = inst;
		windowClass.lpfnWndProc = DefWindowProcA;
		windowClass.lpszClassName = clname;

		if (!RegisterClassExA(&windowClass)) {
			return nullptr;
		}

		HWND hwnd = CreateWindowExA(
			0,
			clname,
			wndname,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			500, 300, 500, 380,
			NULL, NULL, inst, NULL);

		return hwnd;
	}
}