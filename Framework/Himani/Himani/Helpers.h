#pragma once
#include"Hpch.h"

namespace Helpers {

	class HPoint {
	public:
		HPoint()
			:x(0), y(0) {}

		HPoint(int _x, int _y)
			:x(_x), y(_y) {}

		//TOOO maybe make private??
		//private:

		union {
			POINT pt;

			struct {
				LONG  x;
				LONG  y;
			};

			struct {
				LONG first;
				LONG second;
			};
		};
	};

	typedef HPoint Pair;


	class HRect {

	public:
		HRect() {
			SetRectEmpty(&rect);
		}

		HRect(int xLeft, int yTop, int xRight, int yBottom) {
			SetRect(&rect, xLeft, yTop, xRight, yBottom);
		}

		HRect(int length, int width) {
			SetRect(&rect, 0, 0, length, width);
		}

		static HRect Random(
			unsigned long long xlimit, unsigned long long ylimit) {
			return HRect(
				rand() % xlimit, rand() % ylimit,
				rand() % xlimit, rand() % ylimit
			);
		}

		/*---------------Wrappers-------------------*/

		void Offset(int dx, int dy) {
			OffsetRect(&rect, dx, dy);
		}

		void Inflate(int dx, int dy) {
			InflateRect(&rect, dx, dy);
		}

		HRect Intersection(HRect otherRect) const {
			HRect temp;
			IntersectRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		HRect Union(HRect otherRect) const {
			HRect temp;
			UnionRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		bool IsEmpty() {
			IsRectEmpty(&rect);
		}

		bool PointIn(HPoint p) {
			return PtInRect(&rect, p.pt);
		}

		LONG xLength() {
			return right - left;
		}

		LONG yLength() {
			return bottom - top;
		}

		/*---------------Wrappers-------------------*/


//TOOO maybe make private??
//private:

		union {
			RECT rect;

			//TODO check the union thing
			struct {
				LONG    left;
				LONG    top;
				LONG    right;
				LONG    bottom;
			};

		};
	};

	template<class T, unsigned int Size>
	class HWinArray {
	public:

		template<class... _Types>
		HWinArray(_Types&& ... _Args) {
			for (int i = 0; i < Size; ++i)
				Windows[i] = std::make_unique<T>((_Args)...);

		}

		unsigned int size() {
			return Size;
		}

		T& operator[](unsigned int index)const {
			return *Windows[index];
		}

	public:
		std::unique_ptr<T>Windows[Size];
	};

	

}

