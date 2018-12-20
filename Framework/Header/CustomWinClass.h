#pragma once
//#include"Wrappers.hpp"
#include"Messages.h"
#include"WinProc.hpp"



//#define MAKE_CUSTOM_CLASS(ClassName__)	static constexpr const wchar_t* ClassName = L###ClassName__;
#define MAKE_CUSTOM_CLASS(ClassName__)	virtual const wchar_t* ClassName()override{ return L###ClassName__; }

#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, Cursor, Background, MenuName) MAKE_CUSTOM_CLASS(ClassName__)	\
bool ValidClass = __ClassProp(); \
bool __ClassProp() {				\
		return RegisterWinClass(Style, Procedure(), Icon, Cursor, Background, MenuName, ClassName());	\
}

#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style,	\
														(LoadIcon(NULL, IDI_APPLICATION)), (LoadCursor(NULL, IDC_ARROW)),	\
															(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)

//#define CLASS_PROPERTIES(Style, MenuName ) CLASS_ALL_PROPERTIES(Style, )
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
			throw Exceptions::WinClass();
		}
		return true;
	}

	class Window :public BaseWin, public BaseWinProc {
	public:

	private:

	protected:

		virtual const wchar_t* ClassName() = 0;

		virtual bool CreateWin(std::wstring Tittle, DWORD style, Helpers::CPPRectangle size, BaseWin *_Parent = nullptr)override {			

			HWND par = nullptr;
			if (_Parent) {
				Parent = *_Parent;
				par = *_Parent;
			}
			hwnd = CreateWindowW(ClassName(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				(par),
				NULL, Instance(), NULL);
			return hwnd;
		}

		 BaseWin Parent;
	};

	
}