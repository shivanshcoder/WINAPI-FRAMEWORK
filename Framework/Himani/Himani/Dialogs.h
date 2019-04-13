#pragma once
#include"Core.h"
#include"HBaseWin.h"
//#include"WinProc.h"

namespace Himani {

	class HWindow;

	class HBaseDialog :public HWindow, public HDialogProc {
	public:
		HBaseDialog(HWindow& parent, const HString& resourceName)
			:Parent(parent), ResourceName(resourceName) {}

		HBaseDialog(HWindow& parent, int resourceID)
			:Parent(parent), ResourceID(resourceID) {}

		virtual BOOL MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam) override;

		virtual bool OnInit() { return true; }
		virtual bool OnPaint() { return false; }
		virtual void OnCommand(int ID) = 0;

	protected:
		HWindow& Parent;
		int ResourceID;
		HString ResourceName;

		/*Wrapper Functions*/

		void EndDialog(int returnVal) {
			::EndDialog(Handle(), returnVal);
		}

	public:
		void CheckRadioButton(int StartID, int EndID, int CheckItemID) {
			::CheckRadioButton(Handle(), StartID, EndID, CheckItemID);
		}

	//	HWindow GetItem(int ItemID) {
	//		return HWindow(::GetDlgItem(Handle(), ItemID));
	//	}


	};

	/*
	Modal Dialog Box Class
	Simply Create Instance of this Class to get a ModalDialog box and this doesn't return untill the
	Dialog Box processing is complete
	*/
	class HModalDialog :public HBaseDialog {
	public:
		
		using HBaseDialog::HBaseDialog;

		bool Result() const{
			return EndResult;
		}

	protected:

		//Should be called in Constructor of the Derived class
		void StartDialog() {
			EndResult = DialogBox(Instance(),
				(ResourceID) ? MAKEINTRESOURCE(ResourceID) : ResourceName.c_str(),
				Parent.Handle(), Procedure()
			);
		}


	private:
		//True if Ok is pressed
		//False if Cancel is pressed
		bool EndResult;
	};


	/*
	Modless DIalog Box Class
	Saves the data using constructor and simply call Init function to start the Modless DialogBox
	*/
	//class HModlessDialog :public HBaseDialog {
	//public:
	//	using HBaseDialog::HBaseDialog;

	//	//Initializes the Modless Dialog
	//	void StartDialog();
	//};

}