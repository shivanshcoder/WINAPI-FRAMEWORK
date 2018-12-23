#pragma once
#include<Windows.h>
#include<unordered_map>

namespace WINAPIPP {

	class Keyboard {

		int Handle(UINT message, WPARAM wParam, LPARAM lParam);

	};

	int WINAPIPP::Keyboard::Handle(UINT message, WPARAM wParam, LPARAM lParam){
		return 0;
	}
	class Mouse {

	};
}