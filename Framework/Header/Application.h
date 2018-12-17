#pragma once
#include"GDI.hpp"
#include"CustomWinClass.h"
/*
#include"Wrappers.hpp"
#include"WinProc.hpp"*/


namespace WINAPIPP {

	class CustomApplication :public CUSTOM_CLASS {

	public:
		CustomApplication(std::wstring ClassName)	
			:CUSTOM_CLASS(ClassName) {}


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

		virtual WPARAM start() = 0;
		virtual void Idle() {

		}

	};

	class Application : public CustomApplication {

	public:
		Application()
			:CustomApplication(std::wstring(TEXT("Application"))) {}
		void ClassProp(WNDCLASS &wndclass) override{
			wndclass.style = CS_VREDRAW | CS_HREDRAW;
			wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		}

		DECLARE_MESSAGE_MAP();

		virtual int OnPaint() {
			WINAPIPP::PaintDC dc(*this);
			return 0;
		}
		virtual int OnDestroy() {
			PostQuitMessage(0);
			return 0;
		}
		virtual int OnSize(WPARAM wParam, LPARAM lParam) { return 0; }
		virtual int OnMouseDown(WPARAM wParam, LPARAM lParam) { return 0; }

	};

	MESSAGE_MAP_BEGIN(Application)
		MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
		MESSAGE_MAP_ENTRY(OnDestroy, WM_DESTROY)
		MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
		MESSAGE_MAP_ENTRY_PARAMS(OnMouseDown, WM_LBUTTONDOWN)
	MESSAGE_MAP_END()

}


