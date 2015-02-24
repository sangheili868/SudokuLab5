/*
Chris Maheu
Lab 5
Sudoku Solver
*/

#include<iostream>
#include "Puzzle.h"
#include <string>
using namespace std;

int displayerror(int);	// Displays appropriate text based on reason the cell could not be changed

int main()
{
	string filename,input;
	int placeerror=0,inputerror;
	cout<<"Enter the filename."<<endl;
	cin>>filename;
	Puzzle<char> myPuzzle(filename);

	while(1)	// Primary Game Loop
	{ 
	  do			// Loop to check for valid placement
	  {

	    do				// Loop to check for valid input
	    {
	      inputerror=0;
	      cout<<endl;
	      myPuzzle.display();
	      cout<<"Enter a three digit number where the first is the row, the second is the column, and the third is the new value for that cell, or q to quit."<<endl;
	      cin>>input;
	      if(input=="q") return 0;
	      if(input[0]<='0' || input[0]>'9' || input[1]<='0' || input[1]>'9')
	      {
		cout<<"Invalid input"<<endl;
		inputerror=1;
	      }
	    } while(inputerror);
					// There is valid input
	  } while(displayerror(myPuzzle.setCell(input[0]-'1',input[1]-'1',input[2])));
				// A valid number has been placed
	  if(myPuzzle.checkWin())
	  {
	  		// Game over
	    myPuzzle.display();
	    cout<<"Congratulations! You win!"<<endl;
	    return 1;
	  }
	}
}

int displayerror(int error)
{

	switch(error)
	{
	  case 0: 
	    break;
	  case 1:
	    cout<<"There is a number in that box already!"<<endl;
 	    break;
	  case 2:
	    cout<<"That number is in that box already!"<<endl;
	    break;
	  case 3:
	    cout<<"That number is in that row already!"<<endl;
	    break;
	  case 4:
	    cout<<"That number is in that column already!"<<endl;
	    break;
	  default:
	    cout<<"Some other error has occured."<<endl;
	}
	return error;
}
