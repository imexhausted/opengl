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