#pragma once
#include"Hpch.h"
#include"Helpers.h"
#include"WinProc.h"
#include"Messages.h"




namespace Himani {

	bool RegisterWinClass(UINT Style, WNDPROC Proc, HICON Icon, HICON IconSm, HCURSOR Cursor, HBRUSH Background, PTSTR MenuName, PTSTR ClassName);


	LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	class HWindow :public HHandleWrapperBaseClass<HWND> {

	public:
		using HHandleWrapperBaseClass<HWND>::HHandleWrapperBaseClass;

		HWindow(HWindow&&) = default;

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

		virtual ~HWindow() {}
	};


	/*
	Class for Registering a Windows Class
	Add the Class properties to a Data Structure

	Usage:
	static Object should be created using this class inside HCustomWindow
	for std17++
		inline static HWinClassProperties Object = { ... };
	*/

	//TODO complete this class
	//template<class WinClassName>
	//struct HWinClass {
	//	HWinClass(Himani::HString className, WNDPROC procAddr, int classStyle) {
	//		ClassName += TEXT("Himani.WinClass.");
	//		ClassName += className;

	//		//ClassList.push_back({ ClassName, procAddr, classStyle });

	//	}

	//	/*When using a system Registered Class*/
	//	HWinClass(Himani::HString className) {
	//		ClassName += className;
	//	}

	//	//Registers all the Classes added to the Store
	//	//This funciton is called Automatically by Framework and Registers all the Window Classes before Entering the Main Application!
	//	static void RegisterAllClasses() {
	//		while (ClassList.size()) {
	//			auto ClassProp = ClassList.back();
	//			ClassList.pop_back();

	//			WNDCLASSEX wndclass = {};
	//			wndclass.cbSize = sizeof(WNDCLASSEX);
	//			wndclass.style = ClassProp.classStyle;
	//			wndclass.lpfnWndProc = ClassProp.procAddr;
	//			wndclass.hInstance = Instance();
	//			wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//			wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//			wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//			wndclass.lpszClassName = ClassProp.className.c_str();
	//			wndclass.lpszMenuName = NULL;
	//			wndclass.hIconSm = NULL;

	//			if (!RegisterClassEx(&wndclass)) {
	//				CheckDefaultWinError;
	//			}
	//		}
	//	}

	//	struct ClassProperties {
	//		Himani::HString className;
	//		WNDPROC procAddr;
	//		int classStyle;
	//	};
	//	
	//	HString ClassName;
	//	static inline std::vector<ClassProperties>ClassList;

	//};


	struct HWinClassProperties {

		HWinClassProperties(Himani::HString className, WNDPROC procAddr, int classStyle) {
			ClassName += TEXT("Himani.WinClass.");
			ClassName += className;

			ClassList.push_back({ ClassName/*, procAddr*/, classStyle });

		}

		/*When using a system Registered Class*/
		HWinClassProperties(Himani::HString className) {
			ClassName += className;
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
				wndclass.lpfnWndProc = CommonWndProc;
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
			//WNDPROC procAddr;
			int classStyle;
		};
		HString ClassName;
		static inline std::vector<ClassProperties>ClassList;

	};


	/*
	------------------------	Custom HCustomWindow Classes ------------------------------
	Derive from HCustomWindow Class
	Define Class properties using MACRO WINCLASS_PROP


	A Constructor should be made taking const HClassInitializer&
		with default Initialization with HClassInitilizer()

	!!!!No other Constructor Should be made!!!!

	Don't Use Constructor for Operations using Window
	Since the Window is created after calling CreateWin only!5
	*/
	class HCustomWindow :public HWindow, private HWindowsProc {
	private:
		//		HCustomWindow(HWND rawHandle)
		friend class HPredefinedWindow;

		using HWindow::HWindow;
		//protected:
		//	HCustomWindow() = default;
	public:

		//Can be overriden incase derived class stores the parent itself
		virtual HWindow* Parent() {
			HWND parent = nullptr;
			if (parent = GetParent(Handle())) {
				return (HWindow*)SendMessage(parent, H_WM_GETOWNINSTANCE, 0, 0);
			}
			return nullptr;
		}

		//Disable Copy Constructor and Assignment
		HCustomWindow(const HCustomWindow&) = delete;
		HCustomWindow& operator=(const HCustomWindow&) = delete;

		HCustomWindow(HCustomWindow&& other) :HWindow(std::move(other)) {
			//Swap the Procedure!
			UpdateProc();
			//Make the other UnInitialized
			other.InitHandle(nullptr);
		}

		//Disable Assignment for now 
		HCustomWindow& operator=(HCustomWindow&& other) {
			int c = lstrcmpi(other.ClassName().c_str(), ClassName().c_str());
			if (c) {
				//Add Error Handling later on!
				__debugbreak();
			}

			HWND temmp = (HWND)* this;


			InitHandle(other.Handle());
			//Swap the Procedure!
			//SetWindowLongPtr(Handle(), GWLP_WNDPROC, (LONG_PTR)Procedure());
			UpdateProc();
			//Make the other UnInitialized
			other.InitHandle(nullptr);

			if (temmp)DestroyWindow(temmp);
			return *this;
		}

		~HCustomWindow() {
			//TODO do the error checking!!!
			HWND rawHandle = (HWND)* this;
			//if()
			if (rawHandle) {
				if ((WNDPROC)GetWindowLongPtr((HWND)* this, GWLP_WNDPROC) == Procedure())
					SetWindowLongPtr((HWND)* this, GWLP_WNDPROC, (LONG_PTR)DefWindowProc);
			}
		}


		void CreateWinEx(const HString& Title, DWORD style, DWORD ExStyle = 0, HWindow * parent = nullptr, Helpers::HRect size = Helpers::HRect());
	protected:

		virtual HString& ClassName() = 0;

		virtual LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam);

		//Don't override 
		//TODO later on
		virtual WNDPROC UpdateProc() {
			return (WNDPROC)SetWindowLongPtr(Handle(), GWLP_WNDPROC, (LONG_PTR)Procedure());

		}

	private:
		LRESULT __MessageProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)final;
	};

	//Reserved for Framework shouldn't now be used anywhere else
	class ReservedTempWindow :public HCustomWindow {
	private:
	public:
		friend LRESULT CALLBACK CommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		ReservedTempWindow(const HString& classname, HWND hwnd) {
			className = classname;
			InitHandle(hwnd);
			UpdateProc();
		}

		HString& ClassName()final {
			return className;
		}

		LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)final {
			switch (message) {
			case WM_NCDESTROY:
				delete this;
				return 0;
			}

			return HCustomWindow::MessageFunc(message, wParam, lParam);
		}
		HString className;
	};
}