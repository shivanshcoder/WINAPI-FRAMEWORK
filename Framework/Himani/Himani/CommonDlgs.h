#pragma once
#include"HBaseWin.h"

namespace Himani {
	class FileOpenDlg {
	public:
		FileOpenDlg() :ofn{ sizeof(OPENFILENAME) } {
			ofn.lpstrFilter = filters.c_str();


		}

		HString filters;

		OPENFILENAME ofn;


	};
}