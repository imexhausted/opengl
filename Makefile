cpp = g++
out = final
libs = -lGL -lglfw -lGLEW
sources = ./src/main.cpp

compile:
	$(cpp) $(sources) $(libs) -o $(out)
	clear
clear:
	rm $(out)
	clear

crun:
	$(cpp) $(sources) $(libs) -o $(out)
	./$(out)
	rm $(out)
	clear

deps:
	sudo apt-get install g++ git
	sudo apt-get install libx11-dev libglu1-mesa-dev freeglut3-dev libglew2.2 libglew-dev libglu1-mesa libgl1-mesa-glx libgl1-mesa-dev libglfw3-dev libglfw3