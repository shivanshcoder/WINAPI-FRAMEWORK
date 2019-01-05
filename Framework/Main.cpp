
#include"Header/HIMANI.h"
#include<vector>



class MainWin : public HIMANI::HApplication {
	int cxClient;
	int cyClient;

	HIMANI::HStaticWindow ColorRect;
	Helpers::HWinArray<HIMANI::ScrollBar, 3> Scroll;
	Helpers::HWinArray<HIMANI::HStaticWindow, 3> Label;
	Helpers::HWinArray<HIMANI::HStaticWindow, 3> Value;
public:
	CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

		DECLARE_MESSAGE_MAP();

	MainWin()
		:HApplication(L"Color Scroll", WS_OVERLAPPEDWINDOW),
		ColorRect(std::wstring(L""), SS_WHITERECT | WS_VISIBLE | WS_CHILD, Helpers::HRect(), *this),
		Scroll(WS_TABSTOP | SBS_VERT, Helpers::HRect(), *this),
		Label(std::wstring(), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::HRect(), *this),
		Value(std::wstring(), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::HRect(), *this)
	{
		for (int i = 0; i < Scroll.size(); ++i) {
			Scroll[i].SetInfo(0, 255);
		}

		auto str = GetWinText();

	}


	WPARAM start() {
		//auto t = &HelloWin::start;
		//std::function<int(HelloWin)
		//auto t = std::mem_fn(HelloWin::start);

		return Run();
	}

	int OnSize(WPARAM wParam, LPARAM lParam) {

		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		ColorRect.MoveWindow(Helpers::HRect(cxClient / 2, cyClient), true);

		int cyChar = HIWORD(GetDialogBaseUnits());

		for (int i = 0; i < 3; ++i) {

			Scroll[i].MoveWindow(
				Helpers::HRect{ (2 * i + 1)*cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar },
				true);

			/*Label[i].MoveWindow(
				Helpers::HRect{}
			)*/

		}
		return 0;
	}

	int OnMouseDown(WPARAM wParam, LPARAM lParam) {
		MessageBeep(0);
		return 0;
	}


};

MESSAGE_MAP_BEGIN(MainWin)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY_SINGLE(MessageBeep(0), 0, WM_LBUTTONDOWN)
//MESSAGE_MAP_ENTRY_SINGLE(SetFocus(GetDlgItem(*this, idFocus)), 0, WM_SETFOCUS)
//MESSAGE_MAP_ENTRY_PARAMS(KeyDown, WM_KEYDOWN)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END(HIMANI::HCustomApplication)

ENTRY_APP(MainWin);