#include"Hpch.h"
#include"Himani.h"
#include"Himani/Log System/Log.h"
#define ROUNDVAL(Val, i)	((Val++)%i)

class MainWin : public Himani::HApplication {
	int cxClient;
	int cyClient;

	int idFocus = 0;
	int ClassColor[3] = {};
	Himani::HStaticWindow ColorRect;
	Helpers::HWinArray<Himani::HScrollBar, 3> Scroll;
	Helpers::HWinArray<Himani::HStaticWindow, 3> Label;
	Helpers::HWinArray<Himani::HStaticWindow, 3> Value;
	Helpers::HRect Rectang;
	LogSystem::WindowLog Logger;
public:
	//CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

		DECLARE_MESSAGE_MAP();

	void UpdateColor(int Index, int Val) {
		std::wstringstream ws;
		ws << Val;
		Value[Index].SetWinText(ws.str());
		ClassColor[Index] = Val;
		DeleteObject((HBRUSH)
		SetClassLongPtr(*this, GCLP_HBRBACKGROUND,
			(LONG)CreateSolidBrush(RGB(ClassColor[0], ClassColor[1], ClassColor[2]))));
		CheckDefaultWinError;
		InvalidateRect(Rectang, true);
	}

	MainWin()
		:HApplication(L"Color Scroll", WS_OVERLAPPEDWINDOW),
		ColorRect(Himani::HString(), SS_WHITERECT | WS_VISIBLE | WS_CHILD, Helpers::HRect(), *this),
		Scroll(WS_TABSTOP | SBS_VERT, Helpers::HRect(), *this),
		Label(Himani::HString(), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::HRect(), *this),
		Value(Himani::HString(L"0"), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::HRect(), *this)
	{
		LogSystem::QuickTimeLog Q1(Logger);
		DeleteObject((HBRUSH)
			SetClassLongPtr(*this, GCLP_HBRBACKGROUND,
			(LONG)CreateSolidBrush(RGB(ClassColor[0], ClassColor[1], ClassColor[2]))));
		for (int i = 0; i < Scroll.size(); ++i) {
			Scroll[i].SetInfo(0, 255);
		}
		COLORREF colors[3] = {
			RGB(255, 0, 0),
			RGB(0, 255, 0),
			RGB(0, 0, 255)
		};

		for (int i = 0; i < 3; ++i) {
			LogSystem::QuickTimeLog Q(Logger);
			Label[i].TextColor = colors[i];
			Value[i].TextColor = colors[i];
			Scroll[i].brush = Himani::HBrush(colors[i]);

		}

		Scroll[0].ScrollCallbk = [&](int Val) {
			Logger.Push(1, L"RED");
			UpdateColor(0, Val);
		};

		Scroll[1].ScrollCallbk = [&](int Val) {
			Logger.Push(1, L"GREEN");
			UpdateColor(1, Val);
		};

		Scroll[2].ScrollCallbk = [&](int Val) {
			Logger.Push(1, L"BLUE");
			UpdateColor(2, Val);
		};


		auto str = GetWinText();

	}

	int OnSize(WPARAM wParam, LPARAM lParam) {

		//CheckDefaultWinError;
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		ColorRect.MoveWindow(Helpers::HRect(cxClient / 2, cyClient), true);
		Rectang = Helpers::HRect(cxClient / 2, 0, cxClient, cyClient);

		int cyChar = HIWORD(GetDialogBaseUnits());

		for (int i = 0; i < 3; ++i) {

			Scroll[i].MoveWindow(
				Helpers::HRect{ (2 * i + 1)*cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar },
				true);
			Label[i].MoveWindow(
				Helpers::HRect{ (4 * i + 1) * cxClient / 28, cyChar / 2,
				cxClient / 7, cyChar }, true);
			Value[i].MoveWindow(
				Helpers::HRect{ (4 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2,
				cxClient / 7, cyChar }, true);

			//CheckDefaultWinError;
		}
		SetFocus();
		return 0;
	}

	int OnPaint() {
		//return DefWindowProc(hwn)
	}

	int OnMouseDown(WPARAM wParam, LPARAM lParam) {
		MessageBeep(0);
		return 0;
	}


};
LRESULT MainWin::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_SIZE:
		return OnSize(wParam, lParam);

	case WM_SETFOCUS: {
		Scroll[ROUNDVAL(idFocus, 3)].SetFocus();
		return 0;
	}
	case WM_VSCROLL:
		return SendMessage((HWND)lParam, message, wParam, 0);

	case WM_CTLCOLORSCROLLBAR:
		//		return SendMessage((HWND)lParam, message, wParam, 0);

	case WM_CTLCOLORSTATIC:
		return SendMessage((HWND)lParam, message, wParam, 0);

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return Himani::HCustomApplication::MessageFunc(hwnd, message, wParam, lParam);
}
//MESSAGE_MAP_BEGIN(MainWin)
//MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
//MESSAGE_MAP_ENTRY_SINGLE(MessageBeep(0), 0, WM_LBUTTONDOWN)
////MESSAGE_MAP_ENTRY_SINGLE(SetFocus(GetDlgItem(*this, idFocus)), 0, WM_SETFOCUS)
////MESSAGE_MAP_ENTRY_PARAMS(KeyDown, WM_KEYDOWN)
////MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
//MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
//MESSAGE_MAP_END(Himani::HCustomApplication)

ENTRY_APP(MainWin);