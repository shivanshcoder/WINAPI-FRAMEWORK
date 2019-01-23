#pragma once
#include"Hpch.h"

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


	//TODO make HandleWrapperClass for all types of handles
	template<class HandleType>
	class HHandleWrappersClass {


	private:
		HandleType Handle;
	};



#pragma region ErrorHandling


	/*------------------------------------Error Handling------------------------------------*/
	class Exceptions {

	public:

		Exceptions(const HString &data = L"Unknown Error") :Data(data) {}
		
		Exceptions(int LineNumber, const HString FilePath, const HString &data = L"Unknown Error");

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


#define S(x) #x
#define S_(x) S(x)
#define __S_LINE__ S_(__LINE__)

#define CheckDefaultWinError	if (GetLastError())	throw Himani::WinExceptions(__LINE__,TEXT(__FILE__))


#define DISABLE_CLASS_COPY_ASSIGNMENT(ClassName) \
	ClassName(const ClassName&) = delete;\
	ClassName& operator=(const ClassName&) = delete;


	/*------------------------------------Error Handling------------------------------------*/

#pragma endregion

}