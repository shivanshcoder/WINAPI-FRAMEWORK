#include"WinProc.hpp"
#include<utility>
#include<memory>
#include<string>


struct WindowClassEx :tagWNDCLASSEXW{
	inline WindowClassEx (LPCWSTR ClassName) {
		style = CS_HREDRAW | CS_VREDRAW;
		lpfnWndProc = nullptr;
		cbClsExtra = 0;
		cbWndExtra = 0;
		hInstance = GetModuleHandle (NULL);;
		hCursor = LoadCursor (NULL, IDC_ARROW);
		hIcon = LoadIcon (NULL, IDI_APPLICATION);
		hIconSm = NULL;
		hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
		lpszMenuName = nullptr;
		lpszClassName = ClassName;
		cbSize = sizeof (WNDCLASSEX);
	}

	inline ATOM Register () {
		ATOM Val;
		if (Val = RegisterClassEx (this))
			return Val;
		CheckError ();
		return 0;
		//else
		// Maybe throw exception
			//throw 0;
	}

	inline void AttachProc (WNDPROC Proc) {
		lpfnWndProc = Proc;
		
	}
};

namespace WINAPIPP {
	class BaseWin :public BaseWinProc {

	public:
		virtual inline int MessageLoop ();
		virtual inline int Run ();


		virtual inline void Idle () {

		}

		void Create (std::wstring WindowName);

	private:

		std::wstring ClassName;

		ATOM GenerateDefaultClass ();

		WindowClassEx * WinClass;

		//Represents if the class was registered or not
		bool ClassRegistered;

		HWND Window;
	};

	//The message loop which handles and dispatches WindowsMessages
	int BaseWin::MessageLoop () {
		bool Running = true;
		MSG message;
		while (Running) {

			while (PeekMessage (&message, 0, 0, 0, PM_REMOVE)) {
				if (message.message == WM_DESTROY)
					Running = false;

				TranslateMessage (&message);
				DispatchMessage (&message);
			}

			Idle ();
		}
		return message.wParam;
	}



	int BaseWin::Run () {
		if (!ClassRegistered)
			GenerateDefaultClass ();
		ShowWindow (Window, SW_SHOWNORMAL);
		UpdateWindow (Window);
		return (int)MessageLoop ();

	}
}