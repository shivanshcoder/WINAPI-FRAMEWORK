#include"Header/BaseWin.hpp"

namespace WINAPIPP {

	
	void BaseWin::Create (std::wstring WindowName) {
		
		if (!ClassRegistered)
			GenerateDefaultClass ();

		Window = CreateWindowEx (0, ClassName.c_str (), WindowName.c_str (), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle (NULL), NULL);
		
	}

	ATOM BaseWin::GenerateDefaultClass () {
		ClassRegistered = true;
		ClassName = TEXT("TEMP");
		WinClassProp = new WindowClassEx (ClassName.c_str ());;
		WinClassProp->AttachProc (Procedure);
		return WinClassProp->Register ();
	}

	// Some PreProccessing of some common Messages
	// If you override of this func all the Messages need to be handled or 
	// need to be returned to DefWindowProc 
	LRESULT BaseWin::MessageFunc (HWND Window, UINT message, WPARAM wParam, LPARAM lParam)  {
		switch (message) {
		case WM_CREATE: {
			LPCREATESTRUCT CreateStr = reinterpret_cast<LPCREATESTRUCT>(lParam);
			OnCreate (*CreateStr);
		}return 0;


		case WM_PAINT: {
			PAINTSTRUCT ps;

			HDC DeviceClient = BeginPaint (Window, &ps);
			OnPaint (DeviceClient, ps);

			EndPaint (Window, &ps);

		}return 0;


		case WM_CLOSE: {
			OnClose ();
			
		}
		//We can Break or do something else
		case WM_DESTROY: {
			OnDestroy ();
		}return 0;


		default: 
			return WndProc (Window, message, wParam, lParam);
		
		}


	}

}