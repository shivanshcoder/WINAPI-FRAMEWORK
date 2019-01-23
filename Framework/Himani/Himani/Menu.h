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

		virtual int callback(int ID);

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
