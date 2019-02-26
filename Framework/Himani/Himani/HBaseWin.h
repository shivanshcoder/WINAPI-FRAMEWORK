#pragma once
#include"Hpch.h"
#include"Helpers.h"
#include"WinProc.h"




namespace Himani {

	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName);


	class HBaseWin {
		friend class HWindow;
		friend class HPredefinedWindow;
		//friend class HWrapperWin;
	public:
		//Return Empty HBaseWin as nullptr
		HBaseWin() {
			hwnd = nullptr;
		}
		HBaseWin(HWND _hwnd) {
			hwnd = _hwnd;
		}

		//Doesn't check if the handle is nullptr or not
		operator HWND()const {
			return hwnd;
		}

		~HBaseWin() {}

		//Should it be protected?
	//protected:
		//Checks if the handle to window is nullptr or not
		HWND Handle() {
			if (!hwnd)
				throw Himani::Exceptions(L"nullptr Window Handle");
			return hwnd;
		}

	private:
		HWND hwnd;
	};


	class HWrapperWin :public HBaseWin {
	public:

		HString GetWinText();
		Helpers::HRect GetClientRect();

		Helpers::HRect GetWinRect();

		/*						Simple Wrappers						*/
		
		void SetWinText(const HString& Text) {
			SetWindowText(Handle(), Text.c_str());
		}

		void InvalidateRect(Helpers::HRect rect, bool redraw) {
			::InvalidateRect(Handle(), &rect.rect, redraw);
		}

		void MoveWindow(Helpers::HRect rect, bool Repaint) {
			::MoveWindow(Handle(), rect.left, rect.top, rect.right, rect.bottom, Repaint);
		}

		void SetScrollInfo(LPSCROLLINFO Info, int nBar, bool Redraw) {
			::SetScrollInfo(Handle(), nBar, Info, Redraw);
		}
		void GetScrollInfo(LPSCROLLINFO Info, int nBar) {
			::GetScrollInfo(Handle(), nBar, Info);
		}

		void SetMenu(HMENU menu) {
			::SetMenu(Handle(), menu);
		}

		void SetFocus() {
			::SetFocus(Handle());
		}

		void Show(int CmdShow) {
			::ShowWindow(Handle(), CmdShow);
		}
		bool IsVisible() {
			return IsWindowVisible(Handle());
		}
		void Enable() {
			::EnableWindow(Handle(), true);
		}
		void Disable() {
			::EnableWindow(Handle(), false);
		}
		/*						Simple Wrappers						*/

	};


#pragma region CUSTOM_CLASS_MACROS
	//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, IconSm, Cursor, Background, MenuName)	DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = Himani::RegisterWinClass(Style, Himani::StaticWndProc , Icon, IconSm, Cursor, Background, MenuName, ClassName());	\
		return __ValidClass;\
}

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style, (LoadIcon(NULL, IDI_APPLICATION)), NULL, (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)


#pragma endregion

	/*
	------------------------	Custom HWindow Classes ------------------------------
	Derive from HWindow Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
	*/
	class HWindow :public HWrapperWin, public HWindowsProc {

	public:
		HWindow(const HBaseWin &_Parent = HBaseWin()) {
			wndParent = _Parent;
		}
		HBaseWin Parent() { return wndParent; }

		HWindow(const HWindow&) = delete;
		HWindow& operator=(const HWindow&) = delete;
	protected:

		//Will be overriden using macro CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
		virtual LPCWSTR ClassName() = 0;
		virtual bool __ClassProp() { return 0; }


		//TODO make it void
		HWND CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr);


		HBaseWin wndParent;
	};

	//Static predefined Windows Classes
	class HPredefinedWindow :public HWrapperWin, public HWindowsProc {
	public:
		HPredefinedWindow(const HBaseWin &_Parent = HBaseWin()) {
			wndParent = _Parent;
		}

		HPredefinedWindow(const HPredefinedWindow&) = delete;
		HPredefinedWindow& operator=(const HPredefinedWindow&) = delete;

	protected:
		HBaseWin Parent() { return wndParent; }

		//Will be overriden using macro OVERRIDE_PREDEFINEDCLASS
		virtual LPCWSTR ClassName() = 0;

		void CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr);

		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override {
			return OldProc(hwnd, message, wParam, lParam);
		}
		WNDPROC OldProc;
		HBaseWin wndParent;
	};

}