
#include"Header/HIMANI.h"
#include<vector>
class MainWin : public HIMANI::Application {
	int cxBlock, cyBlock;
	
	HIMANI::StaticWindow ColorRect;
	Helpers::WinArray<HIMANI::StaticWindow, 3> Scroll;
	Helpers::WinArray<HIMANI::StaticWindow, 3> Label;
	Helpers::WinArray<HIMANI::StaticWindow, 3> Value;
public:
	CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION),NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

		DECLARE_MESSAGE_MAP();

	MainWin()
		:Application(L"Color Scroll", WS_OVERLAPPEDWINDOW),
		ColorRect(std::wstring(L""), SS_WHITERECT | WS_VISIBLE | WS_CHILD, Helpers::Rect(), *this),
		Scroll(std::wstring(), WS_CHILD | WS_VISIBLE | WS_TABSTOP | SBS_VERT, Helpers::Rect(), *this),
		Label(std::wstring(), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::Rect(), *this),
		Value(std::wstring(), WS_CHILD | WS_VISIBLE | SS_CENTER, Helpers::Rect(), *this)
	{
		
	}


	WPARAM start() {
		//auto t = &HelloWin::start;
		//std::function<int(HelloWin)
		//auto t = std::mem_fn(HelloWin::start);
		
		return Run(); }

	int OnSize(WPARAM wParam, LPARAM lParam) {

		cxBlock = LOWORD(lParam) ;
		cyBlock = HIWORD(lParam) ;

		return 0;
	}

	int OnMouseDown(WPARAM wParam, LPARAM lParam) {
		MessageBeep(0);
		return 0;
	}
	

};

MESSAGE_MAP_BEGIN(MainWin)
//MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY_SINGLE(MessageBeep(0), 0, WM_LBUTTONDOWN)
//MESSAGE_MAP_ENTRY_SINGLE(SetFocus(GetDlgItem(*this, idFocus)), 0, WM_SETFOCUS)
//MESSAGE_MAP_ENTRY_PARAMS(KeyDown, WM_KEYDOWN)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END(HIMANI::CustomApplication)

ENTRY_APP(MainWin);