#pragma once
#include"Core.h"

//TODO later do this menu stufffffffffffffffff

namespace Himani {
	class HMenu
	{
	public:
		HMenu(int ResourceID) {
			menu = LoadMenu(Instance(), MAKEINTRESOURCE(ResourceID));
		}

		HMenu(const HString& Resource) {
			menu = LoadMenu(Instance(), Resource.c_str());
		}


		//~HMenu() {
		//	DestroyMenu(menu);
		//}
	private:
		HMenu(HMENU handle) {
			menu = handle;
		}

		class MenuItem {
		public:
			MenuItem(int Index, HMENU handleMenu)
				:index(Index), parentMenu(handleMenu) {
				wchar_t temp[64];
				GetMenuString(parentMenu, Index, temp, 64, MF_BYPOSITION);
				//URGENT
				HMENU Submenu = GetSubMenu(parentMenu, Index);
				if (Submenu) {
					parentMenu = Submenu;
					index = -1;
				}
			}

			bool Enable() {
				EnableMenuItem(parentMenu, index, MF_ENABLED | MF_BYPOSITION);
			}
			bool Grayed() {
				EnableMenuItem(parentMenu, index, MF_GRAYED | MF_BYPOSITION);
			}
			bool Disable() {
				EnableMenuItem(parentMenu, index, MF_DISABLED | MF_BYPOSITION);
			}

			DWORD Check() {
				CheckMenuItem(parentMenu, index, MF_CHECKED | MF_BYPOSITION);
			}
			DWORD UnCheck() {
				CheckMenuItem(parentMenu, index, MF_UNCHECKED | MF_BYPOSITION);
			}

			MenuItem operator[](int Index) {
			//	HMENU subMenu = GetSubMenu(parentMenu, index);
			//	if (subMenu) {
					return HMenu(parentMenu)[Index];
				//}
			}
		private:
			int index = 0;
			HMENU parentMenu = NULL;
		};

	public:

		MenuItem operator[](int Index) {
			
			return MenuItem(Index, menu);
		}

		void Init() {
			MENUINFO Info = {};
			Info.cbSize = sizeof(MENUINFO);
			Info.fMask = MIM_STYLE;
			Info.dwStyle = MNS_NOTIFYBYPOS;

			SetMenuInfo(menu, &Info);
		}


	private:
		HMENU menu;
	};


}
