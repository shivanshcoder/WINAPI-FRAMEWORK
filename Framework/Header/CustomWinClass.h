#pragma once
#include"Wrappers.hpp"
#include"WinProc.hpp"

#define DECLARE_MESSAGE_MAP() LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)final

#define MESSAGE_MAP_END()	}						\
return DefWindowProc(hwnd, message, wParam, lParam); }

#define MESSAGE_MAP_BEGIN(CLASS_NAME)	LRESULT CLASS_NAME::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {  \
		switch(message){															
#define MESSAGE_MAP_ENTRY_PARAMS(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC(wParam, lParam);

#define MESSAGE_MAP_ENTRY(FUNC, MESSAGE_VAL) case MESSAGE_VAL: return FUNC();



namespace WINAPIPP {

	class CUSTOM_CLASS :public BaseWin, public BaseWinProc {
	public:

	protected:
		//In Order to Define WinCLass properties and register the class
		virtual void ClassProp(WNDCLASS &wndClass) = 0;

		virtual bool CreateWin(std::wstring Tittle, DWORD style, WINAPIPP::Rectangle size, BaseWin const &Parent)override {
			
			
			WNDCLASS wndclass;
			ClassProp(wndclass);
			wndclass.lpszClassName = ClassName.c_str();
			wndclass.lpfnWndProc = Procedure();
			if (!RegisterClass(&wndclass))
				//TODO replace with exceptions
				__debugbreak();


			hwnd = CreateWindow(ClassName.c_str(), Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				Parent.hwnd, NULL, Instance(), NULL);
			return hwnd;
		}

		virtual WPARAM Run() {

			ShowWindow(*this, WINAPIPP::CmdShow());
			UpdateWindow(*this);
			
			return MessageProcess();
		}

		virtual WPARAM MessageProcess() {
			MSG msg;


			while (true) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT)
						break;

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			return msg.wParam;
		}

		//DECLARE_MESSAGE_MAP();

		const std::wstring ClassName;
	};

	
}