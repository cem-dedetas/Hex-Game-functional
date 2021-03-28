#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define ARR_SIZE 144

using namespace std;



void play_game(char  board[][12], int b_size);
int get_move(char * letter, int * number, int &b_size, int &turn, char  board[][12], int ispvp);
void print_board(char board[][12], int b_size);
int init_board(char board[][12]);
void arrayFiller(int arr[][2], int size);
bool winCheck(char board[][12], int bsize, int trail[][2],int * trailIndex, int xPos, int yPos, bool forX);
bool isIncluded(int arr[][2], int x , int y, int size);
void helper(char board[][12], int arr[][2],int size, char a);
void saveBoard(char board[][12],string filename,int &size,const int &isPvp, int turn);
void loadBoard(char board[][12], string filename, int &size, int &isPvp, int &turn);
void compMoves(char board[][12],char * x, int * y , int size = 12);

enum move_type { xTile = 'x', oTile = 'o', emptyTile = '.', winX = 'X', winO = 'O'};

int main()
{
	int b_size;
	char board[12][12];

	
	b_size = init_board(board);
    cout << "X(left-to-right) vs.  O(top-to-bottom)" << endl;
	play_game(board, b_size);

	return 0;
}


/*Main gameplay loop*/
void play_game(char  board[][12], int b_size){

	char letter;
	int number, moves = 0, index = 0;
	bool stop = false;
	int trail[ARR_SIZE][2];
	int pvp= -1;

	arrayFiller(trail, ARR_SIZE);

	while(!(pvp == 0 || pvp == 1)){
		cout << "Vs. Player(0) or Vs. Comp.(1) (Please input 1 or 0):";
		cin >> pvp;
		if(!(pvp == 0 || pvp == 1)){
			cerr << "\nInvalid Game Type!!"<< endl;
			cout <<pvp<< "\n(Please input 1 or 0):"<<endl;
		}
	}

	print_board(board, b_size);

	while(!stop){
        int temp;
		temp = get_move(&letter, &number, b_size, moves, board,pvp);

		while (temp==0){
			cerr << endl << letter << number << "<- this move is invalid.";
			cout << " \n Try again:" << endl;
			temp = get_move(&letter, &number, b_size, moves, board,pvp);
		}
			 
		if (temp == 1){
			if(board[letter-'a'][number-1] == emptyTile){
				if(moves % 2 == 0) board[letter-'a'][number-1] = xTile;
            	else board[letter-'a'][number-1] = oTile;
    
				++moves; 
			}

			else cout <<  "Inavlid move!! (there is an \'" << board[letter-'a'][number-1] << "\' there) try again." << endl;
	
			if(moves >= (b_size*b_size)) {
				stop = true;
			}
			print_board(board, b_size);

        	for (int i = 0; i < b_size; i++)
        	{
			

        	    if(board[0][i] == xTile)
        	        if(winCheck(board,b_size, trail, &index, 0, i, true)){
						stop = true;
        	        	helper(board, trail, index, winX);
						print_board(board,b_size);
					}
					arrayFiller(trail, ARR_SIZE);
					
        	        index = 0;
        	    if(board[i][0] == oTile)
        	        if(winCheck(board,b_size, trail, &index, i, 0, false)){
						stop = true;
        	        	helper(board, trail, index, winO);
						print_board(board,b_size);

					}
        	        arrayFiller(trail, ARR_SIZE);
        	        index = 0;
        	}
        	arrayFiller(trail, ARR_SIZE);
		}

		else if(temp == 2){
			cout<<endl<<"SAVED SUCCESFULY"<<endl;
			print_board(board, b_size);
		}
		else if(temp == 3){
			cout<<endl<<"LOADED SUCCESFULY"<<endl;
			print_board(board, b_size);
		}
        

	}

	return;
}


/*get move from user and procces it to be used in elsewhere*/
int get_move(char * letter, int * number, int &b_size, int &turn, char  board[][12], int ispvp){

	string sTurn = (turn % 2 == 0)? "X's turn:" : "O's turn:";
	string temp0, temp1;
	cout << sTurn;

	if((ispvp==1) && ((turn % 2 == 0)==1)){
		compMoves(board, letter , number ,b_size);
		return 1;
	}

	cin >>temp0 >> temp1;
	
	if(temp0 == "SAVE"){
		saveBoard(board, temp1, b_size, 0, turn);
		return 2;
	}
	else if(temp0 == "LOAD"){
		loadBoard(board, temp1, b_size, ispvp, turn);
		return 3;	
	}
	else {
		stringstream toInt(temp1);
		toInt >> *number;
		*letter = temp0[0];

		if((*letter >= 'A' && *letter <= 'A' + b_size)) letter -= (int)('a'-'A');

		return ((*letter >= 'a' && *letter <= 'a' + b_size) && (*number >= 1 && *number <= b_size));
	}
	
	
}


/*print board in correct format*/
void print_board(char board[][12], int b_size){
	
    
    
    for(int a = 'a'; a< ('a'+b_size); ++a)
		cout << " " << (char)a ;
    cout<<endl;
	for (int i = 0; i < b_size; ++i)
	{
		cout << i+1;
		for (int k = 0; k <= i ; ++k)
			cout << " ";
		
		for (int  j = 0; j < b_size; ++j)
		{
			cout << board[j][i] ;
			cout << " ";
		}
		cout << endl;
	}
	return;
}


/*fill the board with '.' */
int init_board(char board[][12]){
	int size = -1;
	cout << "What shall be the size of the board (0 - 12):";
	cin >> size;
	while(size < 0 || size > 13){
		cout << "\nSize should be between 0 and 12. Your entry :"<< size << "\nTry again :";
		cin >> size;
	}

	for (int i = 0; i < 12; ++i)
	{
		for (int  j = 0; j < 12; ++j)
		{
			board[i][j] = emptyTile;
		}
	}

	return size;
}

/*Main game logic: when an edge tile is placed , checks a winning path recursively. While in recursive function, 
* holds a list of previously visited tiles so it doesnt go in an infinite loop.
*/
bool winCheck(char board[][12], int bsize, int trail[][2],int * trailIndex, int xPos, int yPos, bool forX){
	trail[*trailIndex][0] = xPos;
    trail[*trailIndex][1] = yPos;
    (*trailIndex)++;
    bool isDone = false;

    if(forX && xPos==bsize-1){
        cout << "\n'X' WON!!"<<endl;
        return true;
    }
    if(!forX && yPos == bsize-1){
        cout<< "\n'O' WON!!"<<endl;
        return true;
    }

    if(!isIncluded(trail, xPos-1, yPos, *trailIndex) && ((forX && board[xPos-1][yPos]==xTile) || (!forX && board[xPos-1][yPos]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos-1,yPos, forX);
    }
	if(isDone)return true;
    
    if(!isIncluded(trail, xPos-1, yPos+1, *trailIndex) && ((forX && board[xPos-1][yPos+1]==xTile) || (!forX && board[xPos-1][yPos+1]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos-1,yPos+1, forX);
    }
	if(isDone)return true;
    
    if(!isIncluded(trail, xPos, yPos+1, *trailIndex) && ((forX && board[xPos][yPos+1]==xTile) || (!forX && board[xPos][yPos+1]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos,yPos+1, forX);
    }
	if(isDone)return true;
    
    if(!isIncluded(trail, xPos, yPos-1, *trailIndex) && ((forX && board[xPos][yPos-1]==xTile) || (!forX && board[xPos][yPos-1]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos,yPos-1, forX);
    }
	if(isDone)return true;
    
    if(!isIncluded(trail, xPos+1, yPos, *trailIndex) && ((forX && board[xPos+1][yPos]==xTile) || (!forX && board[xPos+1][yPos]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos+1,yPos, forX);
    }
	if(isDone)return true;
    
    if(!isIncluded(trail, xPos+1, yPos-1, *trailIndex) && ((forX && board[xPos+1][yPos-1]==xTile) || (!forX && board[xPos+1][yPos-1]==oTile))){
        isDone = winCheck(board,bsize,trail ,trailIndex,xPos+1,yPos-1, forX);
    }
    if(isDone)return true;

    return false;
}



/*fills the visited tile array with -1's because array indexes cant be -1 so it acts like empty in comparison*/
void arrayFiller(int arr[][2], int size){

	for (int i = 0; i < size; ++i)
	{
		arr[i][0] = arr[i][1] = -1;
	}
	

	return;
}

bool isIncluded(int arr[][2], int x , int y, int size){
    for(int i = 0; i<size; ++i){
        if(arr[i][0]==x && arr[i][1]==y)return true;
    }
    return false;
}

/*Makes the winning path capital letters*/
void helper(char board[][12], int arr[][2],int size, char a){
    for (int i = 0; i < size; i++)
    {
        board[arr[i][0]][arr[i][1]] = a;
    }
	return;
}

/*Saves the board to ./filename*/
void saveBoard(char board[][12],string filename,int &size,const int &isPvp, int turn){
	fstream myFile;
	myFile.open(filename, ios::out);
	myFile<< size<<" "<<isPvp<<" "<<turn<<" ";
	for(int i=0; i<size; ++i){
		for (int j = 0; j <size; ++j){
			myFile << board[i][j];
		}
	}
	myFile.close();
}
/*Loads the board from ./filename*/
void loadBoard(char board[][12], string filename, int &size, int &isPvp, int &turn){
	fstream myFile;
	char temp;
	myFile.open(filename, ios::in);
	myFile >> size>>isPvp>> turn;
	myFile.get(temp);
	for(int i=0; i<size; ++i){
		for (int j = 0; j <size; ++j){
			myFile.get(temp);
			board[i][j] = temp;
		}
	}
	myFile.close();

}

/*Generate moves for computer to play*/
void compMoves(char board[][12],char * x, int * y , int size){
	struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
    srand((time_t)ts.tv_nsec);
  	int r[2];

  	for(int i =0; i<2; ++i)r[i] = rand()%size;
	auto xRand = r[0];
	decltype(*y) yRand = r[1];


	while(board[xRand][yRand] != emptyTile){
		for(int i =0; i<2; ++i)r[i] = rand()%size;
		xRand = r[0];
		yRand = r[1];
	}
	
	*x = xRand + 'a';
	*y = yRand+1;

	cout << "X:"<< *x;
	cout << "Y:"<< *y<<endl;

	return;
}