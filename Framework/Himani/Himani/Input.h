#pragma once
#include"Core.h"

namespace Himani{

	template<class T>
	class Keyboard {

	public:
		Keyboard(T &inst) :Instance(&inst) {}
		int Process(UINT message, WPARAM wParam, LPARAM lParam) { return Keys[message](wParam, lParam); }

		void AttachKey(int KeyVal, std::function<int(WPARAM, LPARAM)>func) {
			Keys[KeyVal] = func;
		}
	private:
		static std::unordered_map<int,std::function<int(WPARAM, LPARAM)>>Keys;
		T *Instance;
		

	};
	template<class T>
	std::unordered_map<int, std::function<int(WPARAM, LPARAM)>> Keyboard<T>::Keys;


	class KeyboardStream {
	public:


	private:
		std::queue<int>Keys;
	};	

	class Mouse {

	};
}