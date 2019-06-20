
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Himani/PredefinedClasses.h"
#include"Himani/Himani/SampleWinClasses.h"
#include"Himani/Log System/Log.h"
#include"resource.h"


class ColorDialog :public Himani::HBaseDialog {
public:
	ColorDialog(HWindow& parent) :HBaseDialog(parent, MAKEINTRESOURCE(IDD_DIALOG1)),
		red(*this, IDC_SCROLLBAR1, 255), green(*this, IDC_SCROLLBAR2, 255), blue(*this, IDC_SCROLLBAR3, 255) {
		Show(SW_NORMAL);
	}
	BOOL MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override {
		switch (message) {
		case WM_VSCROLL:
			DeleteObject(
				(HGDIOBJ)SetClassLongPtr(Parent.Handle(), GCLP_HBRBACKGROUND,
				(LONG_PTR)CreateSolidBrush(
					RGB(red.GetVal(), green.GetVal(), blue.GetVal())
				)));
			Parent.InvalidateClient(TRUE);
			return 0;
		}
		return HBaseDialog::MessageFunc(message, wParam, lParam);
	}

private:

	Himani::HScrollBar red, blue, green;
};

class MainWin :public Himani::HSimpleWindow {
public:
	MainWin() :HSimpleWindow(TEXT("Color Scroll"), WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN), dialog(*this) {
		SetClassLongPtr(Handle(), GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(0));
		Show(SW_NORMAL);
	}

	LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override {

		if (WM_DESTROY == message) {
			DeleteObject(
				(HGDIOBJ)SetClassLongPtr(Handle(), GCLP_HBRBACKGROUND,
				(LONG_PTR)GetStockObject(WHITE_BRUSH))
			);
			PostQuitMessage(0);
			return 0;
		}
		return HSimpleWindow::MessageFunc(message, wParam, lParam);
	}
	~MainWin() {
		int c = 23;
	}
private:
	ColorDialog dialog;
};

class MainApp :public Himani::HBaseApp {
public:
	MainApp() {}
	MainWin win;
};
ENTRY_APP(MainApp);
