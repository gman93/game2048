game: src/application.cpp src/gameBoard.cpp
	g++ -o game -lglut -lGL -lGLU -lGLEW src/gameBoard.cpp src/application.cpp -I include
