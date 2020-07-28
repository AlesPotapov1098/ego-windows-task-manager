#ifndef E_OBJECT_FACTORY_H
#define E_OBJECT_FACTORY_H
#pragma once

#include <Windows.h>
#include <map>

namespace common{
	class EObjectFactory
	{
	public:
		EObjectFactory(HINSTANCE instance);
		~EObjectFactory();

		void OnCreateWindow(LPCSTR class_name, LPCSTR window_name);
			
	private:
		HINSTANCE hInstance;
		std::multimap<HWND,LPCSTR> _storage;
	};
}

#endif
