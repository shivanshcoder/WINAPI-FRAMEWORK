#pragma once
//#include"Wrappers.hpp"
#include"Messages.h"
#include"WinProc.hpp"



namespace WINAPIPP {

	class CUSTOM_CLASS :public BaseWin, public BaseWinProc {
	public:

	protected:
		CUSTOM_CLASS(std::wstring ClassName)
			:ClassName(ClassName) {}
		//In Order to Define WinCLass properties and register the class
		virtual void ClassProp(WNDCLASS &wndClass) = 0;

		virtual bool CreateWin(std::wstring Tittle, DWORD style, Helpers::CPPRectangle size, BaseWin *_Parent = nullptr)override {			
			WNDCLASS wndclass = {};
			ClassProp(wndclass);
			wndclass.lpszClassName = ClassName.c_str();
			wndclass.lpfnWndProc = Procedure();
			if (!RegisterClass(&wndclass))
				//TODO replace with exceptions
				__debugbreak();
			HWND par = nullptr;
			if (_Parent) {
				Parent = *_Parent;
				par = *_Parent;
			}
			hwnd = CreateWindowW(ClassName.c_str(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				(par),
				NULL, Instance(), NULL);
			return hwnd;
		}

		 std::wstring ClassName;
		 BaseWin Parent;
	};

	
}