#pragma once
#include"Core.h"
#include"HBaseWin.h"
//#include"WinProc.h"

namespace Himani {


	class HBaseDialog;

	class HDialogBoxParams {
	public:
		friend class HBaseDialog; 
		template<class DialogBoxClass, class DialogClassParams>
		friend class ReservedTempDialog;

		HDialogBoxParams(HWindow& parent) :ptr(parent) {}

	private:
		winThunk* thunk = nullptr;
		HWindow& ptr;
		HWND currentInst = nullptr;
	};

	
	class HBaseDialog :public HWindow, private HDialogProc {
		template<class DialogBoxClass, class DialogClassParams>
		friend class ReservedTempDialog;
	public:
		HBaseDialog(HDialogBoxParams &params)
			:Parent(params.ptr),HDialogProc(params.thunk), HWindow(params.currentInst) {
			
		}

		//HBaseDialog(HBaseDialog&& other) :HDialogProc(std::move(other)), Parent(other.Parent) {}

		virtual BOOL MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam);

		virtual bool OnInit() { return true; }
		virtual bool OnPaint() { return false; }
		virtual void OnCommand(int ID) {}

	protected:
		HWindow& Parent;

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

	template<class DialogBoxClass, class DialogClassParams>
	class ReservedTempDialog:public HBaseDialog {
	public:
		ReservedTempDialog(HDialogBoxParams& params):HBaseDialog(params){}


		virtual BOOL MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message) {
			case WM_INITDIALOG: {
				/*HBaseDialog* ptr = new *///DialogBoxClass s(std::move((HBaseDialog*)this));
				HDialogBoxParams* ptr = (HDialogBoxParams*)lParam;
				ptr->thunk = (winThunk*)Procedure();
				ptr->currentInst = _hDlg;
				//FunctionThunk::thunk = new (eHeapAddr)winThunk;

				HBaseDialog *ptr2 = new DialogBoxClass(*(DialogClassParams*)ptr);
				
				SendMessage(_hDlg, message, wParam, lParam);
				return TRUE;
			}

			}
			return HBaseDialog::MessageFunc(_hDlg, message, wParam, lParam);
		}

		DLGPROC proc() {
			return Procedure();
		}
	};

	template<class DialogClass, class DialogClassParams = HDialogBoxParams>
	void CreateDialogBox(LPCWSTR ResourceName, HWindow& parent,const DialogClassParams &Args) {
		ReservedTempDialog<DialogClass, DialogClassParams>* instance = new ReservedTempDialog<DialogClass, DialogClassParams>((HDialogBoxParams&)Args);
		bool EndResult;

		EndResult = DialogBoxParam(Instance(), ResourceName, parent.Handle(), instance->proc(), (LPARAM)&Args);


	}
	/*
	Modal Dialog Box Class
	Simply Create Instance of this Class to get a ModalDialog box and this doesn't return untill the
	Dialog Box processing is complete
	*/
	class HModalDialog :public HBaseDialog {
	public:

		using HBaseDialog::HBaseDialog;

		bool Result() const {
			return EndResult;
		}

	protected:

		//Should be called in Constructor of the Derived class
		void StartDialog() {
			//EndResult = DialogBox(Instance(),
			//	(ResourceID) ? MAKEINTRESOURCE(ResourceID) : ResourceName.c_str(),
			//	Parent.Handle(), Procedure()
			//);
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