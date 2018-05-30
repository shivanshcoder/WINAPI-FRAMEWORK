#include"Header/BaseWin.hpp"

namespace WINAPIPP {

	//The message loop which handles and dispatches WindowsMessages
	/*int BaseWin::MessageLoop () {
		bool Running = true;
		MSG message;
		while (Running) {

			while (PeekMessage (&message, 0, 0, 0, PM_REMOVE)) {
				if (message.message == WM_DESTROY)
					Running = false;
				
				TranslateMessage (&message)
					;
				DispatchMessage (&message);
			}

			Idle ();
		}
		return message.wParam;
	}
	*/
	void BaseWin::Create (std::wstring WindowName) {
		
		if (!ClassRegistered)
			GenerateDefaultClass ();

		Window = CreateWindowEx (0, ClassName.c_str (), WindowName.c_str (), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle (NULL), NULL);
	}
	ATOM BaseWin::GenerateDefaultClass () {
		ClassRegistered = true;
		ClassName = TEXT("TEMP");
		WinClass = new WindowClassEx (ClassName.c_str ());;
		WinClass->AttachProc (Procedure);
		return WinClass->Register ();
	}

	/*int BaseWin::Run () {
		if (!ClassRegistered)
			GenerateDefaultClass ();
		ShowWindow (Window, SW_SHOWNORMAL);
		UpdateWindow (Window);
		return (int)MessageLoop ();

	}*/

}