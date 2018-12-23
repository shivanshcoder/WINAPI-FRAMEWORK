#pragma once
//#include"Wrappers.hpp"
#include"Messages.h"
#include"WinProc.hpp"


#pragma region CUSTOM_CLASS_MACROS
//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) virtual LPCWSTR ClassName()override { return L###ClassName__; } 

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, Cursor, Background, MenuName) DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = WINAPIPP::RegisterWinClass(Style, WINAPIPP::StaticWndProc , Icon, Cursor, Background, MenuName, ClassName());	\
		return __ValidClass;\
}

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style, (LoadIcon(NULL, IDI_APPLICATION)), (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)
#pragma endregion

namespace WINAPIPP {

	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName) {
		WNDCLASS wndclass = {};
		wndclass.style = Style;
		wndclass.lpfnWndProc =Proc/* StaticWndProc*/;
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
	
	/*
	------------------------	Custom Window Classes ------------------------------
	Derive from Window Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
	*/
	class Window :public WrapperWin, public BaseWinProc {
	
	public:
		BaseWin Parent() { return wndParent; }
	protected:

		//Will be overriden using macro CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
		virtual LPCWSTR ClassName() = 0;
		virtual bool __ClassProp() { return 0; }
		
		
		//TODO make it void
		HWND CreateWin(const std::wstring &Tittle, DWORD style, Helpers::Rect size, BaseWin *_Parent = nullptr, HMENU Menu =  NULL) {			

			bool ValidClass = __ClassProp();

			if (!ValidClass)
				throw std::exception("Class Not Registered");

			HWND par = nullptr;
			if (_Parent) {
				wndParent = *_Parent;
				par = *_Parent;
			}
			CheckError();
			hwnd = CreateWindowW(ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				(par), //Parent Window
				Menu, Instance(),
				Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
			);
			CheckError();
			if (!hwnd)
				throw std::exception("Window Creation Unsuccessful");
			return hwnd;
		}


		 BaseWin wndParent;
	};

	
}