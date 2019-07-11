#include "Hpch.h"
#include "SampleWinClasses.h"

namespace Himani {
	void HSimpleWindow::OnMenu() {

	}
	LRESULT HSimpleWindow::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {

		case WM_COMMAND: {
			if ((!lParam) && (!HIWORD(wParam))) {
				OnMenu();
				return 0;
			}
			break;
		}
		

		case WM_MENUCOMMAND: {
			int a = wParam;
			int c = lParam;
			break;
		}
		case WM_SETFOCUS: {
			return OnFocus();
		}

		}

		return HCustomWindow::MessageFunc(message, wParam, lParam);
	}
}
