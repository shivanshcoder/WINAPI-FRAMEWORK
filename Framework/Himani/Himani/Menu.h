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

	//	~HMenu();

		virtual int callback(int ID);

	private:
		HMenu(HMENU handle) {
			menu = handle;
		}

		class MenuItem {
		public:
			MenuItem(int Index, HMENU handleMenu)
			:index(Index), parentMenu(handleMenu){}

			bool Enable() {
				EnableMenuItem(parentMenu, index, MF_ENABLED | MF_BYPOSITION);
			}
			bool Grayed() {
				EnableMenuItem(parentMenu, index, MF_GRAYED| MF_BYPOSITION);
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

		private:
			int index = 0;
			HMENU parentMenu = NULL;
		};

	public:



		HMenu operator[](int Index) {
			HMENU Submenu = GetSubMenu(menu, Index);
			if (Submenu) {
				return HMenu(Submenu);
			}
			return *this;
		}

		void Init() {
			MENUINFO Info = {};
			Info.cbSize = sizeof(MENUINFO);
			Info.fMask = MIM_STYLE;
			Info.dwStyle = MNS_NOTIFYBYPOS;

			SetMenuInfo(menu, &Info);
		}
		//URGENT remove later
	public:
		HMENU menu;
	};


}
