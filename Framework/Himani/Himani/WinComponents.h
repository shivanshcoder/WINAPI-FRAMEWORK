#pragma once
#include"Core.h"
#include"HBaseWin.h"
#include"Menu.h"

namespace Himani {
	class HWindowDecorators;

	//This Decorator was just for Checking
	//Menus should be directly using other way!

	class HWindowDecorators :public HWindow {
		friend class HWinDecoratorFactory;
	public:

		virtual LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam) {

			switch (message) {
			case WM_COMMAND: {
				if ((lParam == 0) && (HIWORD(wParam) == 0)) {
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

	//This Decorator was just for Checking
	//Menus should be directly using other way!
	/*class HWinMenu :public HWindowDecorators {
		friend class HWinDecoratorFactory;
	public:
		HWinMenu(std::shared_ptr<HMenu>_menu) : menu(_menu) {}
	private:

		virtual void OnMenuClick(int ID)override {
			menu->callback(ID);
		}

		std::shared_ptr<HMenu>menu;
	};
*/
	class HWinDecoratorFactory {
	public:

		static inline void AttachDecorator(HWindowDecorators* decoratorRef, HWindow& winRef) {
			//decoratorRef stores the main Wins last stored decoratorRef
			decoratorRef->winRef = winRef.winRef;
			//Main win now stores the new decoratorRef
			winRef.winRef = decoratorRef;
			decoratorRef->InitHandle(winRef.Handle());
		}

	};
}