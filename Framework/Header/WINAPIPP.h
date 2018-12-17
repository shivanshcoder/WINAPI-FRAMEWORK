#pragma once

//To be included by user for using framework
#include"Application.h"




//------------------Entry Point-------------------------

#ifdef AUTO_ENTRY

extern WINAPIPP::CustomApplication* EntryApplication();

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::__ProgramInstance = Instance;
	WINAPIPP::__ProgramCmdShow = CmdShow;

	auto App = EntryApplication();
	App->start();
	delete App;
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
