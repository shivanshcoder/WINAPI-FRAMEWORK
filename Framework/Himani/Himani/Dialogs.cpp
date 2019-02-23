#include "Hpch.h"
#include "Dialogs.h"

namespace Himani {

	BOOL HBaseDialog::MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_INITDIALOG: {
			hDlg = _hDlg;
		}break;

		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;

		default:
			return FALSE;
		}

		return TRUE;
	}
	
		
	HModalDialog::HModalDialog(HBaseWin* parent, const HString& resourceName)
		:HBaseDialog(parent, resourceName) {

		DialogBox(Himani::Instance(), resourceName.c_str(), (HWND)* parent, Procedure());
	}
	
	HModalDialog::HModalDialog(HBaseWin* parent, int resourceID)
		: HBaseDialog(parent, resourceID) {
		DialogBox(Himani::Instance(), MAKEINTRESOURCE(resourceID), (HWND)* parent, Procedure());
	}
}