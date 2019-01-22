#pragma once
#include"Core.h"


namespace Himani {
	class HMenu
	{
	public:
		HMenu(int ResourceID) {
			menu = LoadMenu(Instance(), MAKEINTRESOURCE(ResourceID));
		//	Init();
		}

		HMenu(const HString& Resource) {
			menu = LoadMenu(Instance(), Resource.c_str());
		//	Init();
		}

	//	~HMenu();

		virtual int callback(int ID) {
			std::wstringstream ss;
			ss << ID << L"Was pressed";
			MessageBox(NULL, ss.str().c_str(), TEXT("TEMO"), MB_OK);
			return 0;
		}

	private:

		void Init() {
			MENUINFO Info = {};
			Info.cbSize = sizeof(MENUINFO);
			Info.fMask = MIM_STYLE;
			Info.dwStyle = MNS_NOTIFYBYPOS;

			SetMenuInfo(menu, &Info);
			CheckDefaultWinError;
		}
		//URGENT remove later
	public:
		HMENU menu;
	};


}
