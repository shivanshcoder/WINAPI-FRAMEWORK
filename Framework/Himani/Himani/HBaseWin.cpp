#include"Hpch.h"
#include "HBaseWin.h"

bool Himani::RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName) {
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
		CheckDefaultWinError;
	}
	return true;
}

//Return Empty HBaseWin as nullptr

inline Himani::HBaseWin::HBaseWin() {
	hwnd = nullptr;
}

inline Himani::HBaseWin::HBaseWin(HWND _hwnd) {
	hwnd = _hwnd;
}

//Doesn't check if the handle is nullptr or not

inline Himani::HBaseWin::operator HWND() const {
	return hwnd;
}

inline Himani::HBaseWin::~HBaseWin() {}

//Checks if the handle to window is nullptr or not

inline HWND Himani::HBaseWin::Handle() {
	if (!hwnd)
		throw Himani::Exceptions(L"nullptr Window Handle");
	return hwnd;
}
