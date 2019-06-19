#pragma once
#include"HBaseWin.h"

/*
Overriden System Registered Window Classes
*/

//TODO add following classes too 
//https://docs.microsoft.com/en-us/windows/desktop/Controls/common-control-window-classes

namespace Himani {



	class HPredefinedWindow : public HCustomWindow {
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


	protected:
		LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override;

		virtual void OnClick() {}

		PREDEFINED_WINCLASS(TEXT("button"))
	};


	LRESULT HButton::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case H_CM_PROCESSNOTIF:
			if (wParam == BN_CLICKED) {
				OnClick();
				return -1;
			}
		}
		return HPredefinedWindow::MessageFunc(message, wParam, lParam);
	}


	namespace Lite {

		template<class parentClass, int ButtonStyles = 0>
		class HPushButton :public HButton {
		public:

			HPushButton(void (parentClass::* callback)(), const HString& Title, parentClass* parent, Helpers::HRect size)
				:ParentMember(callback) {
				parentInstance = parent;
				CreateWinEx(Title,
					ButtonStyles | WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON, 0,
					parent, size);
			}

			HPushButton(void (parentClass::* callback)(), parentClass* parent, int ControlID)
				:ParentMember(callback) {
				parentInstance = parent;
				InitHandle(GetDlgItem(parent->Handle(), ControlID));
				UpdateProc();
			}

		private:
			virtual void OnClick()override {
				(parentInstance->*ParentMember)();
			}

			//Stores the pointer to member function to the parent class which will be called on getting clicked
			void (parentClass::* ParentMember)();


			parentClass* parentInstance;
		};
	}

	template<int ButtonStyles = 0>
	class HPushButton :public HButton {
	public:

		HPushButton(std::function<void()> callback, const HString& Title, HWindow& parent, Helpers::HRect size)
			:callbackOnPush(callback) {
			CreateWinEx(Title,
				WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_PUSHBUTTON | ButtonStyles, 0,
				&parent, size);
		}

		HPushButton(std::function<void()> callback, HWindow& parent, int ControlID)
			:callbackOnPush(callback) {
			InitHandle(GetDlgItem(parent.Handle(), ControlID));
			UpdateProc();
		}



	private:
		virtual void OnClick()override {
			callbackOnPush();
		}

		std::function<void()>callbackOnPush;
	};
/*
	template<int ButtonStyles>
	class HRadioButton;*/

	//template<int size, DWORD RadioButtonStyle>
	//class HRadioGroup {
	//public:
	//	HRadioGroup(HWindow& parent, std::initializer_list<HString>texts) {

	//	}

	//	HRadioGroup(HWindow& parent, std::initializer_list<int>buttonIDs) {
	//		//TODO later use iterators!
	//		int i = 0;
	//		for (auto& ID : buttonIDs) {
	//			buttons[i++] = std::make_unique < HRadioButton<RadioButtonStyle> >(parent, ID);
	//		}
	//	}

	//private:
	//	std::array< std::unique_ptr< HRadioButton<RadioButtonStyle> >, size > buttons;
	//};
	template<class ControlName, int size>
	class HControlGroup;

	template<class ControlName, int size>
	class HGroupableControl : public ControlName {
		template<class ControlName, int size>
		friend class HControlGroup;
	public:
		//using ControlName::ControlName;

		template <class... _Types>
		HGroupableControl(HControlGroup<ControlName, size>& groupOwner, _Types&& ... _Args) :
			GroupOwner(groupOwner), ControlName(std::forward<_Types>(_Args)...) {

		}

		LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override {
			if (H_CM_PROCESSNOTIF == message) {
				GroupOwner.Notify(lParam);
			}
			return ControlName::MessageFunc(message, wParam, lParam);

		}

		//WARNING is destructor needed?
		//~HGroupableControl() 
		HControlGroup<ControlName,size>& GroupOwner;
	};



	template<class ControlName, int size>
	class HControlGroup  {
		//	using HGrouBpableControl<ControlName> ControlGroupable;

	public:
		HControlGroup(HWindow& parent, std::initializer_list<DWORD>controlIDs) {
			int i = 0;
			for (auto& ID : controlIDs) {
				controls[i++] = std::make_unique < HGroupableControl<ControlName, size> >(*this, parent, ID);

			}
		}
		HControlGroup(HWindow& parent, DWORD startID, DWORD endID) {
			int i = 0;
			while(startID <= endID) {
				controls[i++] = std::make_unique < HGroupableControl<ControlName, size> >(*this, parent, startID++);

			}
		}

		void Notify(int ID) {
			//__debugbreak();
		}
		
	private:
		std::array< std::unique_ptr< HGroupableControl<ControlName, size> >, size > controls;
	};


	template<int ButtonStyles = 0>
	class HRadioButton :public HButton {
	public:
	/*	HRadioButton(const HString& Title, HWindow& parent, Helpers::HRect size) {
			CreateWinEx(Title,
				WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_AUTORADIOBUTTON | ButtonStyles, 0,
				&parent, size);
		}*/

		HRadioButton(HWindow& parent, int ControlID) {
			InitHandle(GetDlgItem(parent.Handle(), ControlID));
			UpdateProc();
		}
	};


	/*
	class HWindowGroup:public HCustomWindow{
	public:
		HWindowGroup(const HString& Title, HWindow* ptr, Helpers::HRect size) {
			CreateWinEx(Title, WS_CHILD | WS_TABSTOP | WS_CLIPCHILDREN, 0, ptr, size);
		}

		LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override;
	protected:
		int ChildrenID = 1;
		WINCLASS_PROPERTIES(HWindowGroup, CS_VREDRAW | CS_HREDRAW)
	};

	LRESULT HWindowGroup::MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {

		case H_WM_GETCHILDID:
			return ChildrenID++;
		}

		return HCustomWindow::MessageFunc(message, wParam, lParam);
	}*/
	//class HCheckButton :public HButton {

	//};

	class HComboBox :public HPredefinedWindow {

	};

	class HEdit :public HPredefinedWindow {

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