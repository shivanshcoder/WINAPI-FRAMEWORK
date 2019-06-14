
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Himani/PredefinedClasses.h"
#include"Himani/Himani/SampleWinClasses.h"
#include"Himani/Log System/Log.h"
void DrawRectangle(HWND hwnd)
{
	HBRUSH hBrush;
	HDC    hdc;
	RECT   rect;


	SetRect(&rect, rand() % 480, rand() % 480,
		rand() % 480, rand() % 480);

	hBrush = CreateSolidBrush(
		RGB(rand() % 256, rand() % 256, rand() % 256));

	hdc = GetDC(hwnd);
	FillRect(hdc, &rect, hBrush);
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBrush);
}

class tempWin :public Himani::HSimpleWindow {
public:
	tempWin() :HSimpleWindow(TEXT("tempWin")) {	}

	DECLARE_MESSAGE_MAP() {
		switch (message) {
		case WM_SIZE:
			DrawRectangle(Handle());
			return 0;
		}

		return DefWindowProc(Handle(), message, wParam, lParam);
	}

};

class MainWin :public Himani::HBaseApp {
public:
	MainWin(){
		Himani::HSimpleWindow s(TEXT("Hello"));
		tempWin s2;
		Himani::HSimpleWindow* ptr = &s2;
		//*ptr = std::move(s);
		*(Himani::HSimpleWindow*)& s2 = std::move(s);
		s2.Show(SW_NORMAL);
		s2.Update();
		Run();
		PostQuitMessage(0);
	}
};





ENTRY_APP(MainWin);
//
//class MainWin :public Himani::HBaseApp {
//public:
//	MainWin() {
//		const TCHAR* titles[] = {
//			TEXT("CHILD1"),TEXT("CHILD2"),TEXT("CHILD3"),TEXT("CHILD4")
//		};
//
//		for (int i = 0; i < 4; ++i) {
//			MultiWins[i].CreateWin(this);
//		}
//
//	}
//	int OnSize(WPARAM wParam, LPARAM lParam)override {
//		int cxClient = LOWORD(lParam);
//		int cyClient = HIWORD(lParam);
//		for (int i = 0; i < 4; i++) {
//			MultiWins[i].MoveWindow(
//				Helpers::HRect((i % 2) * cxClient / 2, (i > 1) * cyClient / 2, cxClient / 2, cyClient / 2), TRUE
//			);
//		}
//	}
//private:
//	FirstChild MultiWins[4];
//};

