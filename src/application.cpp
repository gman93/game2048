#include<gameBoard.h>
#include<iostream>
using namespace std;

int main(int argc, char** argv){
	
	

	GameBoard* x=new GameBoard(argc,argv);
	
	/*x.printBoard();
  	while(1){
		int ran;
		cin>>ran;
		if(ran==0){
			cout<<"move up\n ";
			x.moveUp();
		}else if(ran==1){
			cout<<"move Down\n";
			x.moveDown();
		}else if(ran==2){
			cout<<"move right\n";
			x.moveRight();
		}else if(ran==3){
			cout<<"'move left\n";
			x.moveLeft();
		}	
		x.printBoard();
		x.updateBoard();
	}*/	
}
