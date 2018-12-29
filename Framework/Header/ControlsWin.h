#pragma once
#include"BaseWin.hpp"


namespace WINAPIPP {

	class Controls :public PredefinedWindow {
	public:
		Controls(const std::wstring &Text, int Style, Helpers::Rect Size,const BaseWin &Parent):PredefinedWindow(Parent) {
			CreateWin(Text, WS_CHILD | WS_VISIBLE | Style, Size, 0);

		}


	};

	class Button:public Window {
		//overrides Windwos button class
		DEFINE_CLASSNAME(button);

	//	using Controls::Controls;

	public:
		enum ButtonStyle {
			PushButton =		BS_PUSHBUTTON,
			DefPushButton =		BS_DEFPUSHBUTTON,
			CheckBox =			BS_CHECKBOX,
			AutoCheckBox =		BS_AUTOCHECKBOX,
			RadioButton =		BS_RADIOBUTTON,
			State3	=			BS_3STATE,
			AutoState3 =		BS_AUTO3STATE,
			GroupBox =			BS_GROUPBOX,
			AutoRadioButton =	BS_AUTORADIOBUTTON,
			PushBox =			BS_PUSHBOX,
			OwnerDraw =			BS_OWNERDRAW,
			TypeMask =			BS_TYPEMASK,
			LeftText =			BS_LEFTTEXT,

		};


	};
}
