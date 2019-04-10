#pragma once
#include"Hpch.h"
#include"Helpers.h"
#include"WinProc.h"




namespace Himani {

	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, LPCWSTR MenuName, LPCWSTR ClassName);


	//class HWindow {
	//	friend class HCustomWindow;
	//	friend class HPredefinedWindow;
	//	friend class HBaseDialog;
	//	//friend class HWindow;
	//public:
	//	//Return Empty HWindow as nullptr
	//	HWindow() {
	//		hwnd = nullptr;
	//	}
	//	HWindow(HWND _hwnd) {
	//		hwnd = _hwnd;
	//	}
	//
	//	//Doesn't check if the handle is nullptr or not
	//	operator HWND()const {
	//		return hwnd;
	//	}

	//	~HWindow() {}

	//	//Should it be protected?
	////protected:
	//	//Checks if the handle to window is nullptr or not
	//	HWND Handle() {
	//		if (!hwnd)
	//			throw Himani::Exceptions(L"nullptr Window Handle");
	//		return hwnd;
	//	}

	//private:
	//	HWND hwnd;
	//};


	class HWindow :public HHandleWrapperBaseClass<HWND> {
	public:
		using HHandleWrapperBaseClass<HWND>::HHandleWrapperBaseClass;

		HString GetWinText();
		Helpers::HRect GetClientRect();

		Helpers::HRect GetWinRect();

		/*						Simple Wrappers						*/
		
		void SetWinText(const HString& Text) const {
			SetWindowText(Handle(), Text.c_str());
		}

		void InvalidateRect(Helpers::HRect rect, bool redraw)const {
			::InvalidateRect(Handle(), &rect.rect, redraw);
		}

		//Invalidates whole Cient Area
		void InvalidateClient(bool redraw)const{
			::InvalidateRect(Handle(), NULL, redraw);
		}

		
		void Update()const {
			::UpdateWindow(Handle());
		}

		void MoveWindow(Helpers::HRect rect, bool Repaint)const {
			::MoveWindow(Handle(), rect.left, rect.top, rect.right, rect.bottom, Repaint);
		}

		void SetScrollInfo(LPSCROLLINFO Info, int nBar, bool Redraw) const{
			::SetScrollInfo(Handle(), nBar, Info, Redraw);
		}
		void GetScrollInfo(LPSCROLLINFO Info, int nBar) const{
			::GetScrollInfo(Handle(), nBar, Info);
		}

		void SetMenu(HMENU menu) const{
			::SetMenu(Handle(), menu);
		}

		void SetFocus() const{
			::SetFocus(Handle());
		}

		void Show(int CmdShow) const {
			::ShowWindow(Handle(), CmdShow);
		}
		bool IsVisible()const {
			return IsWindowVisible(Handle());
		}
		void Enable()const {
			::EnableWindow(Handle(), true);
		}
		void Disable() const {
			::EnableWindow(Handle(), false);
		}
		/*						Simple Wrappers						*/

	};


	/*
	------------------------	Custom HCustomWindow Classes ------------------------------
	Derive from HCustomWindow Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
	*/
	class HCustomWindow :public HWindow, public HWindowsProc {

	public:
		HCustomWindow(const HWindow &_Parent = HWindow()) {
			wndParent = _Parent;
		}
		HWindow Parent() { return wndParent; }

		HCustomWindow(const HCustomWindow&) = delete;
		HCustomWindow& operator=(const HCustomWindow&) = delete;
	protected:

		//Will be overriden using macro CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
		virtual LPCWSTR ClassName() = 0;
		virtual bool __ClassProp() { return 0; }


		void CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr);

		HWindow wndParent;
	};

	//Static predefined Windows Classes
	class HPredefinedWindow :public HWindow, public HWindowsProc {
	public:
		HPredefinedWindow(const HWindow &_Parent = HWindow()) {
			wndParent = _Parent;
		}

		HPredefinedWindow(const HPredefinedWindow&) = delete;
		HPredefinedWindow& operator=(const HPredefinedWindow&) = delete;

	protected:
		HWindow Parent() { return wndParent; }

		//Will be overriden using macro OVERRIDE_PREDEFINEDCLASS
		virtual LPCWSTR ClassName() = 0;

		void CreateWin(const HString &Tittle, DWORD style, Helpers::HRect size, HMENU Menu = nullptr);

		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override {
			return OldProc(hwnd, message, wParam, lParam);
		}
		WNDPROC OldProc;
		HWindow wndParent;
	};

}