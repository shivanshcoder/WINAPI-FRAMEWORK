#include "Hpch.h"
#include "SampleWinClasses.h"

namespace Himani {
	void HSimpleWindow::OnMenu() {

	}
	LRESULT HSimpleWindow::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			//case WM_PAINT:
			//	OnPaint();

		case WM_COMMAND: {
			if ((!lParam) && (!LOWORD(wParam)))
				OnMenu();
			break;
		}

		}

		return HCustomWindow::MessageFunc(message, wParam, lParam);
	}
}
