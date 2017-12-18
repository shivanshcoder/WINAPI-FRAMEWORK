#include"WindowClass.h"
#include"Print.h"
#include"TEXT.H"
#include<string>

/*template<std::wstring ClassName>*/class BaseApp {
protected:
	BaseApp(){

		////_WindowClass = new WindowClass();
		//_WindowClass->RegisterClass(ClassName);
	}
	~BaseApp() {/*
		UnregisterClass(ClassName.c_str());*/
	}
private:
	//Should be removed for using template
	std::wstring ClassName;

};
WindowClass* Window::_WindowClass = new WindowClass();