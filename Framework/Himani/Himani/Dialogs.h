#pragma once
#include"Core.h"
#include"HBaseWin.h"
//#include"WinProc.h"

namespace Himani {


	class HBaseDialog;

	//Dialog Initialization helper class
	//Every DialogBox class needs to have constructor taking object of this or its derived class
	class HDialogBoxParams {
	public:
		friend class HBaseDialog;
		template<class DialogBoxClass, class DialogClassParams>
		friend class ReservedTempDialog;

		HDialogBoxParams(HWindow& parent) :ptr(parent) {}

	private:
		HWindow& ptr;
		//Only Parent needs to be filled
		//rest variables will be filled automatically 
		winThunk* thunk = nullptr;
		HWND currentInst = nullptr;
	};


	class HBaseDialog :public HWindow, private HDialogProc {
		template<class DialogBoxClass, class DialogClassParams>
		friend class ReservedTempDialog;
	public:
		//To be used when the derived class is a Modal Dialog Box
		HBaseDialog(HDialogBoxParams& params)
			//Initializes the Handle,parent, and replaces the thunk with the supplied thunk
			:Parent(params.ptr), HDialogProc(params.thunk), HWindow(params.currentInst){}

		HBaseDialog(HWindow& parent, PTSTR ResourceName) :Parent(parent) {
			HWND temp = CreateDialog(Instance(), ResourceName, parent.Handle(), Procedure());
			if (temp == nullptr)
				throw Exceptions(TEXT("Failed to Create DialogBox!!!!!"));

			GetApp()->DlgStore.push_back(Handle());
		}

		virtual BOOL MessageFunc(UINT message, WPARAM wParam, LPARAM lParam);

		//TODO make this final type, so that no overriding is allowed
		virtual BOOL __MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam)override {
			//Some Forced functionality to be added!
			switch (message) {

			case WM_INITDIALOG:
				if (((HWND)* this) == nullptr) {
					
					InitHandle(_hDlg);
					return 0;
				}

			case WM_VSCROLL:
			case WM_HSCROLL:
			case WM_COMMAND: {
				if (lParam) {
					//Framework Controls can automatically use the notification by themselves
					int temp = SendMessage((HWND)lParam, H_CM_PROCESSNOTIF, HIWORD(wParam), LOWORD(wParam));

					//TODO change the return value checking !!
					if (temp != -1)
						break;
					return TRUE;
				}
			}


			}
			return MessageFunc(message, wParam, lParam);
		}

		virtual bool OnInit() { return true; }
		virtual bool OnPaint() { return false; }
		virtual void OnCommand(int ID) {}


	protected:
		HWindow& Parent;

		/*Wrapper Functions*/

		void EndDialog(int returnVal) {
			HBaseApp* ptr = GetApp();
			auto c = std::find(ptr->DlgStore.begin(), ptr->DlgStore.end(), (HWND)* this);
			if (c == ptr->DlgStore.end()) {
				::EndDialog(Handle(), returnVal);
				InitHandle(nullptr);
			}
			else {
				ptr->DlgStore.erase(c);
				DestroyWindow((HWND)* this);
				InitHandle(nullptr);

			}
		}

	public:
		void CheckRadioButton(int StartID, int EndID, int CheckItemID) {
			::CheckRadioButton(Handle(), StartID, EndID, CheckItemID);
		}

		~HBaseDialog() {
			if ((HWND)* this) {
				EndDialog(0);
				////std::remove(GetApp()->DlgStore.begin(), GetApp()->DlgStore.end(), (HWND)* this);
				//auto c = std::find(GetApp()->DlgStore.begin(), GetApp()->DlgStore.end(), (HWND)* this);
				//GetApp()->DlgStore.erase(c);
				////In case the object is getting out of scope Destroy Window forcefully!!
				//DestroyWindow((HWND)* this);
			}
		}

	};

	template<class DialogBoxClass, class DialogClassParams>
	class ReservedTempDialog :public HBaseDialog {
	public:
		ReservedTempDialog(HDialogBoxParams& params) :HBaseDialog(params) {}


		virtual BOOL __MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message) {
			case WM_INITDIALOG: {
				/*HBaseDialog* ptr = new *///DialogBoxClass s(std::move((HBaseDialog*)this));
				HDialogBoxParams* ptr = (HDialogBoxParams*)lParam;
				ptr->thunk = leaveThunk();
				ptr->currentInst = _hDlg;
				//FunctionThunk::thunk = new (eHeapAddr)winThunk;

				DialogBoxClass* tempPTR = new DialogBoxClass(*(DialogClassParams*)ptr);
				newDialog = tempPTR;
				auto c = typeid(this).raw_name();
				auto ss = typeid(tempPTR).raw_name();

				SendMessage(_hDlg, message, wParam, lParam);
				return TRUE;
			}

			}
			return HBaseDialog::__MessageFunc(_hDlg, message, wParam, lParam);
		}

		DLGPROC proc() {
			return Procedure();
		}
		~ReservedTempDialog() {
			delete newDialog;
		}
	private:
		HBaseDialog* newDialog = nullptr;
	};

	//Doesn't return till the dialog box is closed or successfully returns
	template<class DialogClass = HBaseDialog, class DialogClassParams = HDialogBoxParams>
	void CreateDialogBox(PTSTR ResourceName, HWindow & parent, DialogClassParams * Args = nullptr) {
		if (!Args) {
			Args = new HDialogBoxParams(parent);
		}
		ReservedTempDialog<DialogClass, DialogClassParams>* instance = new ReservedTempDialog<DialogClass, DialogClassParams>(*(HDialogBoxParams*)Args);
		bool EndResult;

		EndResult = DialogBoxParam(Instance(), ResourceName, parent.Handle(), instance->proc(), (LPARAM)Args);

		//It is safe to assume everything is fine here right?
		delete instance;

	}


}