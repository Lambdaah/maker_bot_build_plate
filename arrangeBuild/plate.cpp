#pragma once
#include <vector> // std::vector
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <iostream> 
#include <stdlib.h>     /* abs */
#include <unordered_map> 
#include <cmath> //sqrt,pow
 
struct point {
	int x;
	int y;
};


template <class T>
class Plate
{
private:
	int **buildPlate;  //Our 2d array
	int size; //User's input size, array is going to be of size, size x size
	point center ; //The center of the build plate
	std::vector<T> objectList; //List of all the objects we need to place
	std::unordered_map<int, std::pair<point, T>> placementList;


	//Functions:
	bool addToPlate(point&, T&,int); //Adds an object to the build Plate
	void rotatePlate(); //Rotate the entire plate 
	bool updatePoint(point&,int); //Updates the point at which we can try to insert a new object
	bool canPlace(point&, int, int); //Given a starting point, and the size of the object, returns if we can place the object
	void populatePlacementList();

	//Brute force function
	void bruteForce(T&,int);

	//Bound finders, these simply find the first row/col of our current figure
	int findBottomBound();
	int findTopBound();
	int findRightBound();
	int findLeftBound();
	

public:
	Plate();
	Plate(int); // Takes in a size N for our buildPlate size
	~Plate();
	
	//GETTERS
	int getSize(); 
	std::vector<T> getObjects();  //Returns the objectList

	void insertObject(T); //Insert a new object 
	void placeObjects(); //Actaully builds the Plate

	void printPlate(); //Prints the current plate
	void printPlacementList(); //Prints the placementList
};



template<class T>
 bool Plate<T>::addToPlate( point& startPoint, T& obj,int id)
{	
	 int x = startPoint.x;
	 int y = startPoint.y;
	 //Check if we go out of bounds, if we do return false indicating we cannot place
		 if (x + obj.getHeight() > size || x < 0 || y + obj.getWidth() > size || y < 0) {
			 //Rotate the object and check the bounds again
			 int temp = obj.getWidth();
			 obj.setWidth(obj.getHeight());
			 obj.setHeight(temp);
			 if (x + obj.getHeight() > size || x < 0 || y + obj.getWidth() > size || y < 0) {
				 rotatePlate();
				 updatePoint(startPoint, findBottomBound());
				 return false; //This time we return false
			 }
		 }
		 int bottomBound;
		 if (id == 1) { //Speical check for the first object
			 bottomBound = obj.getHeight();
		 }
		 else {
			 bottomBound = findBottomBound();
		 }

		 if (!canPlace(startPoint, obj.getHeight(), obj.getWidth()))
			 return false;
	 
	 //Startpoint is a starting cor point of (x,y)/(height,width)
	 for (int i = 0; i < obj.getHeight();i++) {
		 for (int j = 0; j < obj.getWidth();j++) {
			 buildPlate[x][y] = id;
			 y++;
		 }
		 x++;
		 y = startPoint.y;
	 }
	 //Update the start pointer
	 startPoint.x = x;
	 startPoint.y = y;
	 return updatePoint(startPoint,bottomBound);
}


 template<class T>
 bool Plate<T>::updatePoint(point &p,int bottomBound)
 {
	 bool canStillPlace = false;
	 for (int i = 0; i < 4; i++) { //Our limit is 4, if we rotate 4 times and cannot find a result, we cannot place any more objects using this method
		 if (p.y >= size || p.x > bottomBound || p.x >= size) {	//If our y is out of bounds or if it's bigger than our bottom bound, then we MUST rotate
			 rotatePlate();
			 //Update the point
			 p.y = findRightBound(); //Start at the right bound
			 p.x = findTopBound(); //Set x to the top aka 0
			 //There's two posibilities, first is that we are on a non-empty space
			 if (buildPlate[p.x][p.y] != 0) { 

				 //Go until we find a non-empty space
				 while (p.x < size && buildPlate[p.x][p.y] == 0) {
					p.x++;
				 }

				 while (p.x < size && buildPlate[p.x][p.y] != 0) {  //Move down rows until we find a empty position
					p.x++;
				 }
				 if (p.x >= size) { //Reset to the topbound and simply move to the right one, this should place us to the next point
					 p.x = findTopBound();
					 p.y++;
				 }
				 else {
					 while (p.y > 0 && buildPlate[p.x][p.y - 1] == 0) { //Move left till we hit the right of a non-empty position
						 p.y--;
					 }
				 }
			 }
			 else { //The other is if we are on a empty space
				 if (p.y < size) //Move one to the right if possible
					 p.y++;
				 while (p.y < size && buildPlate[p.x][p.y - 1] == 0) {
					 p.x++;
				 }
			 }
			
			 bottomBound = findBottomBound(); //Update bottomBound, it should have changed after a rotate

		 }
		 else {
			 canStillPlace = true;
			 break;//If the condition isn't met, get out
		 }
	 }
	 if (!canStillPlace) {
		 return false;
	 }
	 
	 return true;
 }

 template<class T>
 bool Plate<T>::canPlace(point & pt, int height, int width)
 {
	 int x = pt.x;
	 int y = pt.y;
	 for (int i = 0; i < height; i++) {
		 for (int j = 0; j < width; j++) {
			 if (buildPlate[x][y] != 0) {
				 return false;
			 }
			 y++;
		 }
		 x++;
		 y = pt.y;
	 }
	 return true;
 }

 template<class T>
 void Plate<T>::populatePlacementList()
 {
	 int id;
	 //Go through the entire plate, and log each object
	 for (int row = 0; row < size; row++) {
		 for (int col = 0; col < size; col++) {
			 id = buildPlate[row][col];
			 if (id != 0 && placementList.find(id-1) == placementList.end()) { //Check to see if its a empty field AND if we havn't inserted it into our table yet
				 point currentPoint = { row,col }; //This should be the starting point of this object if we havn't seen it before
				 std::pair<point, T> insertPair = { currentPoint, objectList[id - 1] };
				 placementList[id - 1] = insertPair;
			 }
		 }
	 }
 }

 template<class T>
 void Plate<T>::printPlacementList()
 {
	 typename std::unordered_map<int, std::pair<point, T>>::iterator it = placementList.begin();
	 for (; it != placementList.end(); it++) {
		 std::cout << "ID:" << it->first+1 << " | Starting Point: " << it->second.first.x << "," << it->second.first.y << " | Size: " << it->second.second.getHeight() << " by " << it->second.second.getWidth() << std::endl;
	 }
 }



template<class T>
void Plate<T>::placeObjects()
{
	//The plan is to place the biggest item/object on the center, then build around that biggest object in a 
	if (!objectList.size()) { //Handle case where we have no objects
		std::cerr << "Cannot place objects given no objects!";
		return;
	}

	int idCount = 1; //How we will be displaying the objects

	T currentObject = objectList[0];

	//If we reach here, we know we have at least one object in the list, which should be the biggest object, thus we center the biggest object.

	point curPoint; //Set the current point to the starting point of the object we just placed


	//The starting point will be different depending on the size of the build plate
	if (size % 2) { //Check if odd
		curPoint.x = center.x - ((currentObject.getHeight()) / 2);
		curPoint.y = center.y - ((currentObject.getWidth()) / 2);
	}
	else {
		curPoint.x = center.x - int(double(currentObject.getHeight()) / 2);
		curPoint.y = center.y - int(double(currentObject.getWidth()) / 2);
	}


	std::pair<point, T> newObject; //Create a new pair object that will hold the start pointer + the object itself
	newObject.first = curPoint;
	newObject.second = currentObject;

	addToPlate(newObject.first, newObject.second, idCount++);  //Place our first object in the center, this should always work assuming the object fits the plate


	//We want to start on the first row, and the last column + 1 to place our new object
	curPoint.y = curPoint.y + newObject.second.getWidth() ;

	std::vector<int> bruteForceList; //Holds the list of objects we couldn't place using the spiral method

	for (int i = 1; i < objectList.size(); i++) { //Go through the list and try to apply the spiral method
		currentObject = objectList[i];
		if (!addToPlate(curPoint, currentObject, i+1)) { //If we were unable to place, add it to our brute force list
			bruteForceList.push_back(i);
		}
	}

	for (int i = 0; i < bruteForceList.size(); i++) { //Try to brute force the rest of the objects
		bruteForce(objectList[bruteForceList[i]],bruteForceList[i]);
	}


	populatePlacementList();//We are done adding objects, populate our placementlist for output

	if (placementList.size() != objectList.size()) {
		std::cerr << "WARNING: Not all objects could be placed!" << std::endl;
	}
}


template<class T>
void Plate<T>::bruteForce(T & obj,int id)
{

	double best = size * size; //The manhattan distance to beat
	point bestPoint; //The point where we can place the best object
	//Init these values to -1, check to see if we can place
	bestPoint.x = -1; 
	bestPoint.y = -1;
	point dimensions;

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {

			if (row + obj.getHeight() < size && col + obj.getWidth() < size) { //Check to see if we will be able to place it
				point currentPoint = { row,col };
				if (canPlace(currentPoint, obj.getHeight(), obj.getWidth())) { //Ensure that there isn't another object within the range
					//Get the distance from the center of the board
					double xCenter = row + double(double(obj.getHeight()) / double(2)), yCenter = col + double(double(obj.getWidth()) / double(2));
					double distance = sqrt(pow(center.x - xCenter, 2) + pow(center.y - yCenter, 2) * 1.0);
					if (distance < best) { //Update the best if we need to
						best = distance;
						bestPoint = currentPoint;
						dimensions = { obj.getHeight(),obj.getWidth() };
					}
				}
			}
			//Flip the object and see if we can do better
			int temp = obj.getWidth();
			obj.setWidth(obj.getHeight());
			obj.setHeight(temp);
			//Do the exact same as above
			if (row + obj.getHeight() < size && col + obj.getWidth() < size) { 
				point currentPoint = { row,col };
				if (canPlace(currentPoint, obj.getHeight(), obj.getWidth())) { 
					double xCenter = row + double(double(obj.getHeight()) / double(2)), yCenter = col + double(double(obj.getWidth()) / double(2));
					double distance = sqrt(pow(center.x - xCenter, 2) + pow(center.y - yCenter, 2) * 1.0);
					if (distance < best) {
						best = distance;
						bestPoint = currentPoint;
						dimensions = { obj.getHeight(),obj.getWidth() };
					}
				}
				
			}
		}
	}

	//Place the object on the plate
	int x = bestPoint.x, y = bestPoint.y;
	if (x >= 0 && y >= 0) {
		for (int i = 0; i < dimensions.x; i++) {
			for (int j = 0; j < dimensions.y; j++) {
				buildPlate[x][y] = id + 1;
				y++;
			}
			x++;
			y = bestPoint.y;
		}
	}
	
	

}


template<class T>
void Plate<T>::printPlate()
{
	std::cout << std::endl; //Add a space for nice formatting
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << " "<< buildPlate[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<class T>
int Plate<T>::findBottomBound()
{
	for (int row = size - 1; row > -1; row--) {
		for (int col = 0; col < size; col++) {
			if (buildPlate[row][col] != 0) {
				return row;
			}
		}
	}
	return -1;
}

template<class T>
int Plate<T>::findTopBound()
{
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (buildPlate[row][col] != 0) {
				return row;
			}
		}
	}
	return -1;
}

template<class T>
int Plate<T>::findRightBound()
{
	for (int col = size - 1; col > -1; col--) {
		for (int row = 0; row < size; row++) {
			if (buildPlate[row][col] != 0)
				return col;
		}
	}
	return -1;
}

template<class T>
int Plate<T>::findLeftBound()
{
	for (int col = 0; col < size; col++) {
		for (int row = 0; row < size; row++) {
			if (buildPlate[row][col] != 0)
				return col;
		}
	}
	return -1;
}

template<class T>
Plate<T>::Plate()
{
}

template<class T>
Plate<T>::Plate(int _size)
{
	size = _size;
	buildPlate = new int*[_size];

	for (int row = 0; row < size; row++) {
		buildPlate[row] = new int[size];
		for (int col = 0; col < size; col++) {
			buildPlate[row][col] = 0; //Init each value to zero
		}
	}

	center = { _size / 2,_size / 2 };

}


template<class T>
Plate<T>::~Plate()
{
	for (int i = 0; i < size; i++) {
		delete buildPlate[i];
	}

	delete buildPlate;
}

template<class T>
int Plate<T>::getSize()
{
	return size;
}

template<class T>
void Plate<T>::insertObject(T o)
{
	objectList.push_back(o);
	std::make_heap(objectList.begin(), objectList.end()); //Order the objects from biggest to smallests
}

template<class T>
std::vector<T> Plate<T>::getObjects()
{
	return objectList;
}


template<class T>
void Plate<T>::rotatePlate()
{
	//Does a 90 degree rotation of the entire 2d array
	for (int i = 0; i < size / 2; i++) {
		for (int j = i; j < size - i - 1; j++) {
			int temp = buildPlate[i][j];
			buildPlate[i][j] = buildPlate[j][size - 1 - i];
			buildPlate[j][size - 1 - i] = buildPlate[size - 1 - i][size - 1 - j];
			buildPlate[size - 1 - i][size - 1 - j] = buildPlate[size - 1 - j][i];
			buildPlate[size - 1 - j][i] = temp;
		}
	}
}