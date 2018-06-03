#pragma once
#include"Header/Helper.hpp"
#include<memory>


class Text {
	Text (std::shared_ptr<Basic_DC>DeviceContext) :DC (DeviceContext) {}
	
private:
	std::shared_ptr<Basic_DC>DC;
};