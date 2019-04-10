#define INCLUDE_CPPS
#include"Himani.h"
#include"resource.h"

class MyDialogBox :public Himani::HModalDialog {

public:
	MyDialogBox(Himani::HWindow& parent)
		:HModalDialog(parent, IDD_DIALOG2){
			StartDialog();
	}

	void OnCommand(int ID)override {
		//if (ID == IDOK)
			EndDialog(0);
	}
};

class EllipPush :public Himani::HCustomWindow {
public:
	CLASS_ALL_PROPERTIES(EllipPush, CS_VREDRAW | CS_HREDRAW, NULL, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE + 1), NULL)
	DECLARE_MESSAGE_MAP();

	int OnPaint() {
		auto Rect = GetClientRect();

		//TODO GetWindowText
		auto WinText = GetWinText();
		Himani::PaintDC dc(*this);

		dc.Attach(Himani::HBrush(GetSysColor(COLOR_WINDOW)));

		dc.SetBkColor(GetSysColor(COLOR_WINDOW));
		dc.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

		Ellipse(dc, Rect.left, Rect.top, Rect.right, Rect.bottom);
		DrawText(dc, WinText.c_str(), -1, &Rect.rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		return 0;
	}
	
	int OnKeyUp(WPARAM wParam, LPARAM lParam) {
		if (wParam == VK_SPACE)
			return OnLButtonUp(wParam, lParam);
		return 0;
	}

	int OnLButtonUp(WPARAM wParam, LPARAM lParam) {
		SendMessage(Parent().Handle(), WM_COMMAND, GetWindowLongPtr(Handle(), GWLP_ID), (LPARAM)Handle());
		return 0;
	}
};

MESSAGE_MAP_BEGIN(EllipPush)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY_PARAMS(OnKeyUp,WM_KEYUP)
MESSAGE_MAP_ENTRY_PARAMS(OnLButtonUp, WM_LBUTTONUP)
MESSAGE_MAP_END(HCustomWindow)

class ClipText :public Himani::HCustomApplication {
public:
	int iCurrentColor = IDC_BLUE;
	int iCurrentFigure = IDC_ELLIPSE;


	ClipText() {
		CreateWin(L"Temp", WS_OVERLAPPEDWINDOW, Helpers::HRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT), LoadMenu(Himani::Instance(), MAKEINTRESOURCE(IDR_MENU1)));
	}

	CLASS_PROPERTIES(ClipText, CS_VREDRAW | CS_HREDRAW, NULL)

		int OnSize(WPARAM wParam, LPARAM lParam) {
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxIcon = GetSystemMetrics(SM_CXICON);
		cyIcon = GetSystemMetrics(SM_CYICON);

		return 0;
	}

	int OnPaint() {
		{
			Himani::PaintDC dc(*this);
		}
		return 0;
	}

	int OnCommand(WPARAM wParam, LPARAM lParam) {
		switch (LOWORD(wParam)) {
		case ID_HELP_ABOUT: {
			MyDialogBox Box(*this);
			
			InvalidateClient(true);
		}
		}
		return 0;
	}

	int OnDes() {
		PostQuitMessage(0);
		SetWindowLongPtr(Handle(), GWLP_WNDPROC, (LONG_PTR)(Himani::StaticWndProc));
		return 0;
	}
	DECLARE_MESSAGE_MAP();
private:
	int cxClient, cyClient;
	int cxIcon, cyIcon;
};

MESSAGE_MAP_BEGIN(ClipText)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY_PARAMS(OnCommand, WM_COMMAND)
MESSAGE_MAP_ENTRY(OnDes, WM_DESTROY)
MESSAGE_MAP_END(Himani::HCustomApplication)

ENTRY_APP(ClipText)

