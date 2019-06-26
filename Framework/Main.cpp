
#include"Hpch.h"
#define AUTO_APP_ENTRY
#include"Himani.h"
#include"Himani/Himani/PredefinedClasses.h"
#include"Himani/Himani/SampleWinClasses.h"
#include"Himani/Log System/Log.h"
#include"Himani/Himani/FileManagement.h"
#include"resource.h"

int AskAboutSave(Himani::HWindow* ptr, Himani::HString Title = TEXT("untitled")) {
	Title = TEXT("Save current changes in ") + Title + TEXT("?");
	int returnVal = MessageBox(ptr->Handle(), Title.c_str(), TEXT("PopPad"), MB_YESNOCANCEL | MB_ICONQUESTION);

	return returnVal;
}


#pragma region PopFile

static OPENFILENAME ofn;

void FileInitialize(HWND hwnd) {
	static const TCHAR Filter[] = {
		TEXT("Text Files (*.TXT)\0*.txt\0")
		TEXT("ASCII Files (*.ASC)\0*.asc\0")
		TEXT("All Files (*.*)\0*.*\0\0")
	};
	OPENFILENAME ofn = {};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = Filter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = TEXT("txt");
	::ofn = ofn;
}

int FileOpenDlg(HWND hwnd, PTSTR FileName, PTSTR TitleName) {
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = FileName;
	ofn.lpstrFileTitle = TitleName;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	return GetOpenFileName(&ofn);
}

int FileSaveDlg(HWND hwnd, PTSTR FileName, PTSTR TitleName) {
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = FileName;
	ofn.lpstrFileTitle = TitleName;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	return GetSaveFileName(&ofn);
}

bool FileRead(HWND hwndEdit, PTSTR FileName) {
	HANDLE File;
	if (INVALID_HANDLE_VALUE ==
		(File = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL))) 
		//If fails to open file
		return false;

	int FileLength = GetFileSize(File, NULL);
	
}

#pragma endregion


#pragma region main


class ColorDialog :public Himani::HBaseDialog {
public:
	ColorDialog(HWindow& parent) :HBaseDialog(parent, MAKEINTRESOURCE(IDD_DIALOG1)),
		red(*this, IDC_SCROLLBAR1, 255), green(*this, IDC_SCROLLBAR2, 255), blue(*this, IDC_SCROLLBAR3, 255) {
		CheckDefaultWinError;
		Show(SW_NORMAL);
	}
	BOOL MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override {
		switch (message) {
		case WM_VSCROLL:
			DeleteObject(
				(HGDIOBJ)SetClassLongPtr(Parent.Handle(), GCLP_HBRBACKGROUND,
				(LONG_PTR)CreateSolidBrush(
					RGB(red.GetVal(), green.GetVal(), blue.GetVal())
				)));
			Parent.InvalidateClient(TRUE);
			return 0;
		}
		return HBaseDialog::MessageFunc(message, wParam, lParam);
	}

private:

	Himani::HScrollBar red, blue, green;
};

class MainWin :public Himani::HSimpleWindow {
public:
	MainWin() :HSimpleWindow(TEXT("Color Scroll"), WS_OVERLAPPEDWINDOW), TypingWin(TEXT("edit"), *this, Helpers::HRect(0, 0)) {

		Show(SW_NORMAL);
		SetClassLongPtr(Handle(), GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(0));
	}
	void setText(PTSTR text) {
		SetWindowText(TypingWin.Handle(), text);
	}
	LRESULT MessageFunc(UINT message, WPARAM wParam, LPARAM lParam)override {

		if (WM_DESTROY == message) {
			DeleteObject(
				(HGDIOBJ)SetClassLongPtr(Handle(), GCLP_HBRBACKGROUND,
				(LONG_PTR)GetStockObject(WHITE_BRUSH))
			);
			PostQuitMessage(0);
			return 0;
		}
		return HSimpleWindow::MessageFunc(message, wParam, lParam);
	}

	~MainWin() {}
private:
	Himani::HEdit <
		WS_HSCROLL | WS_VSCROLL | WS_BORDER | ES_LEFT |
		ES_MULTILINE | ES_NOHIDESEL | ES_AUTOHSCROLL | ES_AUTOHSCROLL
	>TypingWin;
};

class MainApp :public Himani::HBaseApp {
public:
	MainApp() {
		{

			Himani::HString name = TEXT("temp.txt");
			Himani::HBytes temp = Himani::ReadTextFile(name);
			OutputDebugStringW((LPCWSTR)temp.GetPtr());
			//HANDLE h = CreateFile(TEXT("temp.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			//auto size = GetFileSize(h, NULL);
			//DWORD re;
			//PBYTE ptr = new BYTE[10000];
			//ReadFile(h, ptr, 5, &re, NULL);
			//uint32_t* p = (uint32_t*)ptr;
			//*p = 0xFFFE0000;
			//bool t = (((*p) & 0x00FFFFFF) == 0xBFBBEF);
			//if(t){
			//	__debugbreak();;
			//}
			////SetFilePointer(h, 0, NULL, FILE_BEGIN);
			////ReadFile(h, ptr+5, 5, &re, NULL);
			//int c =3;
			//Himani::TextFileRead fr(&name[0]);


		}
	}
	MainWin win;
};
ENTRY_APP(MainApp);

#pragma endregion