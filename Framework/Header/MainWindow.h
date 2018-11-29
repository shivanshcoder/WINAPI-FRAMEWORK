#pragma once
#include"BaseWin.hpp"
#include"WinProc.hpp"
#define START_WINDOW(Class) typedef Class MAIN_CLASS

namespace WINAPIPP {

	HINSTANCE Instance;
	int CmdShow;
	

	class MainWindow : public BaseWin, public WINAPIPP::BaseWinProc {
    public:
		virtual void InitClass() {
			WNDCLASS wndClass;
			wndClass.lpszClassName = TEXT("MainWindow");
			wndClass.style = CS_HREDRAW | CS_VREDRAW;
			wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wndClass.cbClsExtra = 0;
			wndClass.cbWndExtra = 0;

			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hInstance = Instance;
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.lpszMenuName = NULL;
			wndClass.lpfnWndProc = Procedure;
			CheckError();
			if(!RegisterClass(&wndClass))
				__debugbreak();
			CheckError();
		}

		WPARAM Run() {
			ShowWindow(*this, CmdShow);
			UpdateWindow(*this);

			MSG msg;

			CheckError();
			while (GetMessage(&msg, NULL, NULL, NULL)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				CheckError();
			}
			return msg.wParam;
		}

        virtual WPARAM start() = 0;



	private:
		//HWND hwnd;

	};
}


