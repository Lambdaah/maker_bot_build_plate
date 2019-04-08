#pragma once

#include <utility>      // std::pair

//We are assuming all these objects are bounding boxes (2d boxs) being either a square or a rectangle and defined simply by a width and a height

class objects
{
private:
	int height;
	int width; 
	int area;
	

public:
	objects();
	
	objects(int, int); //Given two numbers, we will set the height and width as well as all the other private data

	~objects();

	//Private member access functions
	int getArea(); //Simply return the area 
	std::pair<int,int> getDimensions(); //Returns a pair value of width/height
	int getHeight();
	int getWidth();

	void setHeight(int);
	void setWidth(int);


	//Overload comparsion
	inline friend bool operator< (const objects& lhs, const objects& rhs) {
		return lhs.area < rhs.area;
	}


};

