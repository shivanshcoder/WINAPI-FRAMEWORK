#include"Hpch.h"
#include"HBaseWin.h"

namespace Himani {
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
			CheckDefaultWinError;
		}
		return true;
	}

	//TODO make it void

	HWND HWindow::CreateWin(const HString & Tittle, DWORD style, Helpers::HRect size, HMENU Menu) {
		bool ValidClass = __ClassProp();

		if (!ValidClass)
			throw Himani::Exceptions(L"Class Not Registered");

		hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
			Tittle.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			wndParent.hwnd, //Parent HWindow
			Menu, Instance(),
			Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
		);

		if (!hwnd)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Creation Unsuccessful");
		return hwnd;
	}
	void HPredefinedWindow::CreateWin(const HString & Tittle, DWORD style, Helpers::HRect size, HMENU Menu) {

		hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
			Tittle.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			Parent(), //Parent HWindow
			Menu, Instance(), nullptr
		);
		if ((!hwnd))
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Creation Unsuccessful");
		OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Procedure());

		if (!OldProc)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Procedure swap Unsuccessful");

	}


	HString HWrapperWin::GetWinText() {
		int size = GetWindowTextLength(Handle());
		HString str;
		str.resize(size);

		GetWindowText(Handle(), &str[0], size);
		return str;
	}
	Helpers::HRect HWrapperWin::GetClientRect() {
		Helpers::HRect rect;
		::GetClientRect(Handle(), &rect.rect);

		return rect;
	}
	Helpers::HRect HWrapperWin::GetWinRect() {
		Helpers::HRect rect;
		::GetWindowRect(Handle(), &rect.rect);
		return rect;
	}
}