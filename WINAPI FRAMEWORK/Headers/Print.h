#pragma once 
#include<Windows.h>

//#ifndef WIN32
//#define TEXT(q) q
//#define lstrlen strlen
//#define lstrcpy strcpy
//#define wsprintf sprintf
//#endif

struct Size {
	int x;
	int y;
} ;
struct Position {
	int x;
	int y;
};

struct temp {
	int x;
};
template<class T,int _size>
class strings {
public:
	strings()
		:index{ 0 } {
		//hdc = nullptr;
		s = new T[_size];
	//	hdc = _hdc;
	}
	~strings() { delete s; }

	T* operator()(int i = 1);
	strings& operator()(temp tp) {
		for (int i = 0; i < tp.x; ++i) {
			s[index++] = ' ';
		}
		return *this;
	}
	T* operator<<(strings *z);
	int size() { return index; }
	void clear() { index = 0; }
	strings& operator<<(int a);
	strings& operator<<(T *a);
	strings& operator<<(float f);
	strings& operator<<(long double a);
	strings& operator<<(double a);
	strings& operator<<(T a);
	strings& operator<<(strings<T, _size> s) { return *this; }
	strings& operator[](Position _pos);
	void finish() {
		TextOut(hdc, pos.x, pos.y, s,size());
		index = 0;
	}
	HDC hdc;
private:
	Position pos;
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
strings<T, size>& strings<T, size>::operator<< (double a) {
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
	s[index] = '\0';
	index = 0;
	return s;
}

template<class T,int size>
T* strings<T, size>::operator()(int i = 1) {
	if (i) {
		index = 0;
	}
	return s;
}

template<class T,int _size>
strings<T,_size>& strings<T,_size>::operator[](Position _pos) {
	if (index != 0) {
		TextOut(hdc, pos.x, pos.y, s, size());
		index = 0;
	}
	pos = _pos;
	return *this;
}

//template<class T,int _size>
//class printstr :public strings<T, _size> {
//public:
//	printstr& operator[](Position _pos);
//private:
//	Position pos;
//};





