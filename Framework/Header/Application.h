#pragma once
#include"GDI.hpp"
#include"BaseWin.hpp"
#include"Messages.h"
#include"Input.h"


namespace WINAPIPP {

	class CustomApplication :public Window {

	public:
		CustomApplication() = default;
		

		WPARAM Run() {

			ShowWindow(*this, WINAPIPP::CmdShow());
			UpdateWindow(*this);

			return MessageProcess();
		}

		WPARAM MessageProcess() {
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
		//Can be overriden Called after Window creation
		virtual WPARAM start() {
			return Run();
		}
		virtual void Idle() {}

		virtual ~CustomApplication() {} //= 0;
	};

	class Application : public CustomApplication {

	public:
		//using CustomApplication::CustomApplication;
		Application(const std::wstring &Tittle, DWORD style,
			Helpers::Rect size = { CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT }) {
			//NOTE Because Application can't have parent maybe??
			CreateWin(Tittle, style, size, nullptr);
		}
			
		
		CLASS_PROPERTIES(Application, CS_HREDRAW | CS_VREDRAW, NULL)

		virtual WPARAM start()override { return Run(); }
			
		DECLARE_MESSAGE_MAP();

		//virtual int OnPaint() = 0;
		//virtual int OnCreate() { return 0; }
		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }

	};

	MESSAGE_MAP_BEGIN(Application)
		case WM_CREATE: Init(hwnd); return 0;
		//MESSAGE_MAP_ENTRY(OnCreate, WM_CREATE)
		//MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
		MESSAGE_MAP_ENTRY(OnDestroy, WM_DESTROY)
		MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
		MESSAGE_MAP_ENTRY_PARAMS(OnMouseDown, WM_LBUTTONDOWN)
		MESSAGE_MAP_END()

}


