#include<vector>
#include<stdint.h>


using namespace std;
class GameBoard{

public:
	static	int **board;
	static int score;
	static int end;


	
	GameBoard(int, char**);
	static void moveLeft();
	static void moveRight();
	static void moveUp();
	static void moveDown();
	static void printBoard();
	static void updateBoard();
	static int checkEndCondition();
	static int checkCanMoveUpDown();
	static int checkCanMoveRightLeft();
	static vector<int> freeCells();

};



