#include "Hpch.h"
#include "Application.h"

namespace Himani {
	WPARAM Himani::HApplication::Run() {
		Show(Himani::CmdShow());
		Update();
		return MessageProcess();
	}

	WPARAM Himani::HApplication::MessageProcess() {
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

	LRESULT HApplication::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
		switch (message) {
		case WM_SIZE:
			return OnSize(wParam,lParam);

		case WM_DESTROY:
			return OnDestroy();
		}
		return HCustomWindow::MessageFunc(hwnd, message, wParam, lParam);
	}



}