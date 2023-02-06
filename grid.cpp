#include "aetgfunctions.h"
#include <iomanip>
#include <numeric>

using namespace std;

void inputFactorLevels(vector<int>& factorLevels)
{
	int factors = 0;
	int levels = 0;
	int selection = -1;

	cout << "Enter the number of factors: ";
	cin >> factors;
	cout << "Does each factor have the same number of levels?" << endl;
	cout << "1) Yes" << endl;
	cout << "2) No" << endl;
	cin >> selection;

	if (selection == 1)
	{
		cout << "Enter the number of levels per factor: ";
		cin >> levels;
		for (int i = 0; i < factors; i++)
		{
			factorLevels.push_back(levels);
		}
	}
	else if (selection == 2)
	{
		for (int i = 0; i < factors; i++)
		{
			cout << "Enter the number of levels in factor " << i << ": ";
			cin >> levels;
			factorLevels.push_back(levels);
		}
	}
	else
	{
		cout << "INPUT ERROR: Please input a valid choice next time (1 for Yes, 2 for No)." << endl;
		exit(0);
	}
}

/**
 *
 *	This function counts the total number of components
 *	across all factors.
 * 
 *	Returns the total component count as an integer.
 *
 */
int countComponents(vector<int>& levels)
{
	//sum all levels from the input vector
	int totalComponents = accumulate(levels.begin(), levels.end(), 0);
	
	return totalComponents;
}

/**
 *
 *	This function creates a totalComponents x totalComponents
 *  grid to keep track of all possible component pairs. In the
 *  grid, any illegal pair of components, such as components in
 *  the same category (factor) are marked as -1. Any unpaired
 *  components are marked as 0 and any paired components are
 *  marked as 1.
 * 
 *  example:
 *      c0 c1 c2 c3 c4 c5 c6 c7 c8
 *  c0: -1 -1 -1  0  0  0  0  1  0
 *  c1: -1 -1 -1  0  0  0  0  0  0
 *  c2: -1 -1 -1  0  0  0  0  0  0
 *  c3:  0  0  0 -1 -1 -1  0  0  0
 *  c4:  0  0  0 -1 -1 -1  0  0  0
 *  c5:  0  0  0 -1 -1 -1  0  0  0
 *  c6:  0  0  0  0  0  0 -1 -1 -1
 *  c7:  1  0  0  0  0  0 -1 -1 -1
 *  c8:  0  0  0  0  0  0 -1 -1 -1
 *
 *	grid[0][1] == -1 ---> components 0 and 1 are in the same factor
 *  grid[0][5] ==  0 ---> components 0 and 5 have not yet been paired
 *  grid[0][7] ==  1 ---> components 0 and 7 have been paired
 *
 *	Returns the initialized grid with all possible pairs uncovered (0 in all legal positions).
 *
 */
vector<vector<int>> componentGrid(int factors, vector<int>& levels, int totalComponents)
{
	//initialize size of grid's 2D vector and track where current and next factors begin
	vector<vector<int>> grid(totalComponents, vector<int>(totalComponents, 0));
	int currentFactor = 0;
	int currentFactorStart = 0;
	int nextFactorStart = currentFactorStart + levels[currentFactor];

	//ensure that the function stays in bounds of total factors
	while (currentFactor != (levels.size() - 1))
	{
		//for each level in each factor, don't combine with levels in the same factor
		for (int i = currentFactorStart; i != nextFactorStart; i++)
		{
			for (int j = currentFactorStart; j != nextFactorStart; j++)
			{
				grid[i][j] = -1;
			}

			//when finished with a factor's levels, update the next factor's first number
			if ((i + 1) % nextFactorStart == 0 && currentFactor != (levels.size() - 1))
			{
				currentFactor++;
				currentFactorStart = nextFactorStart;
				nextFactorStart += levels[currentFactor];
			}
		}
	}
	return grid;
}

/**
 *
 *	This function prints the grid's current state in a
 *	user-friendly format. Any incompatible pairs are marked
 *  with 'x' while any components which are not yet paired
 *  are marked with '-' instead of -1 or 0, respectively.
 *  All covered pairs remain marked as 1.
 *
 *	Returns no value(s).
 *
 */
void printGrid(vector<vector<int>>& grid)
{
	//prints the outer vector as rows of corresponding symbols in the grid
	for (int i = 0; i != grid.size(); i++)
	{
		cout << "outer " << setw(3) << i << ": ";
		
		//prints each component in the row one by one, replacing symbols as needed
		for (int j = 0; j != grid[i].size(); j++)
		{
			//replace 0s with '-' and -1s with 'x'
			if (grid[i][j] == 0)
			{
				cout << "-";
			}
			else if (grid[i][j] == -1)
			{
				cout << "x";
			}
			else
			{
				cout << grid[i][j];
			}
		}
		cout << endl;
	}
}

/**
 *
 *	This function creates a vector which holds how many
 *  uncovered pairs remain for each component. The index
 *  of this vector corresponds to the component number and
 *  the value held at a component's index is the number
 *  of uncovered pairs remaining. This is updated as pairs
 *  are formed during test case generation.
 *
 *	Returns a vector containing a count of uncovered pairs
 *  remaining for each component.
 *
 */
vector<int> initializeUncovered(vector<int>& levels, int totalComponents)
{
	//initialize size of the vector based on total components
	vector<int> uncoveredCount(totalComponents);
	int currentFactor = 0;
	int currentFactorStart = 0;
	int nextFactorStart = currentFactorStart + levels[currentFactor];

	//set the initial component's uncovered pair count to total components - current factor's levels
	//example: if there are 20 total components and 5 components in the current factor,
	//		   20 - 5 = 15 possible pairs for the current component
	for (int i = currentFactorStart; i != nextFactorStart; i++)
	{
		uncoveredCount[i] = totalComponents - levels[currentFactor];

		//when finished with a factor's levels, update the next factor's first number
		if ((i + 1) % nextFactorStart == 0 && currentFactor != (levels.size() - 1))
		{
			currentFactor++;
			currentFactorStart = nextFactorStart;
			nextFactorStart += levels[currentFactor];
		}
	}
	return uncoveredCount;
}

/**
 *
 *	This function uses a vector to store where each factor
 *  begins. This is used to keep track of which components
 *  we are currently checking. The index number corresponds
 *  to the factor.
 *  
 *  example:
 *	factor 1 is stored in factorBegin[1]
 *
 *	Returns a vector containing the starting component number
 *  for each factor.
 *
 */
vector<int> factorStartingNums(vector<int>& levels)
{
	//initialize size of the vector based on number of factors
	vector<int> factorBegin(levels);
	int currentNum = 0;
	
	//set each factor's start as its first component's number
	for (int i = 0; i != factorBegin.size(); i++)
	{
		//each factor begins at the previous factor's starting
		//component plus the number of levels in the previous factor
		factorBegin[i] = currentNum;
		currentNum += levels[i];
	}
	return factorBegin;
}