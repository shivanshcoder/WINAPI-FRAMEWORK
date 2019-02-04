#include "Hpch.h"
#include "Log.h"
#include<fstream>

namespace LogSystem {

	void FileLog::Refresh()
	{
		if (!Entries.size())return;

		std::wofstream File;
		File.open(FileName, std::wofstream::app);

		for (int i = 0; i < Entries.size(); ++i) {
			File << Entries[i].Level << L": " << Entries[i].Entry << L"\n";
		}
		Entries.clear();
	}


	WindowLog::WindowLog() {
		LogWindowHandle = CreateWindow(TEXT("static"), TEXT("Log Window"), WS_OVERLAPPEDWINDOW |SW_SHOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
		UpdateWindow(LogWindowHandle);
		ShowWindow(LogWindowHandle, SW_SHOW);
		
		HDC dc = GetDC(LogWindowHandle);

		TextOut

		ReleaseDC(LogWindowHandle,dc);
	}
	void WindowLog::DefaultColors(){
		TextColor[LogLevels::Normal] = RGB(0, 150, 150);
		TextColor[LogLevels::Moderate] = RGB(150, 150, 0);
		TextColor[LogLevels::High] = RGB(150, 0, 0);
	}
}
