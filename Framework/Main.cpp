#include"Hpch.h"
#include"Himani.h"
#include"Himani/Log System/Log.h"
#define ROUNDVAL(Val, i)	((Val++)%i)

class MainWin : public Himani::HApplication {
	//LogSystem::WindowLog Logger;
public:
	//CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

	DECLARE_MESSAGE_MAP() {

		return HApplication::MessageFunc(hwnd, message, wParam, lParam);
		switch (message) {
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(Handle(), &ps);
			TextOut(hdc, 50, 50, L"HELLO WORLD", 11);
			EndPaint(Handle(), &ps);

			CheckDefaultWinError;
			return 0;
		}
		}
	}



	MainWin()
		:Himani::HApplication(L"HOLA", WS_OVERLAPPEDWINDOW)
	{
		//LogSystem::QuickTimeLog Q1(Logger);
		CreateWin(L"HEYYE", WS_OVERLAPPEDWINDOW, Helpers::HRect(480 , 720), nullptr);
		//CreateWindowEx(NULL,ClassName().c_str(), L"TITLE", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Himani::Instance(), NULL);
	}

	//int OnSize(WPARAM wParam, LPARAM lParam) {
	int OnPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);
		TextOut(hdc, 50, 50, L"HELLO WORLD", 11);
		EndPaint(Handle(), &ps);
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

//MESSAGE_MAP_BEGIN(MainWin)
//MESSAGE_MAP_ENTRY(OnPaint,WM_PAINT)
//MESSAGE_MAP_END(Himani::HApplication)


ENTRY_APP(MainWin);