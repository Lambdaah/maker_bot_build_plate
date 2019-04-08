#include <iostream>
#include "plate.cpp"
#include "objects.h"
#include <string>
#include <vector>


int main(int argc, char * argv[]) {
	if(argc < 3){
		std::cerr << "\n\nUsage: ./arrangePlate <dimensions> <collection of objects of form Height,Width >\n";
		std::cerr << "Example: ./arrangePlate 5 3,3 1,1 2,2 \n";
		std::cerr << "The line above will create a build plate for dimensions 5x5 and attempt to insert 3 objects of size 3x3,1x1 and 2x2.\n\n";
		return 0;
	}

	Plate<objects> buildPlate(std::stoi(argv[1]));

	for (int i = 2; i < argc; i++) {
		//Let us assume the form of the objects are H,W, so we look for the , as a delimiter and then convert H and W to ints and insert the objects
		
		std::string line = argv[i];
		int index = line.find(',');
		if (index != std::string::npos) { //Check to see if we found our delimiter
			try {
				int height = std::stoi(line.substr(0, index));
				int width = std::stoi(line.substr(index + 1, line.size()));

				buildPlate.insertObject(objects(height, width));
			}
			catch (...) {
				std::cerr << "Could not parse line: " << line << std::endl;
				continue;
			}
		}
		else { //Couldn't parse, send an error and skip this line
			std::cerr << "Could not parse line: " << line << std::endl;
			continue;
		}

	}



	buildPlate.placeObjects();
	buildPlate.printPlate();
	buildPlate.printPlacementList();


}