#pragma once
#include"GDI.h"
#include"HBaseWin.h"
#include"Messages.h"
#include"Menu.h"
#include"Input.h"


namespace Himani {

	class HCustomApplication :public HCustomWindow {

	public:
		HCustomApplication() = default;


		WPARAM Run();

		virtual WPARAM MessageProcess();

		//Can be overriden Called after HCustomWindow creation
		virtual WPARAM start() { return Run(); }
		virtual void Idle() {}

		virtual ~HCustomApplication() {}

	protected:
		LRESULT CALLBACK AppCommonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

		}

	};

	class HApplication : public HCustomApplication {

	public:
		//using HCustomApplication::HCustomApplication;
		HApplication(const HString &Title, DWORD style,
			Helpers::HRect size = { CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT }) {
			//NOTE Because HApplication can't have parent maybe??
			CreateWin(Title, style, size,NULL);

		}


		//CLASS_PROPERTIES(HApplication, CS_HREDRAW | CS_VREDRAW, NULL)

		HString& ClassName()override {
			return Prop.ClassName;
		}

		DECLARE_MESSAGE_MAP();
		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }

	private:
		inline static Himani::HWinClassProperties Prop = { TEXT("HApplication"), Himani::CommonWndProc<HApplication>,CS_VREDRAW | CS_HREDRAW };

	};


}


