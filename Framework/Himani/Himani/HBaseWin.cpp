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


	void HCustomWindow::CreateWin(const HString & Tittle, DWORD style, Helpers::HRect size, HMENU Menu) {
		bool ValidClass = __ClassProp();

		if (!ValidClass)
			throw Himani::Exceptions(L"Class Not Registered");

		auto tempHandle = CreateWindowExW(0, ClassName(), //ClassName using virtual function
			Tittle.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			(HWND)wndParent, //Parent HCustomWindow
			Menu, Instance(),
			Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
		);
		InitHandle(tempHandle);

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");

	}
	void HPredefinedWindow::CreateWin(const HString & Tittle, DWORD style, Helpers::HRect size, HMENU Menu) {

		auto tempHandle = CreateWindowExW(0, ClassName(), //ClassName using virtual function
			Tittle.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			(HWND)Parent(), //Parent HCustomWindow
			Menu, Instance(), nullptr
		);
		InitHandle(tempHandle);

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");


		OldProc = (WNDPROC)SetWindowLongPtr(tempHandle, GWLP_WNDPROC, (LONG_PTR)Procedure());


		if (!OldProc)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Procedure swap Unsuccessful");

	}


	HString HWindow::GetWinText() {
		int size = GetWindowTextLength(Handle());
		HString str;
		str.resize(size);

		GetWindowText(Handle(), &str[0], size);
		return str;
	}
	Helpers::HRect HWindow::GetClientRect() {
		Helpers::HRect rect;
		::GetClientRect(Handle(), &rect.rect);

		return rect;
	}
	Helpers::HRect HWindow::GetWinRect() {
		Helpers::HRect rect;
		::GetWindowRect(Handle(), &rect.rect);
		return rect;
	}
}