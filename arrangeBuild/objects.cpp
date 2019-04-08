#include "objects.h"

objects::objects() //Given no information, we assume that we were given a invalid or uselss object
{
}

objects::objects(int _height, int _width) { 
	width = _width;
	height = _height;
	area = _width * _height;
}


objects::~objects() //No pointers or heap modifications so we don't really need to do anything for the destructer
{
}


//GETTERS
int objects::getArea() {
	return area;
}

std::pair<int, int> objects::getDimensions() {
	return { width,height };
}

int objects::getHeight()
{
	return height;
}

int objects::getWidth()
{
	return width;
}

void objects::setHeight(int newHeight)
{
	height = newHeight;
}

void objects::setWidth(int newWidth)
{
	width = newWidth;
}
