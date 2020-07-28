#ifndef E_MAIN_WINDOW_H
#define E_MAIN_WINDOW_H
#pragma once

#include <Windows.h>

namespace gui{

	const LPCSTR MAIN_WINDOW_CLASS_NAME = "MainWindowClass";

	class EMainWindow
	{
	public:
		EMainWindow();
		~EMainWindow();
	private:
		HWND hwnd;
		LPCSTR window_name;
	};
}

#endif
