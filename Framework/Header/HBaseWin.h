#pragma once
#include"Hpch.h"
#include"Helpers.h"
#include"WinProc.h"

namespace HIMANI {


	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName) {
		WNDCLASSEX wndclass = {};
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = Style;
		wndclass.lpfnWndProc = Proc;
		wndclass.hInstance = Instance();
		wndclass.hbrBackground = Background;
		wndclass.hIcon = Icon;
		wndclass.hCursor = Cursor;
		wndclass.lpszClassName = ClassName;
		wndclass.lpszMenuName = MenuName;
		wndclass.hIconSm = IconSm;
		if (!RegisterClassEx(&wndclass)) {
			CheckDefaultWinError;
		}
		return true;
	}

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
	protected:
		//Checks if the handle to window is nullptr or not
		HWND Handle() {
			if (!hwnd)
				throw HIMANI::Exceptions(L"nullptr Window Handle");
			return hwnd;
		}

	private:
		HWND hwnd;
	};


	class HWrapperWin :public HBaseWin {
	public:

		HString GetWinText() {
			int size = GetWindowTextLength(Handle());
			HString str;
			str.resize(size);

			GetWindowText(Handle(), &str[0], size);
			return str;
		}

		void SetWinText(const HString& Text) {
			SetWindowText(Handle(), Text.c_str());
		}


		Helpers::HRect GetClientRect() {
			Helpers::HRect rect;
			::GetClientRect(Handle(), &rect.rect);

			return rect;
		}

		Helpers::HRect GetWinRect() {
			Helpers::HRect rect;
			::GetWindowRect(Handle(), &rect.rect);
			return rect;
		}

		void InvalidateRect(Helpers::HRect rect, bool redraw) {
			::InvalidateRect(Handle(), &rect.rect, redraw);
		}

		void MoveWindow(Helpers::HRect rect, bool Repaint) {
			auto t = ::MoveWindow(Handle(), rect.left, rect.top, rect.right, rect.bottom, Repaint);
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

	};


#pragma region CUSTOM_CLASS_MACROS
	//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, IconSm, Cursor, Background, MenuName)	DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = HIMANI::RegisterWinClass(Style, HIMANI::StaticWndProc , Icon, IconSm, Cursor, Background, MenuName, ClassName());	\
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
	class HWindow :public HWrapperWin, public HBaseWinProc {

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
		HWND CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr) {
			bool ValidClass = __ClassProp();

			if (!ValidClass)
				throw HIMANI::Exceptions(L"Class Not Registered");

			hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				wndParent.hwnd, //Parent HWindow
				Menu, Instance(),
				Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
			);

			if (!hwnd)
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Creation Unsuccessful");
			return hwnd;
		}


		HBaseWin wndParent;
	};

	//Static predefined Windows Classes
	class HPredefinedWindow :public HWrapperWin, public HBaseWinProc {
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

		void CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr) {

			hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				Parent(), //Parent HWindow
				Menu, Instance(), nullptr
			);
			if ((!hwnd))
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Creation Unsuccessful");
			OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Procedure());

			if (!OldProc)
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"HWindow Procedure swap Unsuccessful");

		}

		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override {
			return OldProc(hwnd, message, wParam, lParam);
		}
		WNDPROC OldProc;
		HBaseWin wndParent;
	};

}