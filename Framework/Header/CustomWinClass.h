#pragma once
//#include"Wrappers.hpp"
#include"Messages.h"
#include"WinProc.hpp"


#pragma region CUSTOM_CLASS_MACROS
//Function for Getting class Name
#define MAKE_CUSTOM_CLASS(ClassName__) virtual LPCWSTR ClassName() { return L###ClassName__; } 

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, Cursor, Background, MenuName) MAKE_CUSTOM_CLASS(ClassName__)	\
 bool __ClassProp() {		\
		static bool Created = false;	\
	if(!Created)	\
		WINAPIPP::RegisterWinClass(Style, Procedure(), Icon, Cursor, Background, MenuName, ClassName()); Created = true;	return true;\
}\
 bool const ValidClass = __ClassProp(); 

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style,	\
														(LoadIcon(NULL, IDI_APPLICATION)), (LoadCursor(NULL, IDC_ARROW)),	\
															(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)
#pragma endregion

namespace WINAPIPP {

	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName) {
		WNDCLASS wndclass = {};
		wndclass.style = Style;
		wndclass.lpfnWndProc = Proc;
		wndclass.hInstance = Instance();
		wndclass.hbrBackground = Background;
		wndclass.hIcon = Icon;
		wndclass.hCursor = Cursor;
		wndclass.lpszClassName = ClassName;
		wndclass.lpszMenuName = MenuName;
		if (!RegisterClass(&wndclass)) {
			CheckError();
		//TODO uncomment it maybe
			//	throw Exceptions::WinClass();
		}
		return true;
	}
	bool temp(const int t) {
		return t;
	}
	/*
	------------------------	Custom Window Classes ------------------------------
	Derive from Window Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
	*/
	class Window :public BaseWin, public BaseWinProc {
	
	public:
		BaseWin Parent() { return wndParent; }
	protected:

		virtual LPCWSTR ClassName() = 0;
		//TODO make it void
		HWND CreateWin(const std::wstring &Tittle, DWORD style, Helpers::CPPRectangle size, BaseWin *_Parent = nullptr, HMENU Menu =  NULL) {			

			HWND par = nullptr;
			if (_Parent) {
				wndParent = *_Parent;
				par = *_Parent;
			}
			CheckError();
			hwnd = CreateWindowW(ClassName(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				(par),
				Menu, Instance(), NULL);

			if (!hwnd)
				throw std::exception("Window Creation Unsuccessful");
			return hwnd;
		}


		 BaseWin wndParent;
	};

	
}