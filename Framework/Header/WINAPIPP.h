#pragma once

//To be included by user for using framework
#include"Application.h"
#include"GDI.hpp"




//------------------Entry Point-------------------------

#ifdef AUTO_ENTRY

extern WINAPIPP::Application* EntryApplication();

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::__ProgramInstance = Instance;
	WINAPIPP::__ProgramCmdShow = CmdShow;

	auto App = EntryApplication();
	App->start();
	delete App;
}

#endif


#define ENTRY_APP(APP)  WINAPIPP::Application* EntryApplication() { return new APP(); }


//------------------Entry Point-------------------------
