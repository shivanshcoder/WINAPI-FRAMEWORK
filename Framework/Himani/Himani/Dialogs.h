#pragma once
#include"Core.h"
#include"WinProc.h"

namespace Himani {

	class HBaseWin;

	class HBaseDialog :public HDialogProc{
	public:
		//HBaseDialog(HBaseWin *parent, const HString &ResourceName);
		//HBaseDialog(HBaseWin *parent, int ResourceID);

		virtual BOOL MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

		virtual void OnCommand() {

		}

	private:
		HBaseWin *Parent;	
	};

	class HModalDialog :public HBaseDialog{
	public:
		HModalDialog(HBaseWin *parent, int ResourceID);
	};

	class HModlessDialog {

	};

}