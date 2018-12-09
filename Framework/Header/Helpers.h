#pragma once
#include<vector>

namespace WINAPIPP {

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
		};
	};
	

	class Rectangle {

	public:
		Rectangle() {
			SetRectEmpty(&rect);
		}

		Rectangle(int xLeft, int yTop, int xRight, int yBottom) {
			SetRect(&rect, xLeft, yTop, xRight, yBottom);
		}


		static Rectangle Random(
			unsigned long long xlimit = MAXULONGLONG, unsigned long long ylimit = MAXULONGLONG) {
			return Rectangle(
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

		Rectangle Intersection(Rectangle otherRect) const {
			Rectangle temp;
			IntersectRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		Rectangle Union(Rectangle otherRect) const {
			Rectangle temp;
			UnionRect(&temp.rect, &rect, &otherRect.rect);
			return temp;
		}

		bool IsEmpty() {
			IsRectEmpty(&rect);
		}

		bool PointIn(Point p) {
			return PtInRect(&rect, p.pt);
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

	class Region {
	public:
		//Elliptic if true makes elliptic region else Rect region is made
		Region(Rectangle rect, bool Elliptic) {
			if (Elliptic)
				region = CreateEllipticRgnIndirect(&rect.rect);
			else
				region = CreateRectRgnIndirect(&rect.rect);
		}

		Region(std::vector<Point>points, int FillMode) {
			region = CreatePolygonRgn(points.begin(),)
		}

	private:
		HRGN region;
	};

}

