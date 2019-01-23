#include "Hpch.h"
#include "Menu.h"
namespace Himani {
	int HMenu::callback(int ID) {
		std::wstringstream ss;
		ss << ID << L"Was pressed";
		MessageBox(NULL, ss.str().c_str(), TEXT("TEMO"), MB_OK);
		return 0;
	}
}
