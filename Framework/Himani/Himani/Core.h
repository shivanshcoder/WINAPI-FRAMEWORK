#pragma once
#include"Hpch.h"



#pragma region CUSTOM_CLASS_MACROS
//Function for Getting class Name
#define DEFINE_CLASSNAME(ClassName__) LPCWSTR ClassName()override { return L###ClassName__; } 

#define OVERRIDE_PREDEFINEDCLASS(ClassName__) DEFINE_CLASSNAME(ClassName__)

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, IconSm, Cursor, Background, MenuName)	DEFINE_CLASSNAME(ClassName__)	\
 bool __ClassProp() override {		\
		static bool __ValidClass = Himani::RegisterWinClass(Style, Himani::StaticWndProc , Icon, IconSm, Cursor, Background, MenuName, ClassName());	\
		return __ValidClass;\
}

#define APLLICATION_PROPERTIES(ClassName__, Style, Icon, Cursor) CLASS_ALL_PROPERTIES(ClassName__, Style, Icon, NULL, Cursor, (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), NULL)

//Defines WNDCLASS properties for each UserDefined Class
#define CLASS_PROPERTIES(ClassName__, Style) CLASS_ALL_PROPERTIES(ClassName__, Style, (LoadIcon(NULL, IDI_APPLICATION)), NULL, (LoadCursor(NULL, IDC_ARROW)),(HBRUSH)GetStockObject(WHITE_BRUSH), NULL)


#define DEFINE_WIN_CLASS(ClassName__, Style, MenuName)															\
static LRESULT CALLBACK ClassName##StaticWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){		\
		StaticWndProc(hwnd,message,wParam,lParam);\
	}\
static void __RegisterWinClass(){																		\
\
	}

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

		Exceptions(const HString &data = TEXT("Unknown Error")) :Data(data) {}
		
		Exceptions(int LineNumber, const HString FilePath, const HString &data = TEXT("Unknown Error"));

		const wchar_t * what() {
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
			else
				throw Exceptions(TEXT("Invalid Handle Being Returned!"));
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



}