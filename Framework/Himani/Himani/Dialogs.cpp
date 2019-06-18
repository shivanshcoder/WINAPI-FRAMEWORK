#include "Hpch.h"
#include "Dialogs.h"

namespace Himani {

	BOOL HBaseDialog::MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_INITDIALOG: {

			InitHandle(_hDlg);
			return OnInit();
		}

		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;

		case WM_PAINT:
			return OnPaint();
		default:
			return FALSE;
		}

		return TRUE;
	}


		
	/*HModalDialog::HModalDialog(HWindow& parent, const HString& resourceName)
		:HBaseDialog(parent, resourceName) {

		DialogBox(Himani::Instance(), resourceName.c_str(), (HWND)* parent, Procedure());
	}
	
	HModalDialog::HModalDialog(HWindow& parent, int resourceID)
		: HBaseDialog(parent, resourceID) {
		DialogBox(Himani::Instance(), MAKEINTRESOURCE(resourceID), (HWND)* parent, Procedure());
	}*/
}