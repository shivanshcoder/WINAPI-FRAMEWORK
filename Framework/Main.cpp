
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Himani/PredefinedClasses.h"
#include"Himani/Himani/SampleWinClasses.h"
#include"Himani/Log System/Log.h"
#include"resource.h"
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
void func() {

}
class tempWin :public Himani::HSimpleWindow {
public:
	tempWin() :HSimpleWindow(TEXT("tempWin"))/*, Button(std::bind(&tempWin::print,this),TEXT("Push this!"),this, Helpers::HRect(10,10,100,100))*/ {

		//Button.CreateWinEx(TEXT("Push this!"), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, this, Helpers::HRect(10,10, 100,100));
		auto rect = Helpers::HRect(100, 100, 100, 100);
		//Button.upda();

		auto l = &tempWin::print;
		//HWND hwndButton = CreateWindowEx(
		//	0,
		//	L"button",  // Predefined class; Unicode assumed 
		//	TEXT("Push this!"),      // Button text 
		//	BS_LEFT|WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		//	rect.left,         // x position 
		//	rect.top,         // y position 
		//	rect.right,        // Button width
		//	rect.bottom,        // Button height
		//	Handle(),     // Parent window
		//	(HMENU)5,       // No menu.
		//	Himani::Instance(),
		//	(LPVOID)-1);      // Pointer not needed.

		//btn = std::make_unique<Himani::Lite::HPushButton<tempWin> >(std::move(Himani::Lite::HPushButton<tempWin> (&tempWin::print this, 5)));

		//btn = std::make_unique<Himani::Lite::HPushButton<tempWin, BS_LEFT> >(&tempWin::print,TEXT("Push Plzz!"), this, rect);
	}

	void print() {
		DrawRectangle(Handle());
	}

	DECLARE_MESSAGE_MAP() {
		//switch (message) {
	//	case WM_SIZE:

			//DrawRectangle(Handle());
		//	return 0;

			//case WM_DESTROY:
				//PostQuitMessage(0);
		//}

		return HSimpleWindow::MessageFunc(message, wParam, lParam);
	}
	
	std::unique_ptr<Himani::Lite::HPushButton<tempWin, BS_LEFT> >btn;
	int z = 2143;

};

class Dial :public Himani::HBaseDialog {
public:

	Dial(Himani::HDialogBoxParams& params) :HBaseDialog(params), btn(std::bind(&Dial::print, this), *this, IDC_BUTTON1),
		grp(*this, { IDC_RADIO1,IDC_RADIO2,IDC_RADIO3,IDC_RADIO4,IDC_RADIO5,IDC_RADIO6 }) {
		HWND a = Handle();
	}
	void print() {
		DrawRectangle(Handle());
	}

	virtual BOOL MessageFunc( UINT message, WPARAM wParam, LPARAM lParam)override{
		if (message == WM_CLOSE) {
			//__debugbreak();
			EndDialog(0);
			return TRUE;
		}
		return FALSE;
	}

private:
	Himani::HPushButton<>btn;
	Himani::HControlGroup<Himani::HRadioButton<>, 6> grp;
};

class MainWin :public Himani::HBaseApp {
public:
	MainWin() {

		tempWin s2;
		s2.Show(SW_NORMAL);
		//s2.Update();
		Himani::CreateDialogBox<Dial>(MAKEINTRESOURCE(IDD_DIALOG1), s2);
		
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

