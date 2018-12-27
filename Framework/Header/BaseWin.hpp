#pragma once
#include"Core.h"
#include"Helpers.h"
#include"WinProc.hpp"

namespace WINAPIPP {


	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName) {
		WNDCLASSEX wndclass = {};
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = Style;
		wndclass.lpfnWndProc = Proc;
		wndclass.hInstance = Instance();
		wndclass.hbrBackground = Background;
		wndclass.hIcon = Icon;
		wndclass.hCursor = Cursor;
		wndclass.lpszClassName = ClassName;
		wndclass.lpszMenuName = MenuName;
		wndclass.hIconSm = IconSm;
		if (!RegisterClassEx(&wndclass)) {
			CheckWinError();
			//URGENT
			//throw Exceptions::WinClass();
		}
		return true;
	}


	/*
	Cannot be copied or
	*/
	class BaseWin {
		friend class Window;
		friend class WrapperWin;
	public:
		BaseWin() {
			hwnd = NULL;
		}
		void Init(HWND t) {
			hwnd = t;
		}

		//TODO should it be protected?????
//	protected:
		operator HWND() {
			return hwnd;
		}


	private:
		HWND hwnd;
	};


	class WrapperWin :public BaseWin {
	public:
		Helpers::Rect GetClientRect() {
			Helpers::Rect rect;
			if (!::GetClientRect(hwnd, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		Helpers::Rect GetWinRect() {
			Helpers::Rect rect;
			if (!::GetWindowRect(hwnd, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		void InvalidateRect(Helpers::Rect rect, bool redraw) {
			if (!::InvalidateRect(hwnd, &rect.rect, redraw))
				throw std::exception("Error");

		}

		
	};


#pragma region CUSTOM_CLASS_MACROS
	//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) virtual LPCWSTR ClassName()override { return L###ClassName__; } 

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, IconSm, Cursor, Background, MenuName) DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = WINAPIPP::RegisterWinClass(Style, WINAPIPP::StaticWndProc , Icon, IconSm, Cursor, Background, MenuName, ClassName());	\
		return __ValidClass;\
}

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style, (LoadIcon(NULL, IDI_APPLICATION)), NULL, (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)
#pragma endregion
	
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
		HWND CreateWin(const std::wstring &Tittle, DWORD style, Helpers::Rect size, BaseWin *_Parent = nullptr, HMENU Menu = NULL) {
			bool ValidClass = __ClassProp();

			if (!ValidClass)
				throw std::exception("Class Not Registered");

			HWND par = nullptr;
			if (_Parent) {
				wndParent = *_Parent;
				par = *_Parent;
			}
			CheckWinError();
			hwnd = CreateWindowExW(0,ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				(par), //Parent Window
				Menu, Instance(),
				Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
			);
			CheckWinError();
			if (!hwnd)
				throw std::exception("Window Creation Unsuccessful");
			return hwnd;
		}


		BaseWin wndParent;
	};


}