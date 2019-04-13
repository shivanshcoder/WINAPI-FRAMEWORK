#pragma once
#include"HBaseWin.h"
#include"Messages.h"
#include"GDI.h"
//TODO
//way to work with TABS when when control gets the focus and how to shift it further or back

namespace Himani {
	//class HGDIObject;

	class HControls :public HPredefinedWindow {
	public:
		using HPredefinedWindow::HPredefinedWindow;

		DECLARE_MESSAGE_MAP();

		virtual INT_PTR CTL_Color(WPARAM wParam, LPARAM lParam) = 0;

		//TODO make a enum for types of derived classes? and make it pure virtual
		virtual int ControlType()const { return 0; }
	};
	
	class HScrollBar :public HControls {

		//Overrides Windows scrollbar class
		//DEFINE_CLASSNAME(scrollbar)

	public:
		//URGENT fix LATER!!!!
		//HScrollBar(int ScrollBarStyle, Helpers::HRect Size, const HWindow &Parent) :HControls(Parent) {
			//CreateWin(HString(), WS_CHILD | WS_VISIBLE | ScrollBarStyle, Size, (HMENU)this);
		//}

		INT_PTR CTL_Color(WPARAM wParam, LPARAM lParam)override;

		void SetInfo(int Min, int Max, int Page = 0, int Pos = 0, bool Redraw = false);

		int Scroll(WPARAM wParam, LPARAM lParam);

		

		DECLARE_MESSAGE_MAP();

	public:
		HGDIObject brush;
		std::function<void(int)>ScrollCallbk;
	};

	

	class HStaticWindow :public HPredefinedWindow {
	public:
		//OVERRIDE_PREDEFINEDCLASS(static)
			//TODO HMENU arguement Removed 
			HStaticWindow(const HString &Title, int Style, const Helpers::HRect &Size/*, const HWindow &_Parent = HWindow()*/) /*:HPredefinedWindow{ _Parent } */{
		//	CreateWin(Title, Style, Size);
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

	class Button :public HCustomWindow {
		//overrides Windwos button class
		//DEFINE_CLASSNAME(button)

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
