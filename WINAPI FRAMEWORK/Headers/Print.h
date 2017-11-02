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

template<class T,int _size>
class strings {
public:
	strings()
		:index{ 0 } {
		s = new T[_size];
	}
	~strings() { delete s; }

	T* operator()(int i = 1);
	T* operator<<(strings *z);
	int size() { return index; }
	void clear() { index = 0; }
	strings& operator<<(int a);
	strings& operator<<(T *a);
	strings& operator<<(float f);
	strings& operator<<(long double a);
	strings& operator<<(double a);
	strings& operator<<(T a);
private:
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
}\

template<class T, int size>
T* strings<T, size>::operator<< (strings *z) {
//#ifdef WIN32
//	//TextOut(hdc, pos.x, pos.y, s, index);
//#else
//	//printf(s);
//#endif
//
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