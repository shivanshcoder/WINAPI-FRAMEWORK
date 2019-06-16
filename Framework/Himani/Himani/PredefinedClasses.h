#pragma once
#include"HBaseWin.h"

/*
Overriden System Registered Window Classes
*/

//TODO add following classes too 
//https://docs.microsoft.com/en-us/windows/desktop/Controls/common-control-window-classes

namespace Himani {
	//class HPredefinedWindow :public HCustomWindow {
	//public:
	//	HPredefinedWindow(Himani::HString Title, DWORD Style, HWindow* Parent, Helpers::HRect Size = Helpers::HRect()) :
	//		HCustomWindow(Himani::HClassInitializer()) {
	//		CreateWin(Title, Style, Parent, Size);
	//	}

	//	HPredefinedWindow(const Himani::HClassInitializer& Data) :
	//		HCustomWindow(Data) {}

	//};

	class HPredefinedWindow: public HCustomWindow {
	public:
		

	protected:
		virtual LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
			return oldProc(Handle(), message, wParam, lParam);
		}

		virtual WNDPROC UpdateProc() {
			return oldProc = HCustomWindow::UpdateProc();
		}

	//private:
		WNDPROC oldProc;
	};

	class HButton :public HPredefinedWindow {
	public:



		enum ButtonStyles {
			Push = BS_PUSHBUTTON,
			DefPush = BS_DEFPUSHBUTTON,
			CheckBox = BS_CHECKBOX,
			AutoCheckBox = BS_AUTOCHECKBOX,
			RadioButton = BS_RADIOBUTTON,
			Manual3State = BS_3STATE,
			Auto3State = BS_AUTO3STATE,
			GroupBox = BS_GROUPBOX,
			User = BS_USERBUTTON,
			AutoRadio = BS_AUTORADIOBUTTON,
			PushBox = BS_PUSHBOX,
			OwnerDraw = BS_OWNERDRAW
		};

	protected:

		PREDEFINED_WINCLASS(TEXT("button"))
	};

	class HPushButton :public HButton {
	public:

		HPushButton(std::function<void()> callback, const HString& Title, HWindow* parent, Helpers::HRect size)
			:callbackOnPush(callback) {
			CreateWinEx(Title,
				WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 0,
				parent, size);	
		}

		HPushButton(std::function<void()> callback, HWindow* parent, int ControlID)
		:callbackOnPush(callback){
			InitHandle(GetDlgItem(parent->Handle(), ControlID));
			UpdateProc();
		}
		
		LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override;

		
	private:
		virtual void OnClick() {
			callbackOnPush();
		}

		std::function<void()>callbackOnPush;
	};

	LRESULT HPushButton::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case H_CM_PROCESSNOTIF:
			if (wParam == BN_CLICKED) {
				OnClick();
			}
		}
		return HButton::MessageFunc(message, wParam, lParam);
	}

	//class HRadioButton :public HButton {

	//};

	//class HCheckButton :public HButton {

	//};

	class HComboBox :public HPredefinedWindow {

	};

	class HEdit:public HPredefinedWindow {

	protected:

		PREDEFINED_WINCLASS(TEXT("edit"))
	};

	class HListBox :public HPredefinedWindow {

	};

	class HMDIClient :public HPredefinedWindow {

	};

	class HRichEdit :public HPredefinedWindow {

	};

	class HRichEdit_2 :public HPredefinedWindow {

	protected:

		//PREDEFINED_WINCLASS(TEXT("static"))
	};

	class HScrollBar :public HPredefinedWindow {

	protected:

		PREDEFINED_WINCLASS(TEXT("scrollbar"))
	};

	class HStatic :public HPredefinedWindow {

	protected:

		PREDEFINED_WINCLASS(TEXT("static"))

	};


}