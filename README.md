# MakeBot Challenge

# Compiling: 


  Simply run the bash script named buildProject which will create 3 object files, compile them and then copy the output file into the root directory.


You could also open up the project in Mircosoft Visual Studios via the sln file.

# Running:
  The program runs completely via command line, the inputs are simply the dimensions of the buildPlate (single number N) and a list of X,Y pairs representing Height,Width.
  
  

  Ex) ./arrangePlate 5 3,3 2,2 1,1 
  
  
  This will create a 5x5 plate and place 3 objects of size (3,3),(2,2), and (1,1)



# Notes:
I am assuming optimial placement refers to placing the objects as close to the center as possible and we would like to have similar quality for each object.

The produced algorithm will first find the largest object and center it with the build plate's center and then start placing objects to it's right hand side if possible in a spiral fashion (something along the lines of https://en.wikipedia.org/wiki/Golden_spiral). The idea is that since the plate is NxN, we should be able to place objects focused on the right hand side and rotate when needed. The spiral method is just an greedy approach to placement, if an object was not able to be placed it will be added to a brute force list where i will find a placement via brute force using the manhanttan distance of the center of the build plate and the center of placing the object at a given point as a heuristic to determine it's best placement. 
