/*
Chris Maheu
Lab 5
Sudoku Solver
*/

#include<iostream>
#include "Puzzle.h"
#include <string>
#include <vector>
using namespace std;

int displayerror(int);	// Displays appropriate text based on reason the cell could not be changed

int main()
{
	string filename;
	cout<<"Enter the filename."<<endl;
	cin>>filename;
	//filename="medpuz.txt";
	Puzzle<int> myPuzzle(filename);

	myPuzzle.display();

	// Check if puzzle is already full
	if(myPuzzle.checkWin())
	{
	  cout<<"Woah! The puzzle was already complete! Good for you!"<<endl;
	  return 1;
	}

	myPuzzle.solve();
}
