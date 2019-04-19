
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Log System/Log.h"
#include"resource.h"
#define ROUNDVAL(Val, i)	((Val++)%i)

class EllipPush :public Himani::HCustomWindow {
public:
	EllipPush(const Himani::HClassInitializer& Args) :Himani::HCustomWindow(Args) {}
	DECLARE_MESSAGE_MAP();

	WINCLASS_PROPERTIES(EllipPush, CS_VREDRAW | CS_HREDRAW);
	int OnButtonUp(WPARAM wParam, LPARAM lParam) {

		SendMessage(GetParent(Handle()), WM_COMMAND,
			ID_OK, (LPARAM)Handle());
		return 0;

	}
	int OnPaint() {
		SetClassLongPtr(Handle(), GCLP_HBRBACKGROUND, (LONG_PTR)(COLOR_BTNFACE + 1));
		SetWindowText(Handle(), L"OK");
		auto rect = GetClientRect();
		auto Text = GetWinText();
		PAINTSTRUCT ps;

		HDC dc = BeginPaint(Handle(), &ps);
		HBRUSH  hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		hBrush = (HBRUSH)SelectObject(dc, hBrush);

		SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));

		Ellipse(dc, rect.left, rect.top, rect.right, rect.bottom);
		DrawText(dc, Text.c_str(), -1, &rect.rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		DeleteObject(SelectObject(dc, hBrush));


		return 0;
	}
};

MESSAGE_MAP_BEGIN(EllipPush)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY_PARAMS(OnButtonUp, WM_LBUTTONUP)
MESSAGE_MAP_END(Himani::HCustomWindow)

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_OK:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

class MainWin : public Himani::HApplication {
	//LogSystem::WindowLog Logger;
public:

	DECLARE_MESSAGE_MAP() {
		switch (message)
		{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{

			case ID_APP_ABOUT:
				auto LL = DialogBox(Himani::Instance(), MAKEINTRESOURCE(IDD_DIALOG1), Handle(), (DLGPROC)AboutDlgProc);
				CheckDefaultWinError;
				return 0;
			}
			break;


			//case WM_DESTROY:
			//	PostQuitMessage(0);
			//	return 0;
		}
		return Himani::HApplication::MessageFunc(message, wParam, lParam);
	}



	MainWin()
		:Himani::HApplication(L"HOLA", WS_OVERLAPPEDWINDOW) {
		auto z = ::SetMenu(Handle(), LoadMenu(Himani::Instance(), MAKEINTRESOURCE(IDR_MENU1)));
		CheckDefaultWinError;
		//LogSystem::QuickTimeLog Q1(Logger);
		//CreateWin(L"HEYYE", WS_OVERLAPPEDWINDOW, Helpers::HRect(480 , 720), nullptr);
		//SampleClass s;
		//s.CreateWin(TEXT("SMALLWIN"), WS_OVERLAPPEDWINDOW,Helpers::HRect(240,480),NULL);
		//s.Show(SW_NORMAL);
		//s.Update();
		//CreateWindowEx(NULL, L"EllipPush", L"LOL", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, Handle(), NULL, Himani::Instance(), NULL);

	}

	//int OnSize(WPARAM wParam, LPARAM lParam) {
	int OnPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);
		TextOut(hdc, 50, 50, L"HELLO WORLD", 11);
		EndPaint(Handle(), &ps);
		return 0;
	}
	//	//CheckDefaultWinError;
	//	cxClient = LOWORD(lParam);
	//	cyClient = HIWORD(lParam);

	//	ColorRect.MoveWindow(Helpers::HRect(cxClient / 2, cyClient), true);
	//	Rectang = Helpers::HRect(cxClient / 2, 0, cxClient, cyClient);

	//	int cyChar = HIWORD(GetDialogBaseUnits());

	//	for (int i = 0; i < 3; ++i) {

	//		Scroll[i].MoveWindow(
	//			Helpers::HRect{ (2 * i + 1)*cxClient / 14, 2 * cyChar, cxClient / 14, cyClient - 4 * cyChar },
	//			true);
	//		Label[i].MoveWindow(
	//			Helpers::HRect{ (4 * i + 1) * cxClient / 28, cyChar / 2,
	//			cxClient / 7, cyChar }, true);
	//		Value[i].MoveWindow(
	//			Helpers::HRect{ (4 * i + 1) * cxClient / 28, cyClient - 3 * cyChar / 2,
	//			cxClient / 7, cyChar }, true);

	//		//CheckDefaultWinError;
	//	}
	//	SetFocus();
	//	return 0;
	//}

	//int OnPaint() {
	//	//return DefWindowProc(hwn)
	//}

	//int OnMouseDown(WPARAM wParam, LPARAM lParam) {
	//	MessageBeep(0);
	//	return 0;
	//}


};
//
//MESSAGE_MAP_BEGIN(MainWin)
//MESSAGE_MAP_ENTRY(OnPaint,WM_PAINT)
//MESSAGE_MAP_END(Himani::HApplication)

//int WINAPI Main() {
//	HWND hwnd = CreateWindow(L"Himani.WinClass.EllipPush", L"TEST", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Himani::Instance(), NULL);
//	MSG msg;
//	CheckDefaultWinError;
//	while (GetMessage(&msg, NULL, NULL, NULL)) {
//
//		if (IsDialogMessage(hwnd, &msg)) {
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//	}
//	return 0;
//}

ENTRY_APP(MainWin);