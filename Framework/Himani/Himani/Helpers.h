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

	class Grid {
	public:
		Grid(HRect GridSize) :gridsize(GridSize) {
			grid.resize(1);
		}

	private:
		struct HorzRow {
			HorzRow(std::vector<HRect>& __grid1D, int height) :
				blockHeight(blockHeight), grid1D(__grid1D) {}

			int blockHeight;
			std::vector<HRect>& grid1D;
		};

		class ProxyGrid {
		public:
			ProxyGrid(std::vector<HRect>& __grid1D, int height) :grid1D(__grid1D), blockHeight(height) {}

			HRect operator[](int index) {
				//Is resizing even needed?
				if (grid1D.size() <= index) {
					grid1D.resize(index);
				}
				return grid1D[index];
			}

			HRect GetPlace(int width, int height) {
				HRect rect = HRect(0, 0, width, height);
				grid1D.push_back(rect);

			}

			int blockHeight;
			std::vector<HRect>& grid1D;
		};

	public:
		ProxyGrid operator[](int index) {
			if (index == 0)
				return ProxyGrid(grid[index], 0);

			if (grid.size() <= index) {
				grid.push_back(std::vector<HRect>());
			}
			return ProxyGrid(grid[index], grid[index - 1][0].bottom);
		}

		void AddHorzBlock(int height) {
			int last_height = grid.back()[0].bottom;
			grid.push_back({ HRect(0, last_height, 0, height + last_height) });
		}

	private:
		std::vector< std::vector< HRect > >grid;
		HRect gridsize;
	};

}

