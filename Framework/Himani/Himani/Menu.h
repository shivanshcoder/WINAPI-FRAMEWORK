#pragma once
#include"Core.h"
#include"HBaseWin.h"

//TODO later do this menu stuff

namespace Himani {
	class HMenu
	{
	public:

		//Constructors!!!!!!
		HMenu() {
			autoDeleteMenu = true;
			menu = CreateMenu();
			mainInstance = this;
		}

		//TODO need to fix the index ranges visual studio starts with like 4000s or something big!
		HMenu(int ResourceID) {
			menu = LoadMenu(Instance(), MAKEINTRESOURCE(ResourceID));
			mainInstance = this;
		}

		HMenu(const HString& Resource) {
			menu = LoadMenu(Instance(), Resource.c_str());
			mainInstance = this;
		}

		HMenu(HMenu&& other) {
			commands = std::move(other.commands);
			mainInstance = other.mainInstance;
			menu = other.menu;
			autoDeleteMenu = other.autoDeleteMenu;

			other.mainInstance = nullptr;
			other.menu = nullptr;
		}


		//Manipulation Functions!!

		void Append(MENUITEMINFO* item, HCommand* command) {
			int count = GetMenuItemCount(menu);

			InsertMenuItem(menu, count, TRUE, item);
			mainInstance->commands.push_back(command);
		}

		void AppendStrItem(HString& str, HCommand* command) {
			int count = GetMenuItemCount(menu);

			MENUITEMINFO item = { sizeof(MENUITEMINFO) };
			item.fMask = MIIM_STRING | MIIM_ID;
			item.dwTypeData = &str[0];
			item.wID = mainInstance->commands.size();


			InsertMenuItem(menu, count, TRUE, &item);
			mainInstance->commands.push_back(command);
		}

		void Insert(int index, MENUITEMINFO* item, HCommand* command) {
			int count = GetMenuItemCount(menu);
			if (index > count) {
				//ERROR index out of range!
				__debugbreak();
				return;
			}

			InsertMenuItem(menu, index, TRUE, item);
			mainInstance->commands.push_back(command);
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
			//Delete only once
			if(autoDeleteMenu)
				DestroyMenu(menu);
		}
	private:
		HMenu(HMENU handle, HMenu *mainInst) {
			menu = handle;
			mainInstance = mainInst;
		}

		class MenuItem {
		public:


			MenuItem(int Index, HMENU handleMenu, HMenu* mainInst)
				:index(Index), parentMenu(handleMenu), mainInstance(mainInst) {	}

			void SetInfo(MENUITEMINFO* info) {
				SetMenuItemInfo(parentMenu, index, TRUE, info);
			}

			MENUITEMINFO GetInfo() {
				MENUITEMINFO info = { sizeof(MENUITEMINFO) };
				GetMenuItemInfo(parentMenu, index, TRUE, &info);
				return info;
			}


			void Append(MENUITEMINFO* item) {
				MENUITEMINFO info = { sizeof(MENUITEMINFO) };
				info.fMask = MIIM_SUBMENU;
				//Is this the correct way?
				info.hSubMenu = CreateMenu();
				SetMenuItemInfo(parentMenu, index, TRUE, &info);
			}
			void AppendStrItem(HString& str, HCommand* command) {
				MENUITEMINFO info = { sizeof(MENUITEMINFO) };
				info.fMask = MIIM_SUBMENU;
				//Is this the correct way?
				info.hSubMenu = CreateMenu();
				SetMenuItemInfo(parentMenu, index, TRUE, &info);
				HMenu(info.hSubMenu,mainInstance).AppendStrItem(str, command); 
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

				HMENU subMenu = GetSubMenu(parentMenu, index);
				if (subMenu) {
					return HMenu(subMenu,mainInstance)[Index];
				}

				__debugbreak();
				throw Exceptions(TEXT("Index not available for the menu!"));

			}
		private:
			HMenu* mainInstance;
			HMENU parentMenu;
			int index = 0;
		};

	public:

		MenuItem operator[](int Index) {

			return MenuItem(Index, menu, mainInstance);
		}

		void Init() {
			MENUINFO Info = {};
			Info.cbSize = sizeof(MENUINFO);
			Info.fMask = MIM_STYLE;
			Info.dwStyle = MNS_NOTIFYBYPOS;

			SetMenuInfo(menu, &Info);
		}


		void callback(int menuItemID) {
			if (menuItemID == 40024) {
				__debugbreak();
			}

			if (menuItemID < mainInstance->commands.size()) {
				mainInstance->commands[menuItemID]->Execute();
			}
		}

	private:

		std::vector<HCommand*>commands;
		HMenu* mainInstance = nullptr;
		HMENU menu;
		bool autoDeleteMenu = false;
	};


}
