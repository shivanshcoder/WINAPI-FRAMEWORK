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