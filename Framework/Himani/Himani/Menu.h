#pragma once
#include"Core.h"
#include"HBaseWin.h"


namespace Himani {
	using Himani::HCommand;
	using Himani::HString;

	/*
		Abstract Class for Menu Handling!
	*/
	class HMenuBase {
		friend class HMenuItem;
		friend class HMenu;
	public:
		HMenuBase(int _index, HMenuBase* ptrparent = nullptr) :
			parent(ptrparent), index(_index) {}

		enum MenuItemType {
			//Is a simple item of a menu
			Item,

			//Is a Sub Menu
			Menu

		};

		//should be called with MenuItem instance
		virtual void callback() = 0;

		
		virtual HMenuBase& operator[](int index) = 0;

		//Replaces the reference of this object from the parent!
		virtual HMenuBase&& operator=(HMenuBase&& other) = 0;

		virtual ~HMenuBase() {}

		//Returns the Instance type
		virtual MenuItemType Type() = 0;
	protected:

		HMenuBase(HMenuBase&& other) {
			parent = other.parent;
			other.parent = nullptr;

			index = other.index;
			other.index = -1;
		}

		//Stores the parent Menu
		HMenuBase* parent = nullptr;

		//Stores the index the current instance is placed in parent
		int index = -1;
	private:

	};


	class HMenu :public HMenuBase {
		friend class HMenuItem;

	public:
		HMenu(int _index = -1) :HMenuBase(_index) {
			menu = CreateMenu();
			Init();
		}
		~HMenu() {
			if (menu) {
				DestroyMenu(menu);
			}
		}

		HMenuBase& operator[](int _index)  override {
			return *(Items[_index]);
		}

		//Attaches the Menu to the Window
		void AttachToWin(HWindow& win);

		void callback() {}

		//Stores the instance in the menu handle internally
		void Init();
		
		HMenu(HMenu&& other);

		HMenu(HMenu&) = delete;
		HMenu& operator=(HMenu&) = delete;

		//Append string menu item into the menu
		void AppendStrItem(const HString& str, std::shared_ptr<HCommand> command);

		HMenuBase&& operator=(HMenuBase&& other) override;


		HMenuBase&& operator=(HMenuItem&& other) = delete;
		HMenu&& operator=(HMenu&& other) = delete;


	private:
		void UpdateIndexHandle(int _index);

		//Child Menu notifies the parent for replacing its place with another instance
		void ReplaceMenuItem(std::shared_ptr<HMenuBase> otherItem, int _index);

		MenuItemType Type()override { return MenuItemType::Menu; }

		//Either store simple items or further menus
		std::vector< std::shared_ptr<HMenuBase> >Items;

	public:
		HMENU menu = 0;
	};

	class HMenuItem :public HMenuBase {
	public:
		HMenuItem(HMenu& parentMenu, int _index, std::shared_ptr<HCommand>_command)
			:HMenuBase(_index, &parentMenu), command(_command) {}

		HMenuItem(HMenuItem&& other) :HMenuBase(std::move(other)) {
				command = other.command;
		}
		
		HMenuBase&& operator=(HMenuBase&& other)override;

		void callback() {
			command->Execute();
		}

	private:

		HMenuBase& operator[](int index)override {
			__debugbreak();
			throw Exceptions(TEXT("No such Menu Item!"));
		}

		MenuItemType Type()override { return MenuItemType::Item; }
		std::shared_ptr<HCommand>command;
	};

}


