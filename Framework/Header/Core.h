#pragma once
#include<Windows.h>
#include<string>
namespace WINAPIPP {


	HINSTANCE __ProgramInstance;
	int __ProgramCmdShow;

	HINSTANCE Instance() {
		return __ProgramInstance;
	}

	int CmdShow() {
		return __ProgramCmdShow;
	}

	namespace Exceptions {

	class WinClass : public std::exception {

	};

	}

}