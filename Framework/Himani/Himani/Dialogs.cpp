#include "Hpch.h"
#include "Dialogs.h"

namespace Himani {
	
	HModalDialog::HModalDialog(HBaseWin * parent, int ResourceID)
	{
	}
	BOOL HBaseDialog::MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

			//WM_CHARTOITEM
			//	WM_COMPAREITEM
			//	WM_CTLCOLORBTN
			//	WM_CTLCOLORDLG
			//	WM_CTLCOLOREDIT
			//	WM_CTLCOLORLISTBOX
			//	WM_CTLCOLORSCROLLBAR
			//	WM_CTLCOLORSTATIC
			//	WM_INITDIALOG
			//	WM_QUERYDRAGICON
			//	WM_VKEYTOITEM
		case WM_COMMAND:
			OnCommand();
			break;

		default:
			return FALSE;
		}

		return TRUE;
	}
}