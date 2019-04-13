#pragma once
#include"Hpch.h"
#include"Helpers.h"
#include"WinProc.h"
#include"Messages.h"




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

	private:
		//Create Instance using Raw Handle to Window
		//Reserved for Framework Use only!
		HWindow(HWND Handle) {
			InitHandle(Handle);
		}

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
		void InvalidateClient(bool redraw)const {
			::InvalidateRect(Handle(), NULL, redraw);
		}


		void Update()const {
			::UpdateWindow(Handle());
		}

		void MoveWindow(Helpers::HRect rect, bool Repaint)const {
			::MoveWindow(Handle(), rect.left, rect.top, rect.right, rect.bottom, Repaint);
		}

		void SetScrollInfo(LPSCROLLINFO Info, int nBar, bool Redraw) const {
			::SetScrollInfo(Handle(), nBar, Info, Redraw);
		}
		void GetScrollInfo(LPSCROLLINFO Info, int nBar) const {
			::GetScrollInfo(Handle(), nBar, Info);
		}

		void SetMenu(HMENU menu) const {
			::SetMenu(Handle(), menu);
		}

		void SetFocus() const {
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
	Class for Registering a Windows Class 
	Add the Class properties to a Data Structure 

	Usage:
	static Object should be created using this class inside HCustomWindow
	for std17++
		inline static HWinClassProperties Object = { ... }; 
	*/
	class HWinClassProperties {
	public:
		HWinClassProperties(Himani::HString className, WNDPROC procAddr, int classStyle) {
			ClassList.push_back({ className, procAddr, classStyle });

		}

		//Registers all the Classes added to the Store
		//This funciton is called Automatically by Framework and Registers all the Window Classes before Entering the Main Application!
		static void RegisterAllClasses() {
			while (ClassList.size()) {
				auto ClassProp = ClassList.back();
				ClassList.pop_back();

				WNDCLASSEX wndclass = {};
				wndclass.cbSize = sizeof(WNDCLASSEX);
				wndclass.style = ClassProp.classStyle;
				wndclass.lpfnWndProc = ClassProp.procAddr;
				wndclass.hInstance = Instance();
				wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
				wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
				wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
				wndclass.lpszClassName = ClassProp.className.c_str();
				wndclass.lpszMenuName = NULL;
				wndclass.hIconSm = NULL;

				if (!RegisterClassEx(&wndclass)) {
					CheckDefaultWinError;
				}
			}
		}

		struct ClassProperties {
			Himani::HString className;
			WNDPROC procAddr;
			int classStyle;
		};

		static std::vector<ClassProperties>ClassList;
	};

	std::vector <HWinClassProperties::ClassProperties> HWinClassProperties::ClassList;

	/*
	------------------------	Custom HCustomWindow Classes ------------------------------
	Derive from HCustomWindow Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES

	The Derived Class should have Default Constructor!
	constructor()
	No other Constructor Should be made!
	*/
	class HCustomWindow :public HWindow, public HWindowsProc {

	public:
		HWindow* Parent() { return wndParent; }

		HCustomWindow(const HCustomWindow&) = delete;
		HCustomWindow& operator=(const HCustomWindow&) = delete;


	protected:

		DECLARE_MESSAGE_MAP();

		//Will be overriden using macro CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
		virtual LPCWSTR ClassName() = 0;
		virtual bool __ClassProp() { return 0; }

		void CreateWin(const HString& Title, DWORD style, Helpers::HRect size, HWindow* parent);

		HWindow* wndParent = nullptr;
	};

	//Static predefined Windows Classes
	class HPredefinedWindow :public HWindow, public HWindowsProc {
	public:
		HPredefinedWindow(const HWindow& _Parent = HWindow()) {
			wndParent = _Parent;
		}

		HPredefinedWindow(const HPredefinedWindow&) = delete;
		HPredefinedWindow& operator=(const HPredefinedWindow&) = delete;


	protected:
		DECLARE_MESSAGE_MAP();

		HWindow Parent() { return wndParent; }

		//Will be overriden using macro OVERRIDE_PREDEFINEDCLASS
		virtual LPCWSTR ClassName() = 0;

		void CreateWin(const HString& Title, DWORD style, Helpers::HRect size);

		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override {
			return OldProc(hwnd, message, wParam, lParam);
		}

		WNDPROC OldProc;
		HWindow wndParent;
	};

}