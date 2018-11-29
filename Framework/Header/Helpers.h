#pragma once


namespace WINAPIPP {

    enum SystemClasses {
        MainWindow,
        Static
    };

    class WString {

    public:
        WString(enum SystemClasses ClassName) {

        }
    };

    class Rectangle {

    public:
        Rectangle()
            :x{ 0 }, y{ 0 }, height{ 0 }, width{ 0 }{}

        Rectangle(int _x, int _y, int _height, int _width)
            :x(_x), y(_y), height(_height), width(_width) {}


   // private:
        int x, y;
        int height, width;
    };
}

