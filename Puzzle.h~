#ifndef CLASS_H
#define CLASS_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

template <typename T>
class Puzzle
{
	public:
	  Puzzle(string="puz1.txt");	// Constructor reads from file
	  void display() const;		// Display current grid
	  int setCell(int,int,T);	// Set cell at pos (x,y) to value given. 0 if successful
	  int checkWin() const;		// Outputs 1 if all cells are full, 0 otherwise
	private:
	  void printRow(int) const;	// Display a single row, used in display()
	  vector<vector<T> > grid;	// Grid storing all elements
};

	// Constructor reads from file
template <typename T>
Puzzle<T>::Puzzle(string filename)
{
	vector<T> tempvec;
	T temp;

	ifstream myFile(filename.c_str());

	// Loop through grid, entering all values
	for (int i=0;i<9;i++)
	{
	  for (int j=0;j<9;j++)
	  {
	    myFile>>temp;
	    tempvec.push_back(temp);
	  }
	  grid.push_back(tempvec);
	  tempvec.clear();
	}
}

	// Display current grid
template <typename T>
void Puzzle<T>::display() const
{
	cout<<"*---*---*---*"<<endl;
	for(int i=0;i<3;i++)
	  printRow(i);
	cout<<"*---*---*---*"<<endl;
	for(int i=3;i<6;i++)
	  printRow(i);
	cout<<"*---*---*---*"<<endl;
	for(int i=6;i<9;i++)
	  printRow(i);
	cout<<"*---*---*---*"<<endl;
}

	// Display a single row, used in display()
template <typename T>
void Puzzle<T>::printRow(int i) const
{
	cout<<"|";
	for (int j=0;j<3;j++)
	  cout<<grid[i][j];
	cout<<"|";
	for (int j=3;j<6;j++)
	  cout<<grid[i][j];
	cout<<"|";
	for (int j=6;j<9;j++)
	  cout<<grid[i][j];
	cout<<"|"<<endl;
}

	// Set cell at pos (x,y) to value given. 0 if successful
	// 1: Cell filled 	2: Number in minigrid
	// 3: Number in row	4: Number in column
template <typename T>
int Puzzle<T>::setCell(int x,int y,T value)
{
	int brow, bcol;

	// Check Cell
	if(grid[x][y]!=(T)'0') return 1;

	// Check Box
	brow=(x/3)*3;		// Gets the x and y of the top left cell in its box
	bcol=(y/3)*3;
	for(int i=0;i<3;i++)	// Check each cell in the minigrid
	  for(int j=0;j<3;j++)
	    if(grid[brow+i][bcol+j]==value) return 2;
	
	// Check Row
	for(int i=0;i<grid[0].size();i++)
	  if(grid[x][i]==value) return 3;	

	// Check Col
	for(int i=0;i<grid[0].size();i++)
	  if(grid[i][y]==value) return 4;

	// All checks passed. Change the value.
	grid[x][y]=value;
	return 0;
}

	// Outputs 1 if all cells are full, 0 otherwise
template <typename T>
int Puzzle<T>::checkWin() const
{
	for (int i=0;i<grid.size();i++)
	  for (int j=0;j<grid[0].size();j++)
	    if(grid[i][j]==(T)'0') 
	      return 0;		// An empty spot has been found in the grid
	return 1;				// No empty spots were found
}
	
#endif
