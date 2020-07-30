#ifndef E_COMMON_OBJECT
#define E_COMMON_OBJECT
#pragma once

#include <Windows.h>

namespace egui{
	class ECommonObject
	{
	public:
		ECommonObject(LPCSTR ObjectName);
		virtual ~ECommonObject() = 0;

		virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lPraram) = 0;
		virtual LRESULT Draw() = 0;
		virtual LRESULT Resize();
		

	private:
		HWND hWnd;
		LPCSTR ClassObjectName;
		LPCSTR ObjectName;
	};
}
#endif
