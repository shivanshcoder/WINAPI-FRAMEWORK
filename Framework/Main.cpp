#include"Hpch.h"
#include"Himani.h"
#include"Himani/Log System/Log.h"
#define ROUNDVAL(Val, i)	((Val++)%i)

class MainWin : public Himani::HApplication {
	LogSystem::WindowLog Logger;
public:
	//CLASS_ALL_PROPERTIES(HelloWin, CS_HREDRAW | CS_VREDRAW, LoadIcon(NULL, IDI_APPLICATION), NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

	//DECLARE_MESSAGE_MAP();



	MainWin()
		:HApplication(L"Color Scroll", WS_OVERLAPPEDWINDOW)
	{
		LogSystem::QuickTimeLog Q1(Logger);

	}

	//int OnSize(WPARAM wParam, LPARAM lParam) {

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

ENTRY_APP(MainWin);