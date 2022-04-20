#include<iostream>
#include<cstdlib>
#include<time.h>
using namespace std;

void select_level(); // queries the player for selecting the level
void set_game_board(); // initial game board to be shown (all -)
void set_mine_board(); // decides at which places mines will be placed
void display_game_board(); // shows the current scenario of the game
bool modify_game_board(); // changes the board according to the choice of the player
void number_of_adjacent_mines(); // determines the number of mines adjacent to a cell
void help(int row, int column);
void help2(int row, int column);
bool is_valid(int row, int column); // check if current position is within bounds
void winning_board(); // display the board with all cells if player wins

int NROWS; // number of rows in the board
int NCOLUMNS; // number of columns in the board
int NMINES; // number of mines in the board
int FLAGS; // number of flags the player can place on the board
int NSAFE; // number of cells which are safe, ie, no mines

const int MAX = 40;

int mineBoard[MAX][MAX]; // for keeping track where mines are placed
char gameBoard[MAX][MAX]; // used to display the game board

int PROW; // row number selected by the player
int PCOLUMN; // column number selected by the player
char S_F; // player selected safe or placed a flag

int main()
{
	select_level();
	set_game_board();
	set_mine_board();

	/*
	for(int i = 0; i < NROWS; i++)
	{
		for(int j = 0; j < NCOLUMNS; j++)
		{
			cout<<mineBoard[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl<<endl;
	//*/

	bool winning = true;

	while(winning)
	{
		display_game_board();
		cout<<"Enter your move, (row, column, safe(s)/flag(f)) -> ";
		cin>>PROW>>PCOLUMN>>S_F;
		cout<<endl<<endl;

		winning = modify_game_board();

		if(NMINES == 0)
			break;
		if(NSAFE == 0)
			break;
	}

	if(!winning){
		display_game_board();
		cout<<endl<<"You lost! :("<<endl;	
	}
	else
	{
		cout<<NSAFE<<endl;
		winning_board();
		cout<<endl<<"You won !!!!! :) :) :)"<<endl;
	}
	
	getchar();

	return 0;
}



void select_level()
{
	int level;

	cout<<"Enter the diffculty level\n";
	cout<<"Press 0 for BEGINNER     (9  * 9  cells and 10 mines)"<<"\n"<<
		  "Press 1 for INTERMEDIATE (16 * 16 cells and 40 mines)"<<"\n"<<
		  "Press 2 for ADVANCED     (16 * 30 cells and 99 mines)"<<"\n";

	cin>>level;

	if(level == 0)
		NROWS = 9, NCOLUMNS= 9, NMINES = 10, FLAGS = 10;
	else if(level == 1)
		NROWS = 16, NCOLUMNS = 16, NMINES = 40, FLAGS = 40;
	else
		NROWS = 16, NCOLUMNS = 40, NMINES = 99, FLAGS = 99;

	NSAFE = (NROWS * NCOLUMNS) - NMINES;

	cout<<endl<<endl<<endl;
}


void set_game_board()
{
	for(int i = 0; i < NROWS; i++)
	{
		for(int j = 0; j < NCOLUMNS; j++)
			gameBoard[i][j] = '-';
	}
}


void display_game_board()
{
	cout<<"    ";

	for(int i = 0; i < NCOLUMNS; i++){
		cout<<i<<" ";
		if(i < 10)
			cout<<" ";
	}

	cout<<endl<<endl;

	for(int i = 0; i < NROWS; i++)
	{
		cout<<i<<"  ";
		if(i < 10)
			cout<<" ";
		for(int j = 0; j < NCOLUMNS; j++)
			cout<<gameBoard[i][j]<<"  ";
		cout<<"  "<<i<<"  ";
		if(i < 10)
			cout<<" ";

		cout<<endl;
	}

	cout<<endl;

	cout<<"    ";
	
	for(int i = 0; i < NCOLUMNS; i++){
		cout<<i<<" ";
		if(i < 10)
			cout<<" ";
	}
	cout<<endl<<endl;

	cout<<FLAGS<<" flags left"<<endl<<endl;
}


bool modify_game_board()
{
	// player said its safe where there is mine, game over
	if(S_F == 's' and mineBoard[PROW][PCOLUMN] == 99)
	{
		for(int i = 0; i < NROWS; i++)
		{
			for(int j = 0; j < NCOLUMNS; j++)
			{
				if(gameBoard[i][j] != 'f' and mineBoard[i][j] == 99)
					gameBoard[i][j] = '*';
			}
		}
		return false;
	}
	// remove flag from game board by player
	else if(S_F == 'f' and gameBoard[PROW][PCOLUMN] == 'F')
	{
		gameBoard[PROW][PCOLUMN] = '-';
		FLAGS++;
	}
	// player correctly found mine and placed a flag
	else if(S_F == 'f' and mineBoard[PROW][PCOLUMN] == 99)
	{
		gameBoard[PROW][PCOLUMN] = 'F';
		FLAGS--;
		NMINES--;
	}
	// place a flag, mine does not exist here
	else if(S_F == 'f')
	{
		gameBoard[PROW][PCOLUMN] = 'F';
		FLAGS--;
	}
	// a safe cell, but cell value not 0 so dont reveal other cells
	else if(S_F == 's' and mineBoard[PROW][PCOLUMN] != 0)
	{
		char c = mineBoard[PROW][PCOLUMN] + '0';
		gameBoard[PROW][PCOLUMN] = c;
		NSAFE--;
		//cout<<PROW<<" "<<PCOLUMN<<" "<<"NSAFE "<<NSAFE<<endl;
	}
	// player selected a safe cell, reveal other cells
	else
	{
		help(PROW, PCOLUMN);
	}


	return true;
}


// recursive function to reveal all cells adjoining to 0 cells
void help(int row, int column)
{
	if(!is_valid(row, column) or gameBoard[row][column] != '-' or mineBoard[row][column] == 99)
		return;

	if(mineBoard[row][column] != 0){
		gameBoard[row][column] = '0' + mineBoard[row][column];
		NSAFE--;
		//cout<<row<<" "<<column<<" "<<"NSAFE "<<NSAFE<<endl;
		return;
	}

	for(int i = row-1; i <= row+1; i++)
	{
		for(int j = column-1; j <= column+1; j++)
		{
			if(gameBoard[i][j] != '-')
				continue;
			if(i == row and j == column){
				gameBoard[i][j] = '0';
				NSAFE--;
				//cout<<row<<" "<<column<<" "<<"NSAFE "<<NSAFE<<endl;
				continue;
			}

			help(i, j);
		}
	}
}



void set_mine_board()
{
	int mines = NMINES;
	srand(time(0));

	while(mines--)
	{
		int rand_num = (rand() % (NROWS * NCOLUMNS + 1));
		int row = rand_num / NROWS;
		int column = rand_num % NCOLUMNS;

		if(mineBoard[row][column] == 99)
			mines++;

		mineBoard[row][column] = 99;
		//cout<<mines<<endl;
		//cout<<row<<" "<<column<<endl;
	}

	number_of_adjacent_mines();
}


void number_of_adjacent_mines()
{
	for(int i = 0; i < NROWS; i++)
	{
		for(int j = 0; j < NCOLUMNS; j++)
		{
			if(mineBoard[i][j] == 99)
				continue;

			help2(i, j);
		}
	}
}

void help2(int row, int column)
{
	int count = 0;

	for(int i = row - 1; i <= row + 1; i++)
	{
		for(int j = column - 1; j <= column + 1; j++)
		{
			if(i == row and j == column)
				continue;

			if(!is_valid(i, j))
				continue;

			if(mineBoard[i][j] == 99)
				count++;
		}
	}

	mineBoard[row][column] = count;
}



bool is_valid(int row, int column)
{
	if(row >= 0 and row < NROWS and column >= 0 and column < NCOLUMNS)
		return true;

	return false;
}



void winning_board()
{
	for(int i = 0; i < NROWS; i++)
	{
		for(int j = 0; j < NCOLUMNS; j++)
		{
			if(mineBoard[i][j] == 99)
				gameBoard[i][j] = 'F';
			else
				gameBoard[i][j] = '0' + mineBoard[i][j];

			cout<<gameBoard[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}