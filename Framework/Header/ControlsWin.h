#pragma once
#include"HBaseWin.h"
#include"Messages.h"
#include"GDI.h"
//TODO
//way to work with TABS when when control gets the focus and how to shift it further or back


namespace HIMANI {

	class HControls :public HPredefinedWindow {
	public:
		using HPredefinedWindow::HPredefinedWindow;

		DECLARE_MESSAGE_MAP();

		virtual INT_PTR CTL_Color(WPARAM wParam, LPARAM lParam) = 0;
	};
	MESSAGE_MAP_BEGIN(HControls)
		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORMSGBOX);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLOREDIT);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORLISTBOX);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORBTN);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORDLG);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSCROLLBAR);
	MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	MESSAGE_MAP_END(HPredefinedWindow);

	class HScrollBar :public HControls {

		//Overrides Windows scrollbar class
		DEFINE_CLASSNAME(scrollbar)

	public:
		HScrollBar(int ScrollBarStyle, Helpers::HRect Size, const HBaseWin &Parent) :HControls(Parent) {
			CreateWin(HString(), WS_CHILD | WS_VISIBLE | ScrollBarStyle, Size, (HMENU)this);
		}

		INT_PTR CTL_Color(WPARAM wParam, LPARAM lParam)override {

			return (INT_PTR)brush.RetrieveObject();
		}

		void SetInfo(int Min, int Max, int Page = 0, int Pos = 0, bool Redraw = false) {
			SCROLLINFO sInfo;

			sInfo.cbSize = sizeof(SCROLLINFO);
			sInfo.nMin = Min;
			sInfo.nMax = Max;
			if (Page == 0)
				sInfo.nPage = (Max - Min) / 20;

			sInfo.nPos = Pos;
			sInfo.fMask = SIF_POS | SIF_RANGE | SIF_RANGE | SIF_PAGE;

			SetScrollInfo(&sInfo, SB_CTL, Redraw);
		}

		int Scroll(WPARAM wParam, LPARAM lParam) {
			SCROLLINFO info;
			info.cbSize = sizeof(SCROLLINFO);
			info.fMask = SIF_ALL;
			GetScrollInfo(&info, SB_CTL);
			//	int oldPos = info.nPos;
			switch (LOWORD(wParam)) {
			case SB_LINEUP:
				//case 	SB_LINELEFT:
				info.nPos -= 1;
				break;
			case SB_LINEDOWN:
				//case 	SB_LINERIGHT:
				info.nPos += 1;
				break;
			case SB_BOTTOM:
				//	case 	SB_RIGHT:
				info.nPos = info.nMax;
				break;
			case SB_TOP:
				//	case 	SB_LEFT:
				info.nPos = info.nMin;
				break;
			case SB_PAGEUP:
				//case 	SB_PAGELEFT:
				info.nPos -= info.nPage;
				break;
			case SB_PAGEDOWN:
				//case 	SB_PAGERIGHT:
				info.nPos += info.nPage;
				break;

			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				info.nPos = info.nTrackPos;
				break;
			}

			SetScrollInfo(&info, SB_CTL, true);
			GetScrollInfo(&info, SB_CTL);


			ScrollCallbk(info.nPos);
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


		DECLARE_MESSAGE_MAP();

	public:
		HGDIObject brush;
		std::function<void(int)>ScrollCallbk;
	};

	MESSAGE_MAP_BEGIN(HScrollBar)
		MESSAGE_MAP_ENTRY_PARAMS(Scroll, WM_VSCROLL)
		MESSAGE_MAP_ENTRY_PARAMS(Scroll, WM_HSCROLL)

		MESSAGE_MAP_END(HControls);


	class HStaticWindow :public HPredefinedWindow {
	public:
		OVERRIDE_PREDEFINEDCLASS(static)

			HStaticWindow(const HString &Tittle, int Style, const Helpers::HRect &Size, const HBaseWin &_Parent = HBaseWin()) :HPredefinedWindow{ _Parent }/*,
			BckBrush{RGB(0,0,0) }*/ {
			CreateWin(Tittle, Style, Size, (HMENU)this);
		}

		DECLARE_MESSAGE_MAP();

		LONG_PTR CTL_Color(WPARAM wParam, LPARAM lParam) {
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, TextColor);

			SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));

			return (INT_PTR)GetSysColor(COLOR_BTNHIGHLIGHT);
			/*
			SetTextColor((HDC)wParam, RGB(rand() % 255, rand() % 255, 0));

			CheckDefaultWinError;*/
			//return (LONG_PTR)BckBrush.RetrieveObject();
		}

		HStaticWindow(const HStaticWindow&) = delete;
		HStaticWindow& operator=(const HStaticWindow&) = delete;


		//private:
		COLORREF TextColor = RGB(255, 255, 0);
		//HBrush BckBrush;
	};
	MESSAGE_MAP_BEGIN(HStaticWindow)

		MESSAGE_MAP_ENTRY_PARAMS(CTL_Color, WM_CTLCOLORSTATIC);
	MESSAGE_MAP_END(HPredefinedWindow);

	class Button :public HWindow {
		//overrides Windwos button class
		DEFINE_CLASSNAME(button)

			//	using HControls::HControls;

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
