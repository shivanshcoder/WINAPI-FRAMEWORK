#define INCLUDE_CPPS
#include"Himani.h"
#include"resource.h"

#define CF_TCHAR CF_UNICODETEXT
const TCHAR DefaultText[] = TEXT("Defalt Text - UNICODE Version");
const TCHAR Caption[] = TEXT("Clipboard Text Transfers - Unicode Version");

class TempClass :public Himani::HDialogProc {
public:
	TempClass() {
		tempProc = Procedure();
	}


	virtual BOOL MessageFunc(HWND _hDlg, UINT message, WPARAM wParam, LPARAM lParam) override {
		if (message == WM_INITDIALOG) {
		__debugbreak();
			CheckRadioButton(_hDlg, IDC_BLACK, IDC_WHITE, IDC_BLUE);
			CheckRadioButton(_hDlg, IDC_RECTANGLE, IDC_ELLIPSE, IDC_RECTANGLE);
			CheckTempError;
		}
		return FALSE;
	}


	DLGPROC tempProc;
};



int iCurrentColor = IDC_BLACK,
iCurrentFigure = IDC_RECTANGLE;

void PaintWindowOLD(HWND hwnd, int iColor, int iFigure)
{
	static COLORREF crColor[8] = { RGB(0,   0, 0), RGB(0,   0, 255),
								   RGB(0, 255, 0), RGB(0, 255, 255),
								   RGB(255,   0, 0), RGB(255,   0, 255),
								   RGB(255, 255, 0), RGB(255, 255, 255) };

	HBRUSH          hBrush;
	HDC             hdc;
	RECT            rect;

	hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);
	hBrush = CreateSolidBrush(crColor[iColor - IDC_BLACK]);
	hBrush = (HBRUSH)SelectObject(hdc, hBrush);

	if (iFigure == IDC_RECTANGLE)
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	else
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);

	DeleteObject(SelectObject(hdc, hBrush));
	ReleaseDC(hwnd, hdc);
}

void PaintTheBlock(HWND hCtrl, int iColor, int iFigure)
{
	InvalidateRect(hCtrl, NULL, TRUE);
	UpdateWindow(hCtrl);
	PaintWindowOLD(hCtrl, iColor, iFigure);
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	static HWND hCtrlBlock;
	static int  iColor, iFigure;

	switch (message)
	{
	case WM_INITDIALOG: {
		iColor = iCurrentColor;
		iFigure = iCurrentFigure;
		auto L1 = GetLastError();
		CheckTempError;
		int a = CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, iColor);
		int b= CheckRadioButton(hDlg, IDC_RECTANGLE, IDC_ELLIPSE, iFigure);
		auto L2 = GetLastError();
		CheckTempError;
		hCtrlBlock = GetDlgItem(hDlg, IDC_PAINT);

		SetFocus(GetDlgItem(hDlg, iColor)); 
	//	TempClass c;
	//	c.tempProc(hDlg, 1, 2, 3);
	}
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			iCurrentColor = iColor;
			iCurrentFigure = iFigure;
			EndDialog(hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog(hDlg, FALSE);
			return TRUE;

		case IDC_BLACK:
		case IDC_RED:
		case IDC_GREEN:
		case IDC_YELLOW:
		case IDC_BLUE:
		case IDC_MAGNETA:
		case IDC_CYAN:
		case IDC_WHITE:
			iColor = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, LOWORD(wParam));
			PaintTheBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;

		case IDC_RECTANGLE:
		case IDC_ELLIPSE:
			iFigure = LOWORD(wParam);
			CheckRadioButton(hDlg, IDC_RECTANGLE, IDC_ELLIPSE, LOWORD(wParam));
			PaintTheBlock(hCtrlBlock, iColor, iFigure);
			return TRUE;
		}
		break;

	case WM_PAINT:
		PaintTheBlock(hCtrlBlock, iColor, iFigure);
		break;
	}
	return FALSE;
}


//TODO  make it HBaseWin Compatible
void PaintWindow(HWND Window, int Color, int Figure) {
	COLORREF Colors[8] = {
		RGB(0,0,0), RGB(0,0,255),
		RGB(0,255,0), RGB(0,255,255),
		RGB(255,0,0), RGB(255,0,255),
		RGB(255,255,0), RGB(255,255,255)
	};

	//CheckTempError;
	Himani::QuickDC dc(Window);
	RECT rect;
	//CheckTempError;

	GetClientRect(Window, &rect);


	Himani::HBrush Brush(Colors[Color - IDC_BLACK]);
	//CheckTempError;
	dc.Attach(Brush);
	//CheckTempError;
	//CheckTempError;

	if (Figure == IDC_RECTANGLE)
		Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
	else
		Ellipse(dc, rect.left, rect.top, rect.right, rect.bottom);

}



class MyDialogBox :public Himani::HModalDialog {

public:
	MyDialogBox(Himani::HBaseWin& parent,int oldColor, int oldFigure)
		:HModalDialog(parent, IDD_DIALOG1),selectedColor(oldColor), selectedFigure(oldFigure){
	//	StartDialog();
		DialogBox(Himani::Instance(), MAKEINTRESOURCE(IDD_DIALOG1), parent, Procedure());
	}


	void OnInit()override {
		CheckRadioButton(IDC_BLACK, IDC_WHITE, selectedColor);
		CheckRadioButton(IDC_RECTANGLE, IDC_ELLIPSE, selectedFigure);

		::SetFocus(GetDlgItem(hDlg, selectedColor));
		PaintBlock();
		ctrlBlock = GetDlgItem(hDlg, IDC_PAINT);
	}

	void PaintBlock() {
		//CheckDefaultWinError;
		UpdateWindow(ctrlBlock);
		InvalidateRect(ctrlBlock, NULL, true);
		
		PaintWindow(ctrlBlock, selectedColor, selectedFigure);
	}

	void OnCommand(HWND _hDlg, int ID)override {
		try {
			switch (ID) {
			case IDC_BLACK:
			case IDC_RED:
			case IDC_GREEN:
			case IDC_YELLOW:
			case IDC_BLUE:
			case IDC_MAGNETA:
			case IDC_CYAN:
			case IDC_WHITE:
				selectedColor = ID; //LOWORD(wParam);
				CheckRadioButton( IDC_BLACK, IDC_WHITE, selectedColor);
				PaintBlock();
				break;

			case IDC_RECTANGLE:
			case IDC_ELLIPSE:
				selectedFigure = ID;

				CheckRadioButton(IDC_RECTANGLE, IDC_ELLIPSE, selectedFigure);
				PaintBlock();
				break;
			case IDOK:
			case IDCANCEL:
				EndDialog(TRUE);
			}
		}
		catch(Himani::Exceptions & e) {
			MessageBoxW(NULL, e.what(), L"ERROR", MB_ICONERROR);
		}
	}

	int selectedColor = IDC_BLACK;
	int selectedFigure = IDC_RECTANGLE;
	HWND ctrlBlock;
};

class ClipText :public Himani::HCustomApplication {
public:
	int iCurrentColor = IDC_BLUE;
	int iCurrentFigure = IDC_ELLIPSE;


	//IconDemo():HCustomApplication(L"IconDemo",WS_OVERLAPPEDWINDOW) {}

	ClipText() 
	/*:Menu(*this)*/{
		CreateWin(L"Temp", WS_OVERLAPPEDWINDOW, Helpers::HRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT),LoadMenu(Himani::Instance(), MAKEINTRESOURCE(IDR_MENU1)));
		//SetMenu(Menu.menu);
	}

	CLASS_PROPERTIES(ClipText,CS_VREDRAW|CS_HREDRAW,NULL)

	int OnSize(WPARAM wParam, LPARAM lParam) {
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		cxIcon = GetSystemMetrics(SM_CXICON);
		cyIcon = GetSystemMetrics(SM_CYICON);
		
		return 0;
	}

	int OnPaint() {
		{
			Himani::PaintDC dc(*this);
		}
		PaintWindow(*this, iCurrentColor, iCurrentFigure);
		return 0;
	}

	int OnCommand(WPARAM wParam, LPARAM lParam) {
		//Menu was used
	//	if (lParam == NULL)
		//	Menu.callback(LOWORD(wParam));
		//else child sent a message
		switch (LOWORD(wParam)) {
		case ID_HELP_ABOUT: {

			try {
			//	TempClass T;
			//	DialogBox(Himani::Instance(), MAKEINTRESOURCE(IDD_DIALOG1), Handle(), AboutDlgProc);
				//CheckTempError;
				MyDialogBox Box(*this, iCurrentColor, iCurrentFigure);
				iCurrentColor = Box.selectedColor;
				iCurrentFigure = Box.selectedFigure;
				CheckTempError;
			}
			catch (...) {
				MessageBoxW(NULL, L"ERRROR", L"ERROR", MB_ICONERROR);
			}
			
			//InvalidateRect(Helpers::HRect(), true);
			::InvalidateRect(*this, NULL, TRUE);
			
		}
		}
		return 0;
	}
	DECLARE_MESSAGE_MAP();
private:
	int cxClient, cyClient;
	int cxIcon, cyIcon;
	//MyMenu Menu;
};

MESSAGE_MAP_BEGIN(ClipText)
MESSAGE_MAP_ENTRY_PARAMS(OnSize, WM_SIZE)
MESSAGE_MAP_ENTRY(OnPaint, WM_PAINT)
MESSAGE_MAP_ENTRY_PARAMS(OnCommand, WM_COMMAND)
MESSAGE_MAP_ENTRY_SINGLE(PostQuitMessage(0), 0, WM_DESTROY)
MESSAGE_MAP_END(Himani::HCustomApplication)

ENTRY_APP(ClipText)

