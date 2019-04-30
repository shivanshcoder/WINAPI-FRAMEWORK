
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Log System/Log.h"

int a = 0;
int b = 0;
class FirstChild :public Himani::HSimpleWindow {
public:
	void CreateWin(HWindow* parent, int* val, int time) {
		ptr = val;
		Himani::HSimpleWindow::CreateWin(TEXT(""), WS_OVERLAPPEDWINDOW | WS_VISIBLE, parent, Helpers::HRect(600, 600));
		SetTimer(Handle(), 1, time, NULL);
	}

	DECLARE_MESSAGE_MAP() {
		std::wstringstream ws;
		ws << *ptr;
		switch (message) {

		case WM_TIMER:
		case WM_LBUTTONDOWN: {
			//(*ptr)++;
			::InvalidateRect(Handle(), NULL, true);
			return 0;
		}


		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(Handle(), &ps);
			TextOut(hdc, 50, 50, ws.str().c_str(), ws.str().size());
			EndPaint(Handle(), &ps);
			return 0;
		}
		}
		return HSimpleWindow::MessageFunc(message, wParam, lParam);
	}
	int* ptr;
};
class MainWin :public Himani::HBaseApp {
public:
	MainWin() :s(&b) {
		fs.CreateWin(nullptr, s, 10000);
	}
	void Idle()override {
		MainWin* Hey = Himani::HThread<MainWin>::GetApp();
		(*Hey->s)++;
	}
	int* s;
	const int z = 23123;
	FirstChild fs;
};
class MainWins :public Himani::HBaseApp {
public:
	MainWins() :
		s(&a) {
		thread2.StartThread();
		fs.CreateWin(nullptr, s, 10);
	}
	void Idle()override {
		MainWin* Hey = Himani::HThread<MainWin>::GetApp();
		(*Hey->s)++;
	}
	int* s;
	const int z = 2;
	FirstChild fs;
	Himani::HThread<MainWin>thread2;

};



ENTRY_APP(MainWins);
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

