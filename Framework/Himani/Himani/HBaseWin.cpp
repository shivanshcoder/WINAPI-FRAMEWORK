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

	LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		if (message == WM_NCCREATE) {
			auto createArguments = (LPCREATESTRUCT)lParam;
			if (createArguments->lpCreateParams != (LPVOID)-1) {
				HCustomWindow* ClassInstance;

				if (createArguments->lpCreateParams == NULL)
					ClassInstance = new ReservedTempWindow(createArguments->lpszClass, hwnd);
			}

			return TRUE;
		}

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	//std::vector <HWinClassProperties::ClassProperties> HWinClassProperties::ClassList;

	//HString& HCustomWindow::ClassName() { static HString temp = (TEXT("NO_CLASS")); return temp; }

	void HCustomWindow::CreateWinEx(const HString& Title, DWORD style, DWORD ExStyle, HWindow* parent, Helpers::HRect size){
		//SelfDestruct = false;
		HWND parentHandle = nullptr;
		if (parent)
			parentHandle = static_cast<HWND>(* parent);

		auto tempHandle = CreateWindowExW(ExStyle, ClassName().c_str(), //ClassName using virtual function
			Title.c_str(), style,
			size.left, size.top, size.right, size.bottom,
			parentHandle, //Parent HWindow
			NULL, Instance(),
			(LPVOID)-1// This Stops from creating instance of Class from the common procedure
		);
		InitHandle(tempHandle);
		//SetWindowLongPtr(tempHandle, GWLP_WNDPROC, (LPARAM)Procedure());
		UpdateProc();

		if (!tempHandle)
			throw WinExceptions(__LINE__, TEXT(__FILE__) L"HCustomWindow Creation Unsuccessful");
	}

	LRESULT HCustomWindow::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		/*switch (message) {
		case WM_COMMAND: {
			if (lParam) {
				SendMessage((HWND)lParam, HIWORD(wParam), LOWORD(wParam), 0);
			}
		}
		}*/

		return DefWindowProc(Handle(), message, wParam, lParam);
	}

	LRESULT HCustomWindow::__MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_CREATE:
			//WARNING Is it necessary?
			//InitHandle(hwnd);

		case H_WM_GETOWNINSTANCE:
			//Sends the Current Instance of the Class;
			return (LRESULT)this;


		case WM_COMMAND: {
			if (lParam) {
				//Framework Controls can automatically use the notification by themselves
				int temp =	SendMessage((HWND)lParam, H_CM_PROCESSNOTIF, HIWORD(wParam), LOWORD(wParam));
				if (temp != -1)
					break;
				return 0;
			}
		}

		//case WM_NCDESTROY:

			//WARNING 
			//Strange code for sure and should only run if class Instance was created using External calls only 
			//Not for Objects Intantited by Class itself(Stack Creation)
			//SelfDestruct if the Instance was created using External Call!
			//if(SelfDestruct)
			//	delete this;
			//	return 0;
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