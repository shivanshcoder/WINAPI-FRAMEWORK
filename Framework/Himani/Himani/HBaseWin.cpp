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


	//std::vector <HWinClassProperties::ClassProperties> HWinClassProperties::ClassList;

	void HCustomWindow::CreateWin(const HString & Title, DWORD style, Helpers::HRect size, HWindow* parent) {
		wndParent = parent;

		//bool ValidClass = __ClassProp();

		HWND parentHandle = NULL;
		if (parent)
			parentHandle = (HWND)*parent;

		/*if (!ValidClass)
			throw Himani::Exceptions(L"Class Not Registered");*/

		auto tempHandle = CreateWindowExW(0, ClassName().c_str(), //ClassName using virtual function
			Title.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			parentHandle, //Parent HWindow
			NULL, Instance(),
			Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
		);
		InitHandle(tempHandle);

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");

	}

	void HCustomWindow::UpdateProperties(HWND hwnd) {
		InitHandle(hwnd);
		SetWindowLongPtr(Handle(), GWLP_WNDPROC, (LONG_PTR)Procedure());
	}

	LRESULT HCustomWindow::MessageFunc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_GETPARENTINSTANCE:
			//Sends the Current Instance of the Class;
			return (LRESULT)this;

		}
	}

	void HPredefinedWindow::CreateWin(const HString & Title, DWORD style, Helpers::HRect size, HWindow *parent) {


		HWND parentHandle = NULL;
		if (parent)
			parentHandle = (HWND)* parent;

		auto tempHandle = CreateWindowExW(0, ClassName().c_str(), //ClassName using virtual function
			Title.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			(HWND)parentHandle, //Parent HCustomWindow
			NULL, Instance(), nullptr
		);

		//InitHandle(tempHandle);

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");


		//OldProc = (WNDPROC)SetWindowLongPtr(tempHandle, GWLP_WNDPROC, (LONG_PTR)Procedure());

	}

	inline LRESULT HPredefinedWindow::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		switch (message) {
		case WM_CREATE: {
			InitHandle(hwnd);
			OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Procedure());

			if (!OldProc)
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Procedure swap Unsuccessful");
		}
		}

		return OldProc(hwnd, message, wParam, lParam);
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