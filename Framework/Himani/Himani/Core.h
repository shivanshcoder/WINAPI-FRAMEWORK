#pragma once
#include"Hpch.h"

/*
	Tasklist Tags!
	URGENT
	TODO
	WARNING
	VERSION
*/


#pragma region CUSTOM_CLASS_MACROS
//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

#define WINCLASS_PROPERTIES(__ClassName, Style) inline static Himani::HWinClassProperties __Prop = { TEXT( #__ClassName ), Himani::CommonWndProc<__ClassName>,(Style)};\
	Himani::HString& ClassName()override {		\
		return __Prop.ClassName;			\
	}											\
friend LRESULT CALLBACK Himani::CommonWndProc<__ClassName>(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#pragma endregion


#pragma region ErrorMacros

#define S(x) #x
#define S_(x) S(x)
#define __S_LINE__ S_(__LINE__)

#define CheckDefaultWinError	if (GetLastError())	throw Himani::WinExceptions(__LINE__,TEXT(__FILE__))

	//Delete or make something else
#define CheckTempError if(GetLastError()){\
	wchar_t *buf;	\
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,\
		NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
		(LPWSTR)& buf, sizeof(buf), NULL);\
	__debugbreak();\
	}

#define DISABLE_CLASS_COPY_ASSIGNMENT(ClassName) \
	ClassName(const ClassName&) = delete;\
	ClassName& operator=(const ClassName&) = delete;


#pragma endregion


namespace Himani {


#ifdef UNICODE
	typedef std::wstring HString;
#else
	typedef std::string HString;
#endif



	extern HINSTANCE __ProgramInstance;
	extern int __ProgramCmdShow;

	inline HINSTANCE Instance() {
		return __ProgramInstance;
	}
	inline int CmdShow() {
		return __ProgramCmdShow;
	}

	//Class storing all the 
	struct Framework {

		std::vector<HString>ClassNameList;
	};


#pragma region ErrorHandling


	/*------------------------------------Error Handling------------------------------------*/
	class Exceptions {

	public:

		Exceptions(const HString& data = TEXT("Unknown Error")) :Data(data) {}

		Exceptions(int LineNumber, const HString FilePath, const HString& data = TEXT("Unknown Error"));

		const wchar_t* what() {
			return Data.c_str();
		}

	protected:
		HString Data;
	};

	class WinExceptions :public Exceptions {
	public:
		WinExceptions(int LineNumber, const HString FilePath);

	};



	/*------------------------------------Error Handling------------------------------------*/

#pragma endregion



	//TODO make HandleWrapperClass for all types of handles
	template<class HandleType>
	class HHandleWrapperBaseClass {
	public:
		HHandleWrapperBaseClass() {
			handle = (nullptr);
		}

		explicit HHandleWrapperBaseClass(HandleType __handle) {
			handle = __handle;
		}

		HandleType Handle()const {
			if (handle)
				return handle;
		//	else
			//	throw Exceptions(TEXT("Handle yet not Initialized"));
		}


		explicit operator HandleType() {
			return handle;
		}

		//This should be Protected?
	//protected:
		void InitHandle(HandleType __handle) {
			handle = __handle;
		}

		virtual ~HHandleWrapperBaseClass() {}

	private:
		HandleType handle;
	};

	class HWNDCLASS {
	public:
		HWNDCLASS() {
			handle = (nullptr);
		}

		explicit HWNDCLASS(HWND __handle) {
			handle = __handle;
		}

		HWND Handle()const {
			if (handle)
				return handle;
			//	else
				//	throw Exceptions(TEXT("Handle yet not Initialized"));
		}


		explicit operator HWND() {
			return handle;
		}

		//This should be Protected?
	//protected:
		void InitHandle(HWND __handle) {
			handle = __handle;
		}

		virtual ~HWNDCLASS() {}

	private:
		HWND handle;
	};

	/*
	----------------------- HWCustomWindow Classes Constructor Arguement Helper Class------------------------------
	This is a helper class for Initializing Extra Data members even if class is being constructed using CreateWin!
	HCustomWindow Derived Class Constructors will only take pointer to one of such class object(i.e. User should derive from
	*/
	struct HClassInitializer {
		//friend class HCustomWindow;
		HClassInitializer(HWND wnd=nullptr) :hwnd(wnd) {}
	//private:
		HWND hwnd;
	};


}