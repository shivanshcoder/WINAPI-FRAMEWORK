#define INCLUDE_CPPS
#include"Himani.h"
#include"resource.h"

#define CF_TCHAR CF_UNICODETEXT
const TCHAR DefaultText[] = TEXT("Defalt Text - UNICODE Version");
const TCHAR Caption[] = TEXT("Clipboard Text Transfers - Unicode Version");


class MyDialogBox :public Himani::HModalDialog {

public:
	MyDialogBox(Himani::HBaseWin* parent)
		:HModalDialog(parent, IDD_DIALOG1) {
		__debugbreak();
	}

	void OnCommand(int ID)override {
		switch (ID) {
		case IDOK:
		case IDCANCEL:
			EndDialog(TRUE);
		}
	}
};


class MyMenu :public Himani::HMenu {
public:
	MyMenu(Himani::HBaseWin *parent) :HMenu(IDR_MENU1), Parent(parent) {}

	int callback(int ID)override {
		switch (ID) {
		case ID_HELP_ABOUT: {
			MyDialogBox box(Parent);
		}
							return 0;
	}

	}

private:
	Himani::HBaseWin* Parent;
};


class ClipText :public Himani::HCustomApplication {
public:
	//IconDemo():HCustomApplication(L"IconDemo",WS_OVERLAPPEDWINDOW) {}

	ClipText() 
	:Menu(this){
		CreateWin(L"Temp", WS_OVERLAPPEDWINDOW, Helpers::HRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT));
		SetMenu(Menu.menu);
	}

	CLASS_PROPERTIES(ClipText,CS_VREDRAW|CS_HREDRAW,NULL)

	int OnSize(WPARAM wParam, LPARAM lParam) {
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxIcon = GetSystemMetrics(SM_CXICON);
		cyIcon = GetSystemMetrics(SM_CYICON);
		
		return 0;
	}

	int OnPaint() {
		Himani::PaintDC dc(*this);

		HICON Icon = LoadIcon(Himani::Instance(), MAKEINTRESOURCEW(IDI_ICON1));
		for (int x = 0; x < cxClient; x+=cxIcon) {
			for (int y = 0; y < cyClient; y+=cyIcon) {
				DrawIcon(dc, x, y, Icon);
			}
		}
		return 0;
	}

	int OnCommand(WPARAM wParam, LPARAM lParam) {
		//Menu was used
		if (lParam == NULL)
			Menu.callback(LOWORD(wParam));
		//else child sent a message
		return 0;
	}
	DECLARE_MESSAGE_MAP();
private:
	int cxClient, cyClient;
	int cxIcon, cyIcon;
	MyMenu Menu;
};

MESSAGE_MAP_BEGIN(ClipText)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY_PARAMS(OnCommand, WM_COMMAND)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END(Himani::HCustomApplication)

ENTRY_APP(ClipText)

