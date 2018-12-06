#pragma once

//To be included by user for using framework
#include"MainWindow.h"
#include"GDI.hpp"


//------------------Entry Point-------------------------

#ifdef AUTO_ENTRY

extern WINAPIPP::MainWindow* EntryApplication();

int WINAPI WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CmdLine, int CmdShow) {
	WINAPIPP::Instance = Instance;
	WINAPIPP::CmdShow = CmdShow;

	auto App = EntryApplication();
	App->start();
	delete App;
}

#endif


#define ENTRY_APP(APP)  WINAPIPP::MainWindow* EntryApplication() { return new APP(); }


//------------------Entry Point-------------------------