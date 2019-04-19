#pragma once
#include"HBaseWin.h"
#include"Messages.h"
#include"GDI.h"

namespace Himani {
	//class HGDIObject;
	/*
	class HControls :public HCustomWindow {
	public:
		using HCustomWindow::HCustomWindow;

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
			HStaticWindow(const HString &Title, int Style, const Helpers::HRect &Size/*, const HWindow &_Parent = HWindow()) :HPredefinedWindow{ _Parent } {
		//	CreateWin(Title, Style, Size);
		}

		DECLARE_MESSAGE_MAP();

		LONG_PTR CTL_Color(WPARAM wParam, LPARAM lParam) {
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, TextColor);

			SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));

			return (INT_PTR)GetSysColor(COLOR_BTNHIGHLIGHT);
			
			SetTextColor((HDC)wParam, RGB(rand() % 255, rand() % 255, 0));

			CheckDefaultWinError;
			//return (LONG_PTR)BckBrush.RetrieveObject();
		}

		HStaticWindow(const HStaticWindow&) = delete;
		HStaticWindow& operator=(const HStaticWindow&) = delete;


		//private:
		COLORREF TextColor = RGB(255, 255, 0);
		//HBrush BckBrush;
	};
	*/

	/*
		SuperClassing the Windows standard button Class!
	*/
	class HButton :public HCustomWindow {

	public:
		HButton(const Himani::HClassInitializer& Args = HClassInitializer()) :HCustomWindow(Args) {	
			if (OldBtnProc) {
				WNDCLASSEX wndclass;
				GetClassInfoEx(NULL, TEXT("button"), &wndclass);
				OldBtnProc = wndclass.lpfnWndProc;
			}
		}

		/*virtual void CreateControl(const HString& Text, enum ButtonStyle style, Helpers::HRect size, HWindow* parent) {
			CreateWin(Text, WS_CHILD | WS_VISIBLE | style, size, parent);
		}*/

		virtual LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override;

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
		

	private:
		WINCLASS_PROPERTIES(HButton, CS_HREDRAW | CS_VREDRAW);
		static inline WNDPROC OldBtnProc = nullptr;
	};

	class HPushButton :HButton {

	};
}
