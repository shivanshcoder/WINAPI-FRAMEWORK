#pragma once
#include"GDI.h"
#include"HBaseWin.h"
#include"Messages.h"
#include"Menu.h"
#include"Input.h"


namespace Himani {

	class HCustomApplication :public HWindow {

	public:
		HCustomApplication() = default;


		WPARAM Run();

		virtual WPARAM MessageProcess();

		//Can be overriden Called after HWindow creation
		virtual WPARAM start() { return Run(); }
		virtual void Idle() {}

		virtual ~HCustomApplication() {}
	};

	class HApplication : public HCustomApplication {

	public:
		//using HCustomApplication::HCustomApplication;
		HApplication(const HString &Tittle, DWORD style,
			Helpers::HRect size = { CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT }) {
			//NOTE Because HApplication can't have parent maybe??
			CreateWin(Tittle, style, size);

		}


		CLASS_PROPERTIES(HApplication, CS_HREDRAW | CS_VREDRAW, NULL)


			DECLARE_MESSAGE_MAP();
		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }

	};


}


