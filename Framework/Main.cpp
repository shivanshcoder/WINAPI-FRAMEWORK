
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

		//case WM_DESTROY:
			//PostQuitMessage(0);
		}

		return DefWindowProc(Handle(), message, wParam, lParam);
	}

	int z = 2143;

};
class MainWin :public Himani::HBaseApp {
public:
	MainWin(){
		HWND  hwnd = CreateWindow(L"Himani.WinClass.HSimpleWindow", TEXT("Random Rectangles"),
			WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, Himani::Instance(), NULL);

		Himani::HCustomWindow* ptr = (Himani::HCustomWindow*)(SendMessage(hwnd, H_WM_GETOWNINSTANCE, 0, 0));

		tempWin s2;
		//s2.Update();
		//Himani::HSimpleWindow s(TEXT("Hello"));
		//Himani::HSimpleWindow* ptr = dynamic_cast<Himani::HSimpleWindow*>(&s2);
		////*ptr = std::move(s);
		s2.Show(SW_NORMAL);
		//s2.Update();
		
		*(Himani::HCustomWindow*)& s2 = std::move(*ptr);

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

