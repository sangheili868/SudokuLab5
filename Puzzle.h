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
	  Puzzle(string="puz1.txt");		// Constructor reads from file
	  void display() const;			// Display current grid
	  void displayPoss() const;		// Display possibilities grid
	  int setCell(int,int,T);		// Set cell at pos (x,y) to value given. 0 if successful
	  int checkCell(int,int,T);		// Check if value can be placed at pos (x,y). 0 if successful
	  int checkOutput(int,int,T, int);	// Outputs text based on checkCell error
	  int checkPoss(int, int);		// If a cell has only one possibility, sets that cell and return 1
	  int checkWin() const;			// Outputs 1 if all cells are full, 0 otherwise
	  int solve();				// Solves the puzzle
	private:
	  void printRow(int) const;		// Display a single row, used in display()
	  void initPoss();			// Initializes possibilities vector
	  void checkRows();			// Checks if there is any number in a row that can only fit in one place in that row
 	  void checkCols();			// Checks if there is any number in a col that can only fit in one place in that col
 	  void checkMini();			// Checks if there is any number in a minigrid that can only fit in one place in that minigrid
	  void singleton(int, int);		// If two cells in the same col, row, or minigrid have the same two possible values, those values cannot be anywhere else in that unit
	  static int changed;			// Stores whether grid has been updated in the pass through
	  vector<vector<T> > grid;		// Grid storing all elements
	  vector<vector<vector<T> > > poss;	// Stores possible values for each cell. [x][y][3]==1 means 3 can be placed in cell x,y.
};

template <typename T>
int Puzzle<T>::changed=0;	// Used to tell if the puzzle was updated in the loop through the whole grid

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Initializing  ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////

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

	// Initializes possibilities vector
template <typename T>
void Puzzle<T>::initPoss()
{
	vector<T> tempvec;
	vector<vector<T> > tempvec2;

	for (int i=0;i<9;i++)
	{
	  for (int j=0;j<9;j++)
	  {
	    if(grid[i][j])	// Value already set here
	    {
	      for (int k=0;k<10;k++)
	        tempvec.push_back(0);	// No possibilities because value already set
	      tempvec2.push_back(tempvec);
	      tempvec.clear();

	    }else{
	      tempvec.push_back(0);	// First element doesn't matter
	      for (int k=1;k<10;k++)
	        tempvec.push_back(1);	// Assume at start that all numbers are possible
	      tempvec2.push_back(tempvec);
	      tempvec.clear();
	    }
	  }
	  poss.push_back(tempvec2);
	  tempvec2.clear();
	}
}
		
///////////////////////////////////////////////////////////////////////////
/////////////////////////////// Displaying  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////

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

	// Display possibilities grid. Used for debugging
template <typename T>
void Puzzle<T>::displayPoss() const
{
	for (int i=0;i<9;i++)
	{
	  cout<<"X: "<<i<<endl;
	  for (int j=0;j<9;j++)
	  {
	    cout<<"Y: "<<j<<" P: ";
	    for (int k=0;k<10;k++)
		cout<<poss[i][j][k];
	    cout<<endl;
	  }
	}
}

	// Outputs text for checkcell error. Used for debugging.
template <typename T>
int Puzzle<T>::checkOutput(int x,int y,T value, int error)
{

	    switch(error)
	    {
	      case 0:	// Placed correctly
		break;
	      case 1:
	        cout<<"Tried to place "<<value<<" in "<<x<<", "<<y<<" but that cell is filled."<<endl;
 	        break;
	      case 2:
	        cout<<"Tried to place "<<value<<" in "<<x<<", "<<y<<" but that number is in that minigrid."<<endl;
	        break;
	      case 3:
	        cout<<"Tried to place "<<value<<" in "<<x<<", "<<y<<" but that number is in that row."<<endl;
	        break;
	      case 4:
	        cout<<"Tried to place "<<value<<" in "<<x<<", "<<y<<" but that number is in that column."<<endl;
	        break;
	      default:
	        cout<<"Tried to place "<<value<<" in "<<x<<", "<<y<<" but some wierd error occured."<<endl;
		break;
	    }
	if(error) display();
	return error;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Modifying Cells ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	// Set cell at pos (x,y) to value given. 0 if successful
	// 1: Cell filled 	2: Number in minigrid
	// 3: Number in row	4: Number in column
template <typename T>
int Puzzle<T>::setCell(int x,int y,T value)
{
	int check;
	// Check the cell
	check=checkCell(x,y,value);
	// Change the value.
	if(!check) grid[x][y]=value;
	return check;
}

	// Check if value can be placed at pos (x,y). 0 if successful
template <typename T>
int Puzzle<T>::checkCell(int x,int y,T value)
{
	int mgrow, mgcol;

	// Check if a value already in Cell
	if(grid[x][y]!=0) return 1;

	// Check if the value already in Minigrid
	mgrow=(x/3)*3;		// Gets the x and y of the top left cell in its minigrid
	mgcol=(y/3)*3;
	for(int i=0;i<3;i++)	// Check each cell in the minigrid
	  for(int j=0;j<3;j++)
	    if(grid[mgrow+i][mgcol+j]==value) return 2;
	
	// Check if the value already in Row
	for(int i=0;i<grid[0].size();i++)
	  if(grid[x][i]==value) return 3;	

	// Check if the value already in Col
	for(int i=0;i<grid[0].size();i++)
	  if(grid[i][y]==value) return 4;
	
	//All checks passed
	return 0;
}

	// Outputs 1 if all cells are full, 0 otherwise
template <typename T>
int Puzzle<T>::checkWin() const
{
	// Loop through entire grid
	for (int i=0;i<grid.size();i++)
	  for (int j=0;j<grid[0].size();j++)
	    if(grid[i][j]==0) 
	      return 0;		// An empty spot has been found in the grid
	return 1;				// No empty spots were found
}
	
///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Puzzle Solving  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	// Solves the puzzle
template <typename T>
int Puzzle<T>::solve()
{
	cout<<"Starting to solve"<<endl;
	initPoss();
	int count=1;

	while(!checkWin())
	{
	  changed=0;
	  for (int i=0;i<grid.size();i++)	// These two loop
	    for (int j=0;j<grid[0].size();j++)	// through the grid
	    {
	      if(!grid[i][j])			// Found an empty cell
	      {
	        for(int k=1;k<10;k++)		// Loop through all values to put there
		  if(checkCell(i,j,k))		// If this value cannot be placed in the cell
		    poss[i][j][k]=0;		// Set that value as not possible
	        if(checkPoss(i,j)==2) return 0;	// See if there is only one value to put there. Quit if nothing can be placed there
	      }
	    }
	  checkRows();		// Go through all rows to see if any value can only go in one spot in that row
	  checkCols();		// Go through all columns to see if any value can only go in one spot in that column
	  checkMini();		// Go through all minigrids to see if any value can only go in one spot in that minigrid
	  cout<<"Passed through grid "<<count<<" times."<<endl;
	  count++;
	  // Check if any cells were changed during the loop
	  if(!changed)
	  {
	    cout<<"Could not find new cells to place. Make me smarter."<<endl;
	    display();
	    return 0;
	  }
	}

	// Show completed puzzle
	display();
	return 1;
	
}

	// If a cell has only one possibility, sets that cell
template <typename T>
int Puzzle<T>::checkPoss(int x, int y)	// Outputs: 2: cannot place anything, 1: placed correctly, 0: Multiple things can be placed
{
	if(grid[x][y]) return 0;	// Already a value there

	int count=0, value=0, error=0, i, singleposs;

	// Count possible values to put in this cell
	for(i=1;i<10;i++)
	  if(poss[x][y][i]) 
	  {
	    count++;
	    value=i;	// Stores last value found. Used if only one value counted
	  }

	switch(count)
	{
	  case 0:	// No possibilites for this cell
	    cout<<"Error. Lost the game. "<<endl;
	    cout<<"Cannot place any number at position "<<x<<", "<<y<<endl;
	    display();
	    return 2;
	  case 1:	// One possibility for this cell, so place the number
	    error=setCell(x,y,value);
	    if(!checkOutput(x,y,value,error))
	    {	// Placed Correctly
		cout<<"Placing "<<value<<" in "<<x+1<<", "<<y+1<<" because only possibility"<<endl;
		changed=1;
		poss[x][y][value]=0;
		return 1;
	    }
	    break;
	  case 2:	// Two possibilities. Check if any pairs found to eliminate possibilities
	    singleton(x,y);
	    break;
	  default:	// More than one possibility for this cell, so do nothing
	    break;
	}   
	return 0;
}

	// If two cells in the same col, row, or minigrid have the same two possible values, those values cannot be anywhere else in that unit
template <typename T>
void Puzzle<T>::singleton(int x, int y)	
{	
	vector<T> values;
	int mgrow,mgcol;

	// Get possible values of this cell
	for(int i=1;i<10;i++)
	  if(poss[x][y][i]) 
	    values.push_back(i);

	// Check Row
	for(int i=0;i<9;i++)			// Loop through all in the same row
	  if(poss[x][i]==poss[x][y] && i!=y)	// Found a match
	    for(int j=0;j<9;j++)		// Loop through cells in row
	      if(j!=i && j!=y)			// Make sure not same cells that match
		for(int k=0;k<values.size();k++)// Switch between the pair of possible values
		  poss[x][j][values[k]]=0;	// Set those as not possible for this cell

	// Check Column
	for(int i=0;i<9;i++)			// Loop through all in the same column
	  if(poss[i][y]==poss[x][y] && i!=x)	// Found a match
	    for(int j=0;j<9;j++)		// Loop through cells in column
	      if(j!=i && j!=x)			// Make sure not same cells that match
		for(int k=0;k<values.size();k++)// Switch between the pair of possible values
		  poss[j][y][values[k]]=0;	// Set those as not possible for this cell

	// Check Minigrid
	mgrow=(x/3)*3;					// Gets the x and y of the top left cell in its minigrid
	mgcol=(y/3)*3;
	for(int i=mgrow;i<mgrow+3;i++)			// Check each cell in the minigrid
	for(int j=mgcol;j<mgcol+3;j++)
	  if(poss[i][j]==poss[x][y] && (i!=x || j!=y))	// Found a match
	    for(int k=mgrow;k<mgrow+3;k++)		// Loop through cells in the minigrid
	    for(int l=mgcol;l<mgcol+3;l++)
	      if((k!=x || l!=y) && (k!=i || l!=j))	// Make sure not same cells that match
		for(int m=0;m<values.size();m++)	// Switch between the pair of possible values
		  poss[k][l][values[m]]=0;		// Set those as not possible for this cell	
}

	// Checks if there is any number in a row that can only fit in one place
template <typename T>
void Puzzle<T>::checkRows()	
{
	int count,spot,error;

	for(int i=0;i<9;i++)		// Loop through all rows
	  for(int n=1;n<10;n++)		// Loop through all possible values
	  {
	    count=0;
	    for(int j=0;j<9;j++)	// Loop through all cells in the row
	    {
	      if(grid[i][j]==n) count=10;	// Value already in the row
	      if(poss[i][j][n])		// Found a cell with n as a possible value
	      {
		count++;
		spot=j;
	      }
	    }
	    if(count==1) 		// Exactly one cell in the row can have that number
	    {
		// Set n at that spot
	      error=setCell(i,spot,n);
	      if(!checkOutput(i,spot,n,error))
	      {	// Placed Correctly
	  	cout<<"Placed "<<n<<" in "<<i+1<<", "<<spot+1<<" because only spot in row"<<endl;
		changed=1;
		for(int j=0;j<10;j++)
		  poss[i][spot][j]=0;
	      }
	    }
	}
}

	// Checks if there is any number in a column that can only fit in one place
template <typename T>
void Puzzle<T>::checkCols()	
{
	int count,spot,error;

	for(int j=0;j<9;j++)		// Loop through all cols
	  for(int n=1;n<10;n++)		// Loop through all possible values
	  {
	    count=0;
	    for(int i=0;i<9;i++)	// Loop through all cells in the col
	    {
	      if(grid[i][j]==n) count=10;	// Value already in the col
	      if(poss[i][j][n])	// Found a cell with n as a possible value
	      {
		count++;
		spot=i;
	      }
	    }
	    if(count==1) 		// Exactly one cell in the col can have that number
	    {
		// Set n at that spot
	      error=setCell(spot,j,n);
	      if(!checkOutput(spot,j,n,error))
	      {	// Placed Correctly
	  	cout<<"Placed "<<n<<" in "<<spot+1<<", "<<j+1<<" because only spot in column"<<endl;
		changed=1;
		for(int i=0;i<10;i++)
		  poss[spot][j][i]=0;
	      }
	    }
	}
}

	// Checks if there is any number in a minigrid that can only fit in one place
template <typename T>
void Puzzle<T>::checkMini()	
{
	int count,spotx,spoty,error;

	for(int mgrow=0;mgrow<9;mgrow+=3)	// Loop through all minigrids
	for(int mgcol=0;mgcol<9;mgcol+=3)		
	  for(int n=1;n<10;n++)			// Loop through all possible values
	  {
	    count=0;
	    for(int i=mgrow;i<mgrow+3;i++)	// Loop through all cells in the minigrid
	    for(int j=mgcol;j<mgcol+3;j++)	
	    {
	      if(grid[i][j]==n) count=10;	// Value already in the minigrid
	      if(poss[i][j][n])			// Found a cell with n as a possible value
	      {
		count++;
		spotx=i;
		spoty=j;
	      }
	    }
	    if(count==1) 		// Exactly one cell in the minigrid can have that number
	    {
		// Set n at that spot
	      error=setCell(spotx,spoty,n);
	      if(!checkOutput(spotx,spoty,n,error))
	      {	// Placed Correctly
	  	cout<<"Placed "<<n<<" in "<<spotx+1<<", "<<spoty+1<<" because only spot in minigrid"<<endl;
		changed=1;
		for(int q=0;q<10;q++)
		  poss[spotx][spoty][q]=0;
	      }
	    }
	}
}	
#endif
