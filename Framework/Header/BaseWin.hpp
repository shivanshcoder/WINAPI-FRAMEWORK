#pragma once
#include"Helpers.h"
#include"WinProc.hpp"

namespace WINAPIPP {


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

	class BaseWin {
		friend class Window;
		friend class PredefinedWindow;
		friend class WrapperWin;
	public:
		//Return Empty BaseWin as NULL
		BaseWin() {
			hwnd = NULL;
		}
		BaseWin(HWND _hwnd) {
			hwnd = _hwnd;
		}

		operator HWND()const {
			return hwnd;
		}

		~BaseWin() {}
	private:
		HWND hwnd;
	};


	class WrapperWin :public BaseWin {
	public:
		Helpers::Rect GetClientRect() {
			Helpers::Rect rect;
			if (!::GetClientRect(hwnd, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		Helpers::Rect GetWinRect() {
			Helpers::Rect rect;
			if (!::GetWindowRect(hwnd, &rect.rect)) {
				throw std::exception("Rect error");
			}
			return rect;
		}

		void InvalidateRect(Helpers::Rect rect, bool redraw) {
			if (!::InvalidateRect(hwnd, &rect.rect, redraw))
				throw std::exception("Error");

		}


	};


#pragma region CUSTOM_CLASS_MACROS
	//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, IconSm, Cursor, Background, MenuName)	DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = WINAPIPP::RegisterWinClass(Style, WINAPIPP::StaticWndProc , Icon, IconSm, Cursor, Background, MenuName, ClassName());	\
		return __ValidClass;\
}

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style, MenuName) CLASS_ALL_PROPERTIES(ClassName__, Style, (LoadIcon(NULL, IDI_APPLICATION)), NULL, (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), MenuName)


#pragma endregion

	/*
	------------------------	Custom Window Classes ------------------------------
	Derive from Window Class
	Define Class properties using MACRO CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
	*/
	class Window :public WrapperWin, public BaseWinProc {

	public:
		Window(const BaseWin &_Parent = BaseWin()) {
			wndParent = _Parent;
		}
		BaseWin Parent() { return wndParent; }

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	protected:

		//Will be overriden using macro CLASS_ALL_PROPERTIES or CLASS_PROPERTIES
		virtual LPCWSTR ClassName() = 0;
		virtual bool __ClassProp() { return 0; }


		//TODO make it void
		HWND CreateWin(const std::wstring &Tittle, DWORD style, Helpers::Rect size, HMENU Menu = nullptr) {
			bool ValidClass = __ClassProp();

			if (!ValidClass)
				throw std::exception("Class Not Registered");

			hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				Parent(), //Parent Window
				Menu, Instance(),
				Procedure()//Procedure is sent using extra param inorder to replace it with our static Procedure
			);

			if (!hwnd)
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"Window Creation Unsuccessful");
			return hwnd;
		}


		BaseWin wndParent;
	};

	//Static predefined Windows Classes
	class PredefinedWindow :public WrapperWin, public BaseWinProc {
	public:
		PredefinedWindow(const BaseWin &_Parent = BaseWin()) {
			wndParent = _Parent;
		}

		PredefinedWindow(const PredefinedWindow&) = delete;
		PredefinedWindow& operator=(const PredefinedWindow&) = delete;

	protected:
		BaseWin Parent() { return wndParent; }

		//Will be overriden using macro OVERRIDE_PREDEFINEDCLASS
		virtual LPCWSTR ClassName() = 0;

		void CreateWin(const std::wstring &Tittle, DWORD style, Helpers::Rect size, HMENU Menu = nullptr) {

			hwnd = CreateWindowExW(0, ClassName(), //ClassName using virtual function
				Tittle.c_str(), style,
				size.left, size.top, size.right, size.bottom,
				Parent(), //Parent Window
				Menu, Instance(), nullptr
			);

			OldProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Procedure());

			if ((!hwnd) || (!OldProc))
				throw WinExceptions(__LINE__, TEXT(__FILE__) L"Window Creation Unsuccessful");

		}

		virtual LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)override {
			return OldProc(hwnd, message, wParam, lParam);
		}
		WNDPROC OldProc;
		BaseWin wndParent;
	};

	class StaticWindow :public PredefinedWindow {
	public:
		OVERRIDE_PREDEFINEDCLASS(static)

			StaticWindow(const std::wstring &Tittle, int Style, Helpers::Rect Size, const BaseWin &_Parent = BaseWin()) :PredefinedWindow(_Parent) {
			CreateWin(Tittle, Style, Size, NULL);
		}

		StaticWindow(const StaticWindow&) = delete;
		StaticWindow& operator=(const StaticWindow&) = delete;

	};

	template<class T, int Size>
	class WinArray {
	public:
		/*WinArray(const std::wstring &Tittle, int Style, Helpers::Rect rect, const WINAPIPP::BaseWin &Parent) {
			for (int i = 0; i < Size; ++i) {
				Windows[i] = std::make_unique<T>(Tittle, Style, rect, Parent);
			}
		}*/
		template<class... _Types>
		WinArray(_Types&&... _Args) {
			for (int i = 0; i < Size; ++i)
				Windows[i] = std::make_unique<T>(std::forward<T>(_Args));

		}
	public:
		std::unique_ptr<T>Windows[Size];
	};
}