#pragma once
#include"Core.h"
#include"HBaseWin.h"
#include"Menu.h"

namespace Himani {
	class HWindowDecorators;

	class HWinDecoratorFactory {
	public:

		static inline void AttachDecorator(HWindowDecorators& decoratorRef, HWindow& winRef) {
			winRef.winRef = &decoratorRef;
		}


	};

	class HWindowDecorators :public HWindow {
		friend class HWinDecoratorFactory;
	public:
		HWindowDecorators(HWindow* ref) :HWindow(ref) {
			InitHandle(ref->Handle());
		}

		virtual LRESULT MessageFunc__(UINT message, WPARAM wParam, LPARAM lParam) {

			switch (message) {
			case WM_COMMAND: {
				if ((lParam == 0) && (HIWORD(wParam))) {
					OnMenuClick(LOWORD(wParam));
					return 0;
				}
				break;
			}
			case WM_HSCROLL: {

				//Sent by a control break;
				if (lParam)break;

				OnHorzScroll(LOWORD(wParam));
				return 0;
			}
			case WM_VSCROLL: {

				//Sent by a control break;
				if (lParam)break;

				OnHorzScroll(LOWORD(wParam));
				return 0;
			}


			}

			return HWindow::MessageFunc(message, wParam, lParam);
		}
		virtual void OnVertScroll(int scrollReq) {

		}
		virtual void OnHorzScroll(int scrollReq) {

		}
		virtual void OnMenuClick(int ID) {

		}
	private:
		HWindow* winRef;

	};

	HWindow* AttachMenu(HWindow& ref, std::shared_ptr<HMenu>menu);
	class HWinMenu :public HWindowDecorators {

	public:


	private:
		HWinMenu(HWindow* ref, std::shared_ptr<HMenu>_menu) :HWindowDecorators(ref), menu(_menu) {}


		virtual void OnMenuClick(int ID)override {
			menu->callback(ID);
		}

		std::shared_ptr<HMenu>menu;
	};

}