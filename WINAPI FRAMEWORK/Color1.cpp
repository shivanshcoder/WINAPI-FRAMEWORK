#include"Headers\WindowClass.h"
#include"Headers\Brush.h"
#include"Headers\Shapes.h"
#include"Headers\WndProc.h"
#include"Headers\Scroll.h"

class MainWindowProcs;
class Scroll;


//class Scroll :public scroll::ScrollCTL {
//public:
//
//};


class MainWindowProcs :public AdvWndProcs<MainWindowProcs> {
public:
	MainWindowProcs():AdvWndProcs(this){}
	LONG_PTR AdvWndProc ()override { return ::DefWindowProc ( hwnd, message, wParam, lParam ); }
private:


}MainWindowProc;

int MAIN () {
	int a = sizeof ( scroll::ScrollWndCTL );
	int b = sizeof ( SCROLLINFO );
	WindowBase MainWindow( L"MainWindow" );
	MainWindow.AttachWinProcedure ( MainWindowProc () );
	MainWindow.Createwindow ( L"MainWindow", L"Colors" );
	MainWindow.Run ();
}