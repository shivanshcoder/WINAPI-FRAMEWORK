#include "Hpch.h"
#include "Dialogs.h"

namespace Himani {

	BOOL HBaseDialog::MessageFunc( UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {

		case WM_INITDIALOG: {
			return OnInit();
		}

		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;

		case WM_PAINT:
			return OnPaint();

		case WM_CLOSE:
			EndDialog(0);
			return TRUE;
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