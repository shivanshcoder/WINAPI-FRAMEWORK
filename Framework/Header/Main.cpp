#include"MainWindow.h"
//using namespace WINAPIPP;

class HelloWin : WINAPIPP::MainWindow {
public:

    WPARAM start() {
        InitClass();
		Create(TEXT("MainWindow"), TEXT("The Hello Program"),
			WS_OVERLAPPEDWINDOW,
			WINAPIPP::Rectangle(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),
			BaseWin());

		CheckError();

		return Run();

    }
    LRESULT MessageFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rect;

        switch (message) {

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);

			DrawText(hdc, TEXT("Hello World!"), -1, &rect,
				DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			EndPaint(hwnd, &ps);
			CheckError();
			return 0;
        }

		case WM_DESTROY:
			PostQuitMessage(0);
			CheckError();
			return 0;


        }
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

};

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::Instance = Instance;
	WINAPIPP::CmdShow = CmdShow;

	HelloWin mainWindow;
	return mainWindow.start();

}
//START_WINDOW(HelloWin);
