#pragma once
#include"GDI.h"
#include"HBaseWin.h"
#include"Messages.h"
#include"Menu.h"
#include"Input.h"


namespace Himani {

	class HSimpleWindow :public HCustomWindow {
	public:
		HSimpleWindow(const HClassInitializer &Args = HClassInitializer()):HCustomWindow(Args) {}

		WINCLASS_PROPERTIES(HSimpleWindow, CS_HREDRAW | CS_VREDRAW);
	};

	class HApplication :public HCustomWindow {

	public:
		HApplication(const HString& Title, DWORD style,
			Helpers::HRect size = { CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT }) :HCustomWindow(HClassInitializer()){
			//NOTE Because HApplication can't have parent maybe??
			CreateWin(Title, style, nullptr, size);

		}

		WPARAM Run();

		virtual WPARAM MessageProcess();

		//Can be overriden for Custom Startup!
		//Should return by calling MessageProcess preferably
		virtual WPARAM start() { return Run(); }

		virtual void Idle() {}

		virtual ~HApplication() {}

		LRESULT CALLBACK MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) override;

		HString& ClassName()override {
			return Prop.ClassName;
		}

		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }


	protected:
		static LRESULT CALLBACK AppCommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

			switch (message) {
			case H_WM_SWAPPROCADDR:
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)(lParam));
				return 0;
			}
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

	private:
		inline static Himani::HWinClassProperties Prop = { TEXT("HApplication"), Himani::HApplication::AppCommonWndProc,CS_VREDRAW | CS_HREDRAW };

	};

}


