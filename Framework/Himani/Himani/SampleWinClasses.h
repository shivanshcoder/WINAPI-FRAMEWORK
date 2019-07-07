#pragma once
#include"HBaseWin.h"

namespace Himani {


	class HSimpleWindow :public HCustomWindow {
	public:

		//HSimpleWindow(const Himani::HClassInitializer& Data) :HCustomWindow(Data) {}

		HSimpleWindow(const HString& Title, DWORD style = WS_OVERLAPPEDWINDOW, HWindow* parent = nullptr, Helpers::HRect size = Helpers::HRect(480, 480)) {
			CreateWinEx(Title, style, 0, parent, size);
		}

		virtual void OnMenu();
		DECLARE_MESSAGE_MAP();
	protected:
		virtual int OnPaint() { return 1; }

		WINCLASS_PROPERTIES(HSimpleWindow, CS_VREDRAW | CS_HREDRAW)
	};

	//MESSAGE_MAP_BEGIN(HSimpleWindow)
	//	MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
	//	MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
	//	MESSAGE_MAP_END(HCustomWindow)


	class HSSimple :public HSimpleWindow {
	public:
		HSSimple() :
			HSimpleWindow(L"HELLLO", WS_OVERLAPPEDWINDOW, nullptr, Helpers::HRect(960, 960)) {}
	};
}
