#pragma once
#include<Windows.h>
#include<vector>

namespace Helpers {




	//TODO make enums for system and local window class name strings
	/*enum SystemClasses {
		MainWindow,
		Static
	};*/



	class Point {
	public:
		Point()
			:x(0), y(0) {}

		Point(int _x, int _y)
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

	typedef Point Pair;
	

	class Rect {

	public:
		Rect() {
			SetRectEmpty(&rect);
		}

		Rect(int xLeft, int yTop, int xRight, int yBottom) {
			SetRect(&rect, xLeft, yTop, xRight, yBottom);
		}


		static Rect Random(
			unsigned long long xlimit , unsigned long long ylimit) {
			return Rect(
				rand() % xlimit,rand() % ylimit,
				rand() % xlimit,rand() % ylimit
			);
		}

		/*---------------Wrappers-------------------*/

		void Offset(int dx, int dy) {
			OffsetRect(&rect, dx, dy);
		}

		void Inflate(int dx, int dy) {
			InflateRect(&rect, dx, dy);
		}

		Rect Intersection(Rect otherRect) const {
			Rect temp;
			IntersectRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		Rect Union(Rect otherRect) const {
			Rect temp;
			UnionRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		bool IsEmpty() {
			IsRectEmpty(&rect);
		}

		bool PointIn(Point p) {
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


	
}

