#include "Hpch.h"
#include "Application.h"

namespace Himani {
	WPARAM Himani::HCustomApplication::Run() {

		Show(Himani::CmdShow());
		Update();
		return MessageProcess();
	}

	WPARAM Himani::HCustomApplication::MessageProcess() {
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


	MESSAGE_MAP_BEGIN(HApplication) 
		MESSAGE_MAP_ENTRY(OnDestroy, WM_DESTROY)
		MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
		MESSAGE_MAP_ENTRY_PARAMS(OnMouseDown, WM_LBUTTONDOWN)
	MESSAGE_MAP_END(HCustomApplication)

}