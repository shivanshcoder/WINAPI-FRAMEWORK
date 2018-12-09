#pragma once
#include"Wrappers.hpp"
#include"WinProc.hpp"


namespace WINAPIPP {


	class Application : public BaseWin, public WINAPIPP::BaseWinProc {

	public:

		virtual void InitClass(UINT style = CS_HREDRAW | CS_VREDRAW) {
			WNDCLASS wndClass;
			wndClass.lpszClassName = TEXT("MainWindow");
			wndClass.style = style;
			wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wndClass.cbClsExtra = 0;
			wndClass.cbWndExtra = 0;

			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hInstance = Instance();
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.lpszMenuName = NULL;
			wndClass.lpfnWndProc = Procedure;
			CheckError();
			if (!RegisterClass(&wndClass))
				__debugbreak();
			CheckError();
		}

		WPARAM Run() {
			ShowWindow(*this, WINAPIPP::CmdShow());
			UpdateWindow(*this);

			MSG msg;


			while (true) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT)
						break;

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				else {
					Idle();
				}
			}

			return msg.wParam;
		}

		virtual WPARAM start() = 0;

		virtual void Idle() {

		}

	private:
		//HWND hwnd;

	};
}


