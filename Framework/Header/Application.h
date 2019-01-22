#pragma once
#include"GDI.h"
#include"HBaseWin.h"
#include"Messages.h"
#include"Menu.h"
#include"Input.h"


namespace HIMANI{

	class HCustomApplication :public HWindow {

	public:
		HCustomApplication() = default;
		

		WPARAM Run() {

			ShowWindow(*this, HIMANI::CmdShow());
			UpdateWindow(*this);
			return MessageProcess();
		}

		virtual WPARAM MessageProcess() {
			MSG msg;

			while (true) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT)
						break;
					
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					
				}
				else {
					Idle();
				}
			}

			return msg.wParam;
		}

		//Can be overriden Called after HWindow creation
		virtual WPARAM start() {
			return Run();
		}
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

		virtual WPARAM start()override { return Run(); }
			
		DECLARE_MESSAGE_MAP();
		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }
		
	};

	MESSAGE_MAP_BEGIN(HApplication)
		MESSAGE_MAP_ENTRY(OnDestroy, WM_DESTROY)
		MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
		MESSAGE_MAP_ENTRY_PARAMS(OnMouseDown, WM_LBUTTONDOWN)
	MESSAGE_MAP_END(HCustomApplication)

}


