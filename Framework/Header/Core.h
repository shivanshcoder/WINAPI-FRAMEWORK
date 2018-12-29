#pragma once
#include<Windows.h>
#include<string>	


namespace HIMANI{


	HINSTANCE __ProgramInstance;
	int __ProgramCmdShow;

	HINSTANCE Instance() {
		return __ProgramInstance;
	}

	int CmdShow() {
		return __ProgramCmdShow;
	}


#pragma region ErrorHandling


	/*------------------------------------Error Handling------------------------------------*/
	class Exceptions {

	public:

		Exceptions(const std::wstring &data = L"Unknown Error") :Data(data) {}
		Exceptions(int LineNumber, const std::wstring FilePath, const std::wstring &data = L"Unknown Error") {
			Data += L"Line :";
			Data += std::to_wstring(LineNumber);
			Data += L"  ";
			Data += FilePath;
			Data += L"  ";
			Data += data;
		}
		const wchar_t * what() {
			return Data.c_str();
		}
	protected:
		std::wstring Data;
	};

	class WinExceptions :public Exceptions {
	public:
		WinExceptions(int LineNumber, const std::wstring FilePath)
			:Exceptions(L"") {
			wchar_t *buf;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&buf, sizeof(buf), NULL);
			Data += L" Windows Error: ";
			Data += std::wstring(buf);
		}

	};


#define S(x) #x
#define S_(x) S(x)
#define __S_LINE__ S_(__LINE__)

#define CheckDefaultWinError	if (GetLastError())	throw HIMANI::WinExceptions(__LINE__,TEXT(__FILE__))

	/*------------------------------------Error Handling------------------------------------*/

#pragma endregion

}