#include"Hpch.h"
#include"Core.h"

namespace Himani {

	HINSTANCE __ProgramInstance; 
	int __ProgramCmdShow;
	int __WinAppStorageIndex = -1;

	Exceptions::Exceptions(int LineNumber, const HString FilePath, const HString & data) {
		Data += L"Line :";
		Data += std::to_wstring(LineNumber);
		Data += L"  ";
		Data += FilePath;
		Data += L"  ";
		Data += data;
	}
	WinExceptions::WinExceptions(int LineNumber, const HString FilePath)
		:Exceptions(L"") {
		wchar_t *buf;
		auto erro  = ::GetLastError();
		std::wstringstream s;
		s << LineNumber;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, erro, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&buf, sizeof(buf), NULL);
		Data += L" Windows Error: ";
		Data += HString(buf);
		Data += s.str();
		__debugbreak();
		//WARNING should it be deleted??
		//delete buf;
	}
}