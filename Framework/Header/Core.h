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


#pragma region ErrorHandling


	/*------------------------------------Error Handling------------------------------------*/
	class Exceptions {

	public:

		Exceptions(const std::wstring &data = L"Unknown Error") :Data(data) {}

		const wchar_t * what() {
			return Data.c_str();
		}
	protected:
		std::wstring Data;
	};

	class WinExceptions :public Exceptions {
	public:


		WinExceptions(const std::wstring &data = L"")
			:Exceptions(data) {
			wchar_t *buf;
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&buf, sizeof(buf), NULL);
			Data += L" Windows Error: ";
			Data += std::wstring(buf);
		}

	};

	//throws Exceptions if GetLastError throws error
	inline void CheckWinError() {
		if (GetLastError())
			throw WinExceptions();
	}

	/*------------------------------------Error Handling------------------------------------*/

#pragma endregion

}