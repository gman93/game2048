#include"iostream"
#include"omp.h"
#include<string>
#include<stdint.h>
#include<vector>
#include<stdlib.h>
#include<gameBoard.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<string.h>
#include<sstream>
using namespace std;

	void myInit(){
		glClearColor(1.0,1.0,1.0,0);
		glColor3f(0,0,0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,1.0,0,1.0);
	}
	int ** GameBoard::board=new int*[4];
	int GameBoard::score=0;
	void keyBoardHandler(int key,int x, int y){
		switch(key){
			case GLUT_KEY_UP:
				GameBoard::moveUp();
				GameBoard::updateBoard();
				GameBoard::printBoard();
				break;

			case GLUT_KEY_DOWN:
				GameBoard::moveDown();
				GameBoard::updateBoard();
				GameBoard::printBoard();
				break;
				
			case GLUT_KEY_RIGHT:
				GameBoard::moveRight();
				GameBoard::updateBoard();
				GameBoard::printBoard();
				break;

			case GLUT_KEY_LEFT:
				GameBoard::moveLeft();
				GameBoard::updateBoard();
				GameBoard::printBoard();
				break;
		}
	}

	GameBoard::GameBoard(int argc,char** argv){
		
		
		for(int i=0;i<4;i++){
			board[i]=new int[4];
		}
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				board[i][j]=-1;
			}
		}
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
		glutInitWindowSize(500,500);
		glutInitWindowPosition(100,100);
		glutCreateWindow("2048");
		glutDisplayFunc(GameBoard::printBoard);
		glutSpecialFunc(keyBoardHandler);
		myInit();
		glutMainLoop();

	}
	
	
	

	int GameBoard::checkCanMoveUpDown(){
		int flag=0;
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			
				#pragma omp parallel for
				for(int j=0;j<3;j++){
					if(board[j][i]==board[j+1][i]||board[j][i]==-1||board[j+1][i]){
						flag=1;
					}
				}
		}	
		return flag;
	}

	int GameBoard::checkCanMoveRightLeft(){
		int flag=0;
		#pragma omp parallel for
		for(int i=0;i<4;i++){
				#pragma omp parallel for
				for(int j=0;j<3;j++){
					if(board[i][j]==board[i][j+1]||board[i][j]==-1||board[i][j+1]==-1){
						flag=1;
					}
				}
		}
		return flag;
	}

	
	void GameBoard::printBoard(){
		int r[3]={217,225,202};
		int b[3]={222,120,47};
		int g[3]={0,0,0};	
		char* name="hellow";	
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1,1,1,0);
		

		for (int i=0;i<4;i++){
			for (int j=0;j<4;j++){
			int color=0;
			if(board[i][j]<16){
				
				color=0;
			}else if(board[i][j]<512){
				color=1;
			}else {
				color=2;
			}



			glBegin(GL_POLYGON);
   			//cout<<"the color is :"<<color<<"\n";	

			glColor3d(r[color],b[color],g[color]);
			glVertex2f(0.2*j+0.1,0.2*i+0.1);
			glVertex2f(0.2*j+0.29,0.2*i+0.1);
			glVertex2f(0.2*j+0.29,0.2*i+0.29);
			glVertex2f(0.2*j+0.1,0.2*i+0.29);
			glEnd();
			if(board[i][j]!=-1){
				glColor3d(0,0,0);
				glRasterPos2f(0.2*j+0.2,0.2*i+0.2);
				stringstream ss;
				ss<<board[i][j];
				string num=ss.str();
				for(int i=0;i<num.length();i++)
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,num[i]);
			}
			}
		}

		

		glFlush();




	}
	int GameBoard::checkEndCondition(){
		if(checkCanMoveUpDown()==0&&checkCanMoveRightLeft()==0){
			return 1;
		}else{
			return 0;
		}
	}

	vector<int> GameBoard::freeCells(){
		vector<int> cells;
		
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			#pragma omp parallel for
			for(int j=0;j<4;j++){
				
				if(board[i][j]==-1){
					cells.push_back(i*4+j);
				}
			}
		}
		
		return cells;
	}

	void GameBoard::updateBoard(){
			cout<<"updating board\n";
			vector<int> a=freeCells();
			int limit=a.size();
			cout<<limit<<"\n";
			int rnd=(int)rand()%limit;
			int spawn_location=a[rnd];

			board[spawn_location/4][spawn_location%4]=2;
			cout<<"update done\n";
	}


	void GameBoard::moveUp(){
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			#pragma omp parallel for
			for(int j=0;j<3;j++){
				#pragma omp parallel for
				for(int k=j+1;k<4;k++){
					
					if(board[j][i]==board[k][i]&&board[j][i]!=-1){
						board[j][i]=2*board[j][i];
						score+=2*board[j][i];
						board[k][i]=-1;
					}else if(board[k][i]!=-1){
						j=k;
					}
				}
			}
			#pragma omp parallel for
			for(int j=0;j<4;j++){
				if(board[j][i]==-1){
					#pragma omp parallel for
					for(int k=j+1;k<4;k++)
					{	if(board[k][i]!=-1){
					      		board[j][i]=board[k][i];
							board[k][i]=-1;
							break;
						}
					}
					
				}
			}
		}
	}
					
	void GameBoard::moveDown(){
	
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			#pragma omp parallel for
			for(int j=3;j>0;j--){
				for(int k=j-1;k>-1;k--){
					if(board[j][i]==board[k][i]&&board[j][i]!=-1){
						board[j][i]=2*board[j][i];
						score+=2*board[j][i];
						board[k][i]=-1;
					}else if(board[k][i]!=-1){
						j=k;
					}
				}
			}
			#pragma omp parallel for
			for(int j=3;j>-1;j--){
				if(board[j][i]==-1){
					#pragma omp parallel for
					for(int k=j-1;k>-1;k--)
					{	if(board[k][i]!=-1){
					      		board[j][i]=board[k][i];
							board[k][i]=-1;
							break;
						}
					}
					
				}
			}
		}
		cout<<"moveDown done\n";
	}

	void GameBoard::moveRight(){
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			#pragma omp parallel for
			for(int j=3;j>0;j--){
				for(int k=j-1;k>-1;k--){
					if(board[i][j]==board[i][k]&&board[i][j]!=-1){
						board[i][j]=2*board[i][j];
						score+=2*board[i][j];
						board[i][k]=-1;
					}else if(board[i][k]!=-1){
						j=k;
					}
				}
			}
			#pragma omp parallel for
			for(int j=3;j>-1;j--){
				if(board[i][j]==-1){
					#pragma omp parallel for
					for(int k=j-1;k>-1;k--)
					{	if(board[i][k]!=-1){
					      		board[i][j]=board[i][k];
							board[i][k]=-1;
							break;
						}
					}
				
				}
			}
		}
	}

	void GameBoard::moveLeft(){
		#pragma omp parallel for
		for(int i=0;i<4;i++){
			#pragma omp parallel for
			for(int j=0;j<3;j++){
				for(int k=j+1;k<4;k++){
					if(board[i][j]==board[i][k]&&board[i][j]!=-1){
						board[i][j]=2*board[i][j];
						score+=2*board[i][j];
						board[i][k]=-1;
					}else if(board[i][k]!=-1){
						j=k;
					}
				}
			}
			#pragma omp parallel for
			for(int j=0;j<4;j++){
				if(board[i][j]==-1){
					#pragma omp parallel for 
					for(int k=j+1;k<4;k++)
					{	if(board[i][k]!=-1){
					      		board[i][j]=board[i][k];
							board[i][k]=-1;
							break;
						}
					}
				}
			}
		}
	}


