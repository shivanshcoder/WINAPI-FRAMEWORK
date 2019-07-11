#include "Hpch.h"
#include "Menu.h"

namespace Himani {

	void HMenu::AttachToWin(HWindow& win) {
		try {
			SetMenu(win.Handle(), menu);
		}
		catch (Exceptions& error) {
			__debugbreak();
			throw error;
		}
	}

	void HMenu::Init() {
		MENUINFO Info = { sizeof(MENUINFO) };
		Info.fMask = MIM_STYLE | MIM_MENUDATA;
		Info.dwStyle = MNS_NOTIFYBYPOS;
		Info.dwMenuData = (ULONG_PTR)this;

		SetMenuInfo(menu, &Info);
	}

	HMenu::HMenu(HMenu&& other) :HMenuBase(std::move(other)) {
		if (menu)
			DestroyMenu(menu);


		menu = other.menu;
		Init();
		other.menu = CreateMenu();

		Items = std::move(other.Items);
	}

	void HMenu::AppendStrItem(const HString& str, std::shared_ptr<HCommand> command) {
		HString tempStr = str;

		MENUITEMINFO info = { sizeof(MENUITEMINFO) };
		info.fMask = MIIM_STRING | MIIM_ID;
		info.fType = MFT_STRING;
		info.dwTypeData = &tempStr[0];
		info.wID = Items.size();

		InsertMenuItem(menu, info.wID, TRUE, &info);

		Items.push_back(std::make_shared<HMenuItem>(*this, info.wID, command));
	}

	//Replaces the reference of this object from the parent!
	HMenuBase&& HMenu::operator=(HMenuBase&& other) {

		//Check for parent 
		//else Throw error!
		if (!parent) {
			throw Exceptions(TEXT("Item cannot be assigned to a parent Menu"));
		}

		HMenu* parentRef = static_cast<HMenu*>(parent);
		if (other.Type() == Item)
			parentRef->ReplaceMenuItem(std::make_shared<HMenuItem>(std::move(static_cast<HMenuItem&&>(other))), index);
		else
			parentRef->ReplaceMenuItem(std::make_shared<HMenu>(std::move(static_cast<HMenu&&>(other))), index);
		return std::move(other);
	}

	void HMenu::UpdateIndexHandle(int _index) {
		HMenu* ptr = dynamic_cast<HMenu*>(&(*Items[_index]));

		MENUITEMINFO info = { sizeof(MENUITEMINFO) };
		info.fMask = MIIM_SUBMENU;

		//Incase type is Item
		if (ptr == nullptr) {
			info.hSubMenu = NULL;
		}
		//Incase type is Menu
		else
			info.hSubMenu = ptr->menu;

		SetMenuItemInfo(menu, _index, TRUE, &info);

	}

	void HMenu::ReplaceMenuItem(std::shared_ptr<HMenuBase> otherItem, int _index) {
		otherItem->index = _index;
		otherItem->parent = this;
		Items[_index] = otherItem;
		UpdateIndexHandle(_index);
	}

	HMenuBase&& HMenuItem::operator=(HMenuBase&& other) {
		if (other.Type() == Item) {
			HMenuItem& tempRef = static_cast<HMenuItem&>(other);
			command = std::move(tempRef.command);
		}
		//Is a menu
		else {
			HMenu* parentRef = static_cast<HMenu*>(parent);
			other.index = index;
			parentRef->ReplaceMenuItem(std::make_shared<HMenu>(std::move(static_cast<HMenu&&>(other))), index);
		}
		return std::move(other);
	}


}