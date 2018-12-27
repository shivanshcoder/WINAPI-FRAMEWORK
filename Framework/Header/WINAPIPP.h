#pragma once

//To be included by user for using framework
#include"Application.h"




//------------------Entry Point-------------------------

namespace WINAPIPP {

	void InitFramework(HINSTANCE Instance, int CmdShow) {
		__ProgramInstance = Instance;
		__ProgramCmdShow = CmdShow;

		///TODO throw if Framework could not be initialzed 
		/*if (0) {
			throw std::exception("FrameWork could not be Initialized!!!");
		}*/
	}

}


#ifdef AUTO_ENTRY

extern WINAPIPP::CustomApplication* EntryApplication();

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::InitFramework(Instance, CmdShow);

	try {
		auto App = EntryApplication();
		App->start();
		delete App;
	}
	catch (WINAPIPP::Exceptions &e) {
		MessageBoxW(NULL, e.what(), L"ERROR", MB_ICONERROR);
	}
}
#define ENTRY_APP(APP)  WINAPIPP::CustomApplication* EntryApplication() { return new APP(); }
//#else
//extern void MAIN();
//
//int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
//	WINAPIPP::__ProgramInstance = Instance;
//	WINAPIPP::__ProgramCmdShow = CmdShow;
//
//	MAIN();
//}
//#define ENTRY_FUNC(func) void MAIN(){func();}
#endif



//------------------Entry Point-------------------------
