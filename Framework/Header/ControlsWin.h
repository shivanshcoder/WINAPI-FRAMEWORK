#pragma once
#include"HBaseWin.hpp"
#include"Messages.h"
#include"GDI.hpp"
//TODO
//way to work with TABS when when control gets the focus and how to shift it further or back


namespace HIMANI {

	class Controls :public HPredefinedWindow {
	public:
		using HPredefinedWindow::HPredefinedWindow;

		DECLARE_MESSAGE_MAP();

		virtual int CTL_Color(WPARAM wParam, LPARAM lParam) = 0;
	};
	MESSAGE_MAP_BEGIN(Controls)
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORMSGBOX);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLOREDIT);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORLISTBOX);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORBTN);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORDLG);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSCROLLBAR);
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	MESSAGE_MAP_END(HPredefinedWindow)

	class ScrollBar :public Controls {

		//Overrides Windows scrollbar class
		DEFINE_CLASSNAME(scrollbar)


	public:



		ScrollBar(int ScrollBarStyle, Helpers::HRect Size, const HBaseWin &Parent):Controls(Parent) {
			CreateWin(std::wstring(), WS_CHILD | WS_VISIBLE | ScrollBarStyle, Size, 0);
		}

		int CTL_Color(WPARAM wParam, LPARAM lParam)override {

		}

		void SetInfo(int Min, int Max, int Page = 0, int Pos = 0, bool Redraw = false) {
			SCROLLINFO sInfo;
			
			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.nMin = Min;
			sInfo.nMax = Max;
			if (Page == 0)
				sInfo.nPage = (Max - Min) / 10;
			sInfo.nPos = Pos;
			sInfo.fMask = SIF_POS | SIF_RANGE | SIF_RANGE;

			SetScrollInfo(&sInfo, SB_CTL, Redraw);
		}

		int OnVScroll(WPARAM wParam, LPARAM lParam) {
			return 0;
		}
	

		int OnHScroll() {
			return 0;
		}
		//TODO make enums for the styles!!!!
		//enum HorzScrollBarStyle {
		//					SBS_TOPALIGN
		//					SBS_LEFTALIGN
		//					SBS_BOTTOMALIGN
		//					SBS_RIGHTALIGN
		//					SBS_SIZEBOXTOPLEFTALIGN
		//					SBS_SIZEBOXBOTTOMRIGHTALIGN
		//					SBS_SIZEBOX
		//};

		//enum VertScrollBarStyle {

		//};

		DECLARE_MESSAGE_MAP();

	private:
		HIMANI::Pen textPen;
	};

	MESSAGE_MAP_BEGIN(ScrollBar)
		MESSAGE_MAP_ENTRY_PARAMS(OnVScroll, WM_VSCROLL)
		MESSAGE_MAP_ENTRY(OnHScroll, WM_HSCROLL)
		MESSAGE_MAP_END(HPredefinedWindow)


		class Button :public HWindow {
		//overrides Windwos button class
		DEFINE_CLASSNAME(button)

			//	using Controls::Controls;

		public:
			enum ButtonStyle {
				PushButton = BS_PUSHBUTTON,
				DefPushButton = BS_DEFPUSHBUTTON,
				CheckBox = BS_CHECKBOX,
				AutoCheckBox = BS_AUTOCHECKBOX,
				RadioButton = BS_RADIOBUTTON,
				State3 = BS_3STATE,
				AutoState3 = BS_AUTO3STATE,
				GroupBox = BS_GROUPBOX,
				AutoRadioButton = BS_AUTORADIOBUTTON,
				PushBox = BS_PUSHBOX,
				OwnerDraw = BS_OWNERDRAW,
				TypeMask = BS_TYPEMASK,
				LeftText = BS_LEFTTEXT,

			};


	};
}
