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

	void HCustomWindow::CreateWin(const HString& Title, DWORD style, HWindow* parent, Helpers::HRect size ) {
		//wndParent = parent;
		SelfDestruct = false;
		HWND parentHandle = nullptr;
		if (parent)
			parentHandle = (HWND)* parent;


		auto tempHandle = CreateWindowExW(0, ClassName().c_str(), //ClassName using virtual function
			Title.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			parentHandle, //Parent HWindow
			NULL, Instance(),
			(LPVOID)-1// This Stops from creating instance of Class from the common procedure
		);

		SendMessage(tempHandle, H_WM_SWAPPROCADDR, NULL, (LPARAM)Procedure());
		InitHandle(tempHandle);

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");

	}

	LRESULT HCustomWindow::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		
		switch (message) {
			
		}

		return DefWindowProc(Handle(), message, wParam, lParam);
	}

	LRESULT HCustomWindow::__MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_CREATE:
			//WARNING Is it necessary?
			InitHandle(hwnd);

		case H_WM_GETPARENTINSTANCE:
			//Sends the Current Instance of the Class;
			return (LRESULT)this;

		case WM_NCDESTROY:

			//WARNING 
			//Strange code for sure and should only run if class Instance was created using External calls only 
			//Not for Objects Intantited by Class itself
			//SelfDestruct if the Instance was created using External Call!
			if(SelfDestruct)
				delete this;
				return 0;
		}
		return MessageFunc(message, wParam, lParam);
	}

	

	HString HWindow::GetWinText() {
		int size = GetWindowTextLength(Handle());
		HString str;
		str.resize(size + 1);

		GetWindowText(Handle(), &str[0], size + 1);
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