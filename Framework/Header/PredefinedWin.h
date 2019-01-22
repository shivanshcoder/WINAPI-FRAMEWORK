#pragma once
#include"HBaseWin.h"
#include"Messages.h"
#include"GDI.h"
namespace HIMANI {
	//class HStaticWindow :public HPredefinedWindow {
	//public:
	//	OVERRIDE_PREDEFINEDCLASS(static)

	//		HStaticWindow(const HString &Tittle, int Style, const Helpers::HRect &Size, const HBaseWin &_Parent = HBaseWin()) :HPredefinedWindow{ _Parent }/*,
	//		BckBrush{RGB(0,0,0) }*/ {
	//		CreateWin(Tittle, Style, Size,(HMENU) this);
	//	}

	//	DECLARE_MESSAGE_MAP();

	//	LONG_PTR CTL_Color(WPARAM wParam, LPARAM lParam) {
	//		HDC hdcStatic = (HDC)wParam;
	//		SetTextColor(hdcStatic, TextColor);
	//		
	//		SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
	//		return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
	//		/*
	//		SetTextColor((HDC)wParam, RGB(rand() % 255, rand() % 255, 0));

	//		CheckDefaultWinError;*/
	//		//return (LONG_PTR)BckBrush.RetrieveObject();
	//	}

	//	HStaticWindow(const HStaticWindow&) = delete;
	//	HStaticWindow& operator=(const HStaticWindow&) = delete;


	////private:
	//	COLORREF TextColor = RGB(255,255,255);
	//	//HBrush BckBrush;
	//};
	//MESSAGE_MAP_BEGIN(HStaticWindow)
	//	
	//	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	//MESSAGE_MAP_END(HPredefinedWindow);
}