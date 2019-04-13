#include"Hpch.h"
#include"Himani.h"
#include"Himani/Log System/Log.h"
#define ROUNDVAL(Val, i)	((Val++)%i)

class SampleClass :public Himani::HCustomWindow {
public:
	DECLARE_MESSAGE_MAP();

	WINCLASS_PROPERTIES(SampleClass, CS_VREDRAW | CS_HREDRAW);
	
	int OnPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(Handle(), &ps);
		TextOut(hdc, 50, 50, L"HELLO WORLD", 11);
		TextOut(hdc, 150, 150, L"HELLO@ WORLD", 11);
		TextOut(hdc, 250, 250, L"HELLO# WORLD", 11);
		EndPaint(Handle(), &ps);
		return 0;
	}
};

MESSAGE_MAP_BEGIN(SampleClass)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_END(Himani::HCustomWindow)

class MainWin : public Himani::HApplication {
	//LogSystem::WindowLog Logger;
public:
	
	DECLARE_MESSAGE_MAP();



	MainWin()
		:Himani::HApplication(L"HOLA", WS_OVERLAPPEDWINDOW)
	{
		//LogSystem::QuickTimeLog Q1(Logger);
		//CreateWin(L"HEYYE", WS_OVERLAPPEDWINDOW, Helpers::HRect(480 , 720), nullptr);
		CreateWindowEx(NULL,L"SampleClass", L"SMALLWIN", WS_OVERLAPPEDWINDOW|WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, Himani::Instance(), NULL);
		//SampleClass s;
		//s.CreateWin(TEXT("SMALLWIN"), WS_OVERLAPPEDWINDOW,Helpers::HRect(240,480),NULL);
		//s.Show(SW_NORMAL);
		//s.Update();
		
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

MESSAGE_MAP_BEGIN(MainWin)
MESSAGE_MAP_ENTRY(OnPaint,WM_PAINT)
MESSAGE_MAP_END(Himani::HApplication)


ENTRY_APP(MainWin);