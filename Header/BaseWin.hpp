#pragma once
#include"WinProc.hpp"
#include<utility>
#include<memory>
#include<string>


struct WindowClassEx :tagWNDCLASSEXW{
	inline WindowClassEx (LPCWSTR ClassName) {
		style = CS_HREDRAW | CS_VREDRAW;
		lpfnWndProc = nullptr;
		cbClsExtra = 0;
		cbWndExtra = 0;
		hInstance = GetModuleHandle (NULL);;
		hCursor = LoadCursor (NULL, IDC_ARROW);
		hIcon = LoadIcon (NULL, IDI_APPLICATION);
		hIconSm = NULL;
		hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
		lpszMenuName = nullptr;
		lpszClassName = ClassName;
		cbSize = sizeof (WNDCLASSEX);
	}
	inline void DefaultInit () {

	}

	inline ATOM Register () {
		ATOM Val;
		if (Val = RegisterClassEx (this))
			return Val;
		CheckError ();
		return 0;
		//else
		// Maybe throw exception
			//throw 0;
	}
	inline void ChangeStyle (UINT _style) {
		style = _style;
	}
	inline void ChangeCursor (HCURSOR Cursor) {
		hCursor = Cursor;
	}
	inline void ChangeIcon (HICON Icon) {
		hIcon = Icon;
	}
	inline void ChangeIconSm (HICON Icon) {
		hIconSm = Icon;
	}
	inline void ChangeBackground (HBRUSH Brush) {
		hbrBackground = Brush;
	}
	inline void ChangeMenuName (LPCWSTR MenuName) {
		lpszMenuName = MenuName;
	}
	inline void ChangeClassName (LPCWSTR ClassName) {
		lpszClassName = ClassName;
	}
	inline void AttachProc (WNDPROC Proc) {
		lpfnWndProc = Proc;
		
	}
};

namespace WINAPIPP {

	class WIN {
	public:
		operator HWND() {
			return Window;
		}

	private:
		HWND Window;
	};

	class BaseWin :public BaseWinProc {

	public:

		BaseWin () {
			if (!WinClassProp) {
				GenerateDefaultClass ();
			}

		}
		virtual inline int MessageLoop ();
		virtual inline int Run ();
		virtual inline void Idle () {

		}

		//Message Virtual Function can be overridden
		virtual void OnCreate (CREATESTRUCT CreateStruct) {}

		virtual void OnPaint (HDC & hdc, PAINTSTRUCT & ps) {}

		virtual void OnClose () {}

		virtual void OnDestroy () {}

		virtual void Create (std::wstring WindowName);

		virtual LRESULT MessageFunc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override;

		virtual LRESULT WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { return DefWindowProc (hwnd, message, wParam, lParam); }

		operator HWND() {
			return Window;
		}

	private:

		std::wstring ClassName;

		ATOM GenerateDefaultClass ();

		WindowClassEx * WinClassProp;	

		//Represents if the class was registered or not
		//Should be Static i guess
		bool ClassRegistered;

		HWND Window;
	};

	//The message loop which handles and dispatches WindowsMessages
	int BaseWin::MessageLoop () {
		bool Running = true;
		MSG message;
		while (Running) {

			while (PeekMessage (&message, 0, 0, 0, PM_REMOVE)) {
				if (message.message == WM_QUIT)
					
					Running = false;

				TranslateMessage (&message);
				DispatchMessage (&message);
			}

			Idle ();
		}
		return message.wParam;
	}



	int BaseWin::Run () {
		if (!ClassRegistered)
			GenerateDefaultClass ();
		ShowWindow (Window, SW_SHOWNORMAL);
		UpdateWindow (Window);
		return (int)MessageLoop ();

	}

}