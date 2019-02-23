#pragma once
#include"Core.h"
#include"HBaseWin.h"
//#include"WinProc.h"

namespace Himani {

	class HBaseWin;

	class HBaseDialog :public HDialogProc{
	public:
		HBaseDialog(HBaseWin* parent, const HString& resourceName)
			:Parent(parent), ResourceName(resourceName) {}

		HBaseDialog(HBaseWin* parent, int resourceID)
			:Parent(parent), ResourceID(resourceID) {}

		virtual BOOL MessageFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) override;

		virtual void OnCommand(int ID) = 0;

	protected:
		HBaseWin *Parent;
		int ResourceID;
		HString ResourceName;

		/*Wrapper Functions*/

		void EndDialog(int returnVal) {
			::EndDialog(hDlg, returnVal);
		}

	public:
		void CheckRadioButton(int StartID, int EndID, int CheckItemID) {
			::CheckRadioButton(hDlg, StartID, EndID, CheckItemID);
		}

	private:
		HWND hDlg;
	};

	/*
	Modal Dialog Box Class
	Simply Create Instance of this Class to get a ModalDialog box and this doesn't return untill the
	Dialog Box processing is complete
	*/
	class HModalDialog :public HBaseDialog{
	public:
		//Constructor Initializes the ModalDialog

		using HBaseDialog::HBaseDialog;

	protected:
		void InitDialog() {
			if(ResourceID)
				DialogBox(Instance(), MAKEINTRESOURCE(ResourceID),*Parent, 
		}
	};


	/*
	Modless DIalog Box Class
	Saves the data using constructor and simply call Init function to start the Modless DialogBox
	*/
	class HModlessDialog :public HBaseDialog{
	public:
		using HBaseDialog::HBaseDialog;

		//Initializes the Modless Dialog
		void Init();
	};

}