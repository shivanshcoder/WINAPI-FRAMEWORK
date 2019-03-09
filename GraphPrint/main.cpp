#define INCLUDE_CPPS
#include"Himani.h"
#include"resource.h"

#define CF_TCHAR CF_UNICODETEXT
const TCHAR DefaultText[] = TEXT("Defalt Text - UNICODE Version");
const TCHAR Caption[] = TEXT("Clipboard Text Transfers - Unicode Version");


//TODO  make it HBaseWin Compatible
void PaintWindow(HWND Window, int Color, int Figure) {
	COLORREF Colors[8] = {
		RGB(0,0,0), RGB(0,0,255),
		RGB(0,255,0), RGB(0,255,255),
		RGB(255,0,0), RGB(255,0,255),
		RGB(255,255,0), RGB(255,255,255)
	};

	Himani::QuickDC dc(Window);
	RECT rect;

	GetClientRect(Window, &rect);


	Himani::HBrush Brush(Colors[Color - IDC_BLACK]);

	dc.Attach(Brush);

	if (Figure == IDC_RECTANGLE)
		Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
	else
		Ellipse(dc, rect.left, rect.top, rect.right, rect.bottom);

}



class MyDialogBox :public Himani::HModalDialog {

public:
	MyDialogBox(Himani::HBaseWin& parent, int oldColor, int oldFigure)
		:HModalDialog(parent, IDD_DIALOG1), selectedColor(oldColor), selectedFigure(oldFigure) {
			StartDialog();
	//	DialogBox(Himani::Instance(), MAKEINTRESOURCE(IDD_DIALOG1), parent, Procedure());
	}


	bool OnInit()override {
		CheckRadioButton(IDC_BLACK, IDC_WHITE, selectedColor);
		CheckRadioButton(IDC_RECTANGLE, IDC_ELLIPSE, selectedFigure);

		ctrlBlock = GetDlgItem(Handle(), IDC_PAINT);
		::SetFocus(GetDlgItem(Handle(), selectedColor));

		return false;
	}

	bool OnPaint()override {
		PaintBlock();
		return false;
	}
	void PaintBlock() {
		::InvalidateRect(ctrlBlock, NULL, true);
		//InvalidateWhole(true);
		UpdateWindow(ctrlBlock);

		PaintWindow(ctrlBlock, selectedColor, selectedFigure);
	}

	void OnCommand(int ID)override {

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
			CheckRadioButton(IDC_BLACK, IDC_WHITE, selectedColor);
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

	int selectedColor = IDC_BLACK;
	int selectedFigure = IDC_RECTANGLE;
	HWND ctrlBlock;
};

class ClipText :public Himani::HCustomApplication {
public:
	int iCurrentColor = IDC_BLUE;
	int iCurrentFigure = IDC_ELLIPSE;


	ClipText() {
		CreateWin(L"Temp", WS_OVERLAPPEDWINDOW, Helpers::HRect(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT), LoadMenu(Himani::Instance(), MAKEINTRESOURCE(IDR_MENU1)));
	}

	CLASS_PROPERTIES(ClipText, CS_VREDRAW | CS_HREDRAW, NULL)

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
		switch (LOWORD(wParam)) {
		case ID_HELP_ABOUT: {

			MyDialogBox Box(*this, iCurrentColor, iCurrentFigure);
			iCurrentColor = Box.selectedColor;
			iCurrentFigure = Box.selectedFigure;


			InvalidateWhole(true);
			//::InvalidateRect(*this, NULL, TRUE);

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

