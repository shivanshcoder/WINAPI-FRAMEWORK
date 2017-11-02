#pragma once 
#include<Windows.h>

//#ifndef WIN32
//#define TEXT(q) q
//#define lstrlen strlen
//#define lstrcpy strcpy
//#define wsprintf sprintf
//#endif

typedef struct tagxy {
	int x;
	int y;
} Position,Size;

template<class T,int size>class strings {
public:
	strings()
		:index{ 0 } {
		s = new T[size];
	}
	~strings() { delete s; /*ReleaseDC(hwnd, hdc);*/ }
	strings& operator<<(int a);
	strings& operator<<(T *a);
	strings& operator<<(float f);
	T* operator<<(strings *z);
	strings& operator<<(long double a);
	strings& operator<<(double a);
	strings& operator<<(T a);
////	strings& operator()(HDC _hdc, Position _pos);
//
//#ifdef WIN32
//	//HDC hdc;
//#endif
//	//HWND hwnd;
//	//Position pos;
	T *s;
	int index;
};

template<class T,int size>
strings<T, size>& strings<T, size>::operator<< (int a) {
	index += wsprintf(&s[index], TEXT("%d"), a);
	return *this;
}
template<class T, int size>
strings<T, size>& strings<T, size>::operator<< (float a) {
	index += wsprintf(&s[index], TEXT("%f"), a);
	return *this;
}
template<class T, int size>
strings<T, size>& strings<T, size>::operator<< (long double a) {
	index += wsprintf(&s[index], TEXT("%Lf"), a);
	return *this;
}
template<class T, int size>
strings<T, size>& strings<T, size>::operator<<(double a) {
	index += wsprintf(&s[index], TEXT("%f"), a);
	return *this;
}
template<class T, int size>
strings<T, size>& strings<T, size>::operator<< (T a) {
	s[index] = a;
	index += 1;
	return *this;
}
template<class T, int size>
strings<T, size>& strings<T, size>::operator<< (T *a) {
	lstrcpy(&s[index], a);
	index = lstrlen(s);
	return *this;
}
template<class T, int size>
T* strings<T, size>::operator<< (strings *z) {
//#ifdef WIN32
//	//TextOut(hdc, pos.x, pos.y, s, index);
//#else
//	//printf(s);
//#endif
//
	return s;
}

//strings& strings::operator()(HDC _hdc, Position _pos) {
//	hdc = _hdc;
//	pos = _pos;
//	return *this;
//}