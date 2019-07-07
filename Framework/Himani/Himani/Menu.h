#pragma once
#include"Core.h"
#include"HBaseWin.h"

//TODO later do this menu stuff

namespace Himani {
	class HMenu
	{
	public:
		HMenu() {
			menu = CreateMenu();
		}

		//TODO need to fix the index ranges visual studio starts with like 4000s or something big!
		HMenu(int ResourceID) {
			menu = LoadMenu(Instance(), MAKEINTRESOURCE(ResourceID));
		}

		HMenu(const HString& Resource) {
			menu = LoadMenu(Instance(), Resource.c_str());
		}


		void Append(MENUITEMINFO* item, HCommand* command) {
			int count = GetMenuItemCount(menu);

			InsertMenuItem(menu, count, TRUE, item);
			commands.push_back(command);
		}

		void AppendStrItem(HString &str, HCommand* command) {
			int count = GetMenuItemCount(menu);

			MENUITEMINFO item = { sizeof(MENUITEMINFO) };
			item.fMask = MIIM_STRING|MIIM_ID;
			item.dwTypeData = &str[0];
			item.wID = count;


			InsertMenuItem(menu, count, TRUE, &item);
			commands.push_back(command);
		}

		void Insert(int index, MENUITEMINFO* item, HCommand* command) {
			int count = GetMenuItemCount(menu);
			if (index > count) {
				//ERROR index out of range!
				__debugbreak();
				return;
			}

			InsertMenuItem(menu, index, TRUE, item);
			commands.push_back(command);
		}

		void AttachToWin(HWindow& win) {
			try {
				SetMenu(win.Handle(), menu);
			}
			catch (Exceptions& error) {
				__debugbreak();
				throw error;
			}
		}

		~HMenu() {
			DestroyMenu(menu);
		}
	private:
		HMenu(HMENU handle, int totalsize) {
			menu = handle;
			totalSize = totalSize;
		}

		class MenuItem {
		public:


			MenuItem(int Index, HMenu& handleMenu)
				:index(Index), parentMenu(handleMenu) {	}

			void SetInfo(MENUITEMINFO* info) {
				SetMenuItemInfo(parentMenu.menu, index, TRUE, info);
			}

			MENUITEMINFO GetInfo() {
				MENUITEMINFO info;
				GetMenuItemInfo(parentMenu.menu, index, TRUE, &info);
				return info;
			}


			void Append(MENUITEMINFO* item) {
				MENUITEMINFO info = { sizeof(MENUITEMINFO) };
				info.fMask = MIIM_SUBMENU;
				//Is this the correct way?
				info.hSubMenu = CreateMenu();
				SetMenuItemInfo(parentMenu.menu, index, TRUE, &info);
			}

			/*
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
			}*/

			MenuItem operator[](int Index) {

				HMENU subMenu = GetSubMenu(parentMenu.menu, index);
				if (subMenu) {
					return HMenu(subMenu, parentMenu.totalSize)[Index];
				}

				__debugbreak();
				throw Exceptions(TEXT("Index not available for the menu!"));

			}
		private:
			HMenu& parentMenu;
			int index = 0;
		};

	public:

		MenuItem operator[](int Index) {

			return MenuItem(Index, *this);
		}

		void Init() {
			MENUINFO Info = {};
			Info.cbSize = sizeof(MENUINFO);
			Info.fMask = MIM_STYLE;
			Info.dwStyle = MNS_NOTIFYBYPOS;

			SetMenuInfo(menu, &Info);
		}


		void callback(int menuItemID) {
			if (menuItemID < commands.size()) {
				commands[menuItemID]->Execute();
			}
		}

	private:
			
		std::vector<HCommand*>commands;

		int totalSize = 0;
		HMENU menu;
	};


}
