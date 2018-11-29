#pragma once


namespace WINAPIPP {

	//TODO make enums for system and local window class name strings
	enum SystemClasses {
		MainWindow,
		Static
	};


	class Rectangle {

	public:
		Rectangle()
			:x{ 0 }, y{ 0 }, height{ 0 }, width{ 0 }{}

		Rectangle(int _x, int _y, int _height, int _width)
			:x(_x), y(_y), height(_height), width(_width) {}


		//TODO make private
		//make functions for making access easier
		// private:
		int x, y;
		int height, width;
	};

	class Point {
	public:
		Point()
			:x(0), y(0) {}

		Point(int _x, int _y)
			:x(_x), y(_y) {} 


		//TODO make private
		//make functions for making access easier
		//private:
		int x, y;
	};
}

