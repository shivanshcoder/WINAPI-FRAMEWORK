#pragma once

//For using Visual Styles!!!!!
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//To be included by user for using framework
#include"Himani/Core.h"
#include"Himani/PredefinedWin.h"
#include"Himani/ControlsWin.h"
#include"Himani/Application.h"




//------------------Entry Point-------------------------

namespace Himani{

	void InitFramework(HINSTANCE Instance, int CmdShow) {
		__ProgramInstance = Instance;
		__ProgramCmdShow = CmdShow;

		///TODO throw if Framework could not be initialzed 
		/*if (0) {
			throw std::exception("FrameWork could not be Initialized!!!");
		}*/
	}

}
//CLEAN
#define AUTO_ENTRY

#ifdef AUTO_ENTRY

extern Himani::HCustomApplication* EntryApplication();

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	Himani::InitFramework(Instance, CmdShow);

	try {
		auto App = EntryApplication();
		App->start();
		delete App;
	}
	catch (Himani::Exceptions &e) {
		MessageBoxW(NULL, e.what(), L"ERROR", MB_ICONERROR);
	}
}
#define ENTRY_APP(APP)  Himani::HCustomApplication* EntryApplication() { return new APP(); }

#endif



//------------------Entry Point-------------------------
