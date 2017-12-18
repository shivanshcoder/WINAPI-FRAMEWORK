#include"Headers\WindowClass.h"
#include"Headers\Shapes.h"
#define ID_SMALLER 1
#define ID_LARGER 2
#define BTN_WIDTH	( 8 * cxChar )
#define BTN_HEIGHT	( 4 * cyChar )

void Triangle ( HDC hdc, POINT pt[] );

class _WndProc :public WndProcs<_WndProc> {
public:
	_WndProc()
		:WndProcs(this){}
	int WndProc ()override;
private:
	int cx, cy;
	LPDRAWITEMSTRUCT pdis;
	POINT            pt[3];
	shapes::Rectangle rc;
	WindowBase Wndsmaller, WndLarger;
	static int cyChar, cxChar;
};

int _WndProc::cyChar = 0;
int _WndProc::cxChar = 0;

int _WndProc::WndProc () {
	//CheckError ();
	switch( message ) {
	case WM_CREATE:
		cxChar = LOWORD ( GetDialogBaseUnits () );
		cyChar = HIWORD ( GetDialogBaseUnits () );
		
		Wndsmaller.Createwindow ( L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		{ 0,0 }, { BTN_WIDTH, BTN_HEIGHT }, hwnd, ( HMENU ) ID_SMALLER );

		WndLarger.Createwindow ( L"button", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		{ 0,0 }, { BTN_WIDTH, BTN_HEIGHT }, hwnd, ( HMENU ) ID_LARGER );
		CheckError ();
		return 0;
	case WM_SIZE:
		UpdateSize ();

		Wndsmaller.MoveWindow ( { size.x / 2 - 3 * BTN_WIDTH / 2, size.y / 2 - BTN_HEIGHT / 2 }, { BTN_WIDTH,BTN_HEIGHT } );
		WndLarger.MoveWindow ( { size.x / 2 + BTN_WIDTH / 2, size.y / 2 - BTN_HEIGHT / 2 }, { BTN_WIDTH,BTN_HEIGHT } );
		CheckError ();
		return 0;

	case WM_COMMAND:
		//GetWindowRect ( hwnd, &rc );
		rc.GetWindowRect ( hwnd );
		switch( wParam ) {
		case ID_SMALLER:
			rc.ScaleDown ( 20 );
			break;
		case ID_LARGER:
			rc.ScaleUp ( 20 );
			break;
		}
		MoveWindow (hwnd,  rc.left (),rc.top () ,  rc.right () - rc.left (),rc.bottom () - rc.top (),true  );
		CheckError ();
		return 0;
	case WM_DRAWITEM:
		pdis = ( LPDRAWITEMSTRUCT ) lParam;

		// Fill area with white and frame it black

		FillRect ( pdis->hDC, &pdis->rcItem,
			( HBRUSH ) GetStockObject ( WHITE_BRUSH ) );

		FrameRect ( pdis->hDC, &pdis->rcItem,
			( HBRUSH ) GetStockObject ( BLACK_BRUSH ) );

		// Draw inward and outward black triangles

		cx = pdis->rcItem.right - pdis->rcItem.left;
		cy = pdis->rcItem.bottom - pdis->rcItem.top;

		switch( pdis->CtlID ) {
		case ID_SMALLER:
			pt[0].x = 3 * cx / 8;  pt[0].y = 1 * cy / 8;
			pt[1].x = 5 * cx / 8;  pt[1].y = 1 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 3 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 7 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 7 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 5 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 5 * cx / 8;  pt[0].y = 7 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 7 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 5 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 1 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 1 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 3 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle ( pdis->hDC, pt );
			break;

		case ID_LARGER:
			pt[0].x = 5 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 1 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 5 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8;  pt[1].y = 3 * cy / 8;
			pt[2].x = 7 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 3 * cx / 8;  pt[0].y = 5 * cy / 8;
			pt[1].x = 5 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 4 * cx / 8;  pt[2].y = 7 * cy / 8;

			Triangle ( pdis->hDC, pt );

			pt[0].x = 3 * cx / 8;  pt[0].y = 3 * cy / 8;
			pt[1].x = 3 * cx / 8;  pt[1].y = 5 * cy / 8;
			pt[2].x = 1 * cx / 8;  pt[2].y = 4 * cy / 8;

			Triangle ( pdis->hDC, pt );
			break;
		}

		// Invert the rectangle if the button is selected

		if( pdis->itemState & ODS_SELECTED )
			InvertRect ( pdis->hDC, &pdis->rcItem );

		// Draw a focus rectangle if the button has the focus

		if( pdis->itemState & ODS_FOCUS ) {
			pdis->rcItem.left += cx / 16;
			pdis->rcItem.top += cy / 16;
			pdis->rcItem.right -= cx / 16;
			pdis->rcItem.bottom -= cy / 16;

			DrawFocusRect ( pdis->hDC, &pdis->rcItem );
		}
		CheckError ();
		return 0;

	case WM_DESTROY:
		PostQuitMessage ( 0 );
		return 0;
	}
	return DefWindowProc ( hwnd, message, wParam, lParam );
}

class MainWindow :public WindowBase {
public:
	
	MainWindow ( std::wstring *_ClassName ) :WindowBase ( _ClassName ) {
		_WindowClass->ChangeMenuName ( L"MainWindow" );
		_WindowClass->AttachWndProc ( WindowProc() );
		_WindowClass->RegisterClass ();
	}
private:
	std::wstring ClassName;
	_WndProc WindowProc;
};

int MAIN () {
	std::wstring AppName = L"MainWindow";
	MainWindow Window ( &AppName );
	Window.Createwindow ( AppName, std::wstring ( L"Owner-Draw Button Demo" ) );
	return Window.Run ();
	/*Window.ShowWindow ( 1 );
	Window.UpdateWindow ();
	return Window.ProcessMessage ();*/
	
}

void Triangle ( HDC hdc, POINT pt[] ) {
	SelectObject ( hdc, GetStockObject ( BLACK_BRUSH ) );
	Polygon ( hdc, pt, 3 );
	SelectObject ( hdc, GetStockObject ( WHITE_BRUSH ) );
}