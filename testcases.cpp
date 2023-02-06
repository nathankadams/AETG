#include "aetgfunctions.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <fstream>

using namespace std;

/**
 *
 *	This function creates the first test case by randomizing
 *  the factor order and choosing a random component for each
 *  factor. Since all pairs are uncovered before the first
 *  test case, randomization of all components is possible.
 *
 *	Returns a test case with selected components for each factor.
 *
 */
TestCase firstTestGenerator(int factors, vector<int>& levels, vector<vector<int>>& grid)
{
	//creates an empty test case and vector for random factor ordering
	TestCase firstCase(factors);
	vector<int> factorOrder(factors);
	int currentComponent = -1;
	int factorStart = 0;

	//randomize order for factor selection
	factorShuffle(factorOrder);

	//select each factor from the randomized list
	for (int i = 0; i != factorOrder.size(); i++)
	{
		factorStart = 0;
		
		//for currently selected factor, get the starting component number
		for (int j = 0; j != factorOrder[i]; j++)
		{
			factorStart += levels[j];
		}
		
		//select a random component from the current factor and store it in the test case
		currentComponent = factorStart + (rand() % levels[factorOrder[i]]);
		firstCase.setComponent(factorOrder[i], currentComponent);

	}
	//count new pairs created with the first test case (this case will have maximum new pairs)
	countNewPairs(firstCase, grid);
	
	return firstCase;
}

/**
 *
 *	This function creates test cases by randomizing the
 *  factor order. The generator first chooses a random
 *  component for the first factor in the shuffled factor
 *  list. Each subsequent factor is chosen based on how many
 *  new pairs each of its components can make with previously
 *  selected components in the test case. The components
 *  which make the most new pairs are pooled and a random
 *  component from the pool is selected for the corresponding
 *  factor.
 *
 *	Returns a test case with selected components for each factor.
 *
 */
TestCase testGenerator(int factors, vector<int>& levels, vector<int>& pairsRemaining, vector<int>& factorBegin, int totalComponents, vector<vector<int>>& grid)
{
	//creates an empty test case, a vector for random factor ordering, and a vector to pool the best component choices
	TestCase testCase(factors);
	vector<int> factorOrder(factors);
	vector<int> maxPairs;
	int maxPairsLeft = 0;
	int currentFactor = -1;
	int selectedComponent = -1;
	int numFactorsSelected = 0;
	int totalNewPairs = 0;

	//randomize order for factor selection
	factorShuffle(factorOrder);

	//start with the first factor in the randomly ordered list
	currentFactor = factorOrder[0];
	
	//find the max number of remaining pairs from the currently selected factor's components
	maxPairsLeft = *max_element(pairsRemaining.begin() + factorBegin[currentFactor], pairsRemaining.begin() + factorBegin[currentFactor] + levels[currentFactor]);

	//pool all components in the current factor that have the max number of remaining pairs
	for (int i = factorBegin[currentFactor]; i != factorBegin[currentFactor] + levels[currentFactor]; i++)
	{
		if (pairsRemaining[i] == maxPairsLeft)
		{
			maxPairs.push_back(i);
		}
	}
	//use rand() % maxPairs.size() to select a component for the current factor
	selectedComponent = rand() % maxPairs.size();

	//add selected component to the test case in the correct factor position
	testCase.setComponent(currentFactor, maxPairs[selectedComponent]);
	
	//keep track of how many factors have been selected so far
	numFactorsSelected++;
		
	//for the remaining factors, components are chosen by potential new pairs formed
	for (int i = 1; i != factorOrder.size(); i++)
	{
		int possiblePairs = 0;
		int currentMaxPairs = 0;

		//select the next factor in the randomly ordered list
		currentFactor = factorOrder[i];
		
		//reset pool of best-fit components for the current factor
		maxPairs.clear();
		
		//count the number of new pairs that the current component makes with previously selected components
		for (int i = factorBegin[currentFactor]; i != factorBegin[currentFactor] + levels[currentFactor]; i++)
		{
			possiblePairs = 0;
			
			//check the grid to see if the current component can pair with the previously selected components
			for (int j = 0; j != numFactorsSelected; j++)
			{
				if (grid[i][testCase.atIndex(factorOrder[j])] == 0)
				{
					possiblePairs++;
				}
			}
			//check to see if the current component makes the most new pairs
			if (possiblePairs > currentMaxPairs)
			{
				currentMaxPairs = possiblePairs;
				//reset vector with just this component in it
				maxPairs.clear();
				maxPairs.push_back(i);
			}
			else if (possiblePairs == currentMaxPairs)
			{
				//add to pool of best components
				maxPairs.push_back(i);
			}
			else
			{
				//component is not a best fit
				continue;
			}
		}
		//select a random component from the pool of components that make the most new pairs
		selectedComponent = rand() % maxPairs.size();

		//store the selected component in the test case at the correct factor position
		testCase.setComponent(currentFactor, maxPairs[selectedComponent]);
		
		//keep track of how many factors have been selected so far
		numFactorsSelected++;

		//keep track of how many new pairs this test case has created so far
		totalNewPairs += currentMaxPairs;

	}
	//update the test case to contain the total number of new pairs created
	testCase.setNewPairs(totalNewPairs);

	return testCase;
}

/**
 *
 *	This function randomizes the order of the factors
 *  in a given test case for the purpose of choosing
 *  each factor's component in a random order.
 *
 *	Returns no value(s).
 *
 */
void factorShuffle(vector<int>& factorOrder)
{
	//initialize the vector so that each index has the corresponding factor number as a value
	for (int i = 0; i != factorOrder.size(); i++)
	{
		factorOrder[i] = i;
	}

	//seeds a random number generator to shuffle the factor ordering
	random_device device;
	auto randomNums = default_random_engine{ device() };
	shuffle(factorOrder.begin(), factorOrder.end(), randomNums);
}

/**
 *
 *	This function tracks how many new pairs are created by the
 *  first test case. It counts all pairs after the test case
 *  is created instead of procedurally, as is done in normal
 *  test case generation.
 *
 *	Returns no value(s).
 *
 */
void countNewPairs(TestCase& currentTestCase, vector<vector<int>>& grid)
{
	int newPairCounter = 0;
	
	//count the number of new pairs that the current component makes with previously selected components
	for (int i = 0; i != currentTestCase.testSize(); i++)
	{
		for (int j = i+1; j != currentTestCase.testSize(); j++)
		{
			//check the grid at the two components intersection to see if the pair is currently not covered
			if (grid[currentTestCase.atIndex(i)][currentTestCase.atIndex(j)] == 0)
			{
				newPairCounter++;
			}
		}
	}
	//update the test case to contain the total number of new pairs created
	currentTestCase.setNewPairs(newPairCounter);
}

/**
 *
 *	This function creates 50 test case candidates, adds
 *  the candidates which form the most new pairs to a pool,
 *  and randomly selects a test case from the pool to be
 *  added to the test suite.  
 *
 *	Returns a test case that creates the most new pairs.
 *
 */
TestCase selectCandidate(int factors, vector<int>& levels, vector<int>& pairsRemaining, vector<int>& factorBegin, int totalComponents, vector<vector<int>>& grid)
{
	//creates a vector to hold the best candidate test cases
	vector<TestCase> candidates;
	int currentMaxPairs = 0;
	int selectedTest = -1;

	//create 50 candidate test cases
	for (int i = 0; i != 50; i++)
	{
		TestCase newTest = testGenerator(factors, levels, pairsRemaining, factorBegin, totalComponents, grid);

		//check to see if the new test case makes the most new pairs
		if (newTest.newPairsCount() > currentMaxPairs)
		{
			currentMaxPairs = newTest.newPairsCount();
			//reset vector with just this test case in it
			candidates.clear();
			candidates.push_back(newTest);
		}
		else if (newTest.newPairsCount() == currentMaxPairs)
		{
			//add to pool of best test cases
			candidates.push_back(newTest);
		}
		else
		{
			//test case is not a good candidate
			continue;
		}
	}
	//select a random test case from the pool of candidates that makes the most new pairs
	selectedTest = rand() % candidates.size();

	return candidates[selectedTest];
}

/**
 *
 *	This function marks the grid with all new pairs found
 *  in a given test case. When a new pair is found, the
 *  vector which tracks how many pairs remain for a given
 *  component is updated. This is done by decrementing the
 *  index corresponding to each component in a new pair by 1.
 *
 *	Returns no value(s).
 *
 */
void addToSuite(TestCase currentTestCase, vector<vector<int>>& grid, vector<int>& pairsRemaining)
{
	//check each factor's selected component one by one
	for (int i = 0; i != currentTestCase.testSize(); i++)
	{
		//for each component, check each of the other selected components to determine new pairs
		for (int j = i + 1; j != currentTestCase.testSize(); j++)
		{
			//when the grid shows two components are not yet paired, mark that pair as covered
			if (grid[currentTestCase.atIndex(i)][currentTestCase.atIndex(j)] == 0)
			{
				grid[currentTestCase.atIndex(i)][currentTestCase.atIndex(j)] = 1;
				grid[currentTestCase.atIndex(j)][currentTestCase.atIndex(i)] = 1;

				//some components will be selected even after all pairs are covered
				//do no decrement the pair counter any further after this occurs
				if (pairsRemaining[currentTestCase.atIndex(i)] > 0)
				{
					pairsRemaining[currentTestCase.atIndex(i)]--;
				}

				if (pairsRemaining[currentTestCase.atIndex(j)] > 0)
				{
					pairsRemaining[currentTestCase.atIndex(j)]--;
				}
			}
		}
	}
}

/**
 *
 *	This function creates 100 test suite candidates, adds
 *  the candidates which have the fewest test cases to a
 *  pool, and randomly selects a test suite from the pool.
 *  This suite is representative of the best possible outcome
 *  for this iteration of the program running.
 *  
 *	Returns a test suite that has the fewest test cases.
 *
 */
vector<TestCase> selectSuite(vector<int>& factorLevels)
{
	//creates a vector to hold the best candidate test suites and keeps tracks of best/worst suite sizes
	vector<TestCase> testSuite;
	vector<vector<TestCase>> bestSuites;
	unsigned int smallestSuiteSize = 10000;
	unsigned int largestSuiteSize = 0;
	int totalCases = 0;

	//find the first component for each factor and count total components in the component pool
	vector<int> factorBegin = factorStartingNums(factorLevels);
	int totalComponents = countComponents(factorLevels);

	//create 100 test suites for comparison
	for (int i = 0; i != 100; i++)
	{
		//ensure that no leftover test cases remain in the suite from previous iterations
		testSuite.clear();

		//initialize the suite's grid and set up vector to track number of remaining pairs for each component
		vector<vector<int>> grid = componentGrid(factorLevels.size(), factorLevels, totalComponents);
		vector<int> pairsRemaining = initializeUncovered(factorLevels, totalComponents);

		//generate our first test case randomly and add it to the suite
		TestCase firstSelection = firstTestGenerator(factorLevels.size(), factorLevels, grid);
		addToSuite(firstSelection, grid, pairsRemaining);
		testSuite.push_back(firstSelection);

		//continue generating all other test cases for the suite until no new pairs remain
		while (*max_element(pairsRemaining.begin(), pairsRemaining.end()) != 0)
		{
			//generate a new test case randomly and add it to the suite
			TestCase nextSelection = selectCandidate(factorLevels.size(), factorLevels, pairsRemaining, factorBegin, totalComponents, grid);
			addToSuite(nextSelection, grid, pairsRemaining);
			testSuite.push_back(nextSelection);
		}

		//track total number of cases generated across suites
		totalCases += testSuite.size();

		//check if the current test suite is the largest suite so far
		if (testSuite.size() > largestSuiteSize)
		{
			largestSuiteSize = testSuite.size();
		}

		//check if the current test suite is the smallest suite so far
		if (testSuite.size() < smallestSuiteSize)
		{
			smallestSuiteSize = testSuite.size();
			//reset vector with just this test suite in it
			bestSuites.clear();
			bestSuites.push_back(testSuite);
		}
		else if (testSuite.size() == smallestSuiteSize)
		{
			//add to pool of best test suites
			bestSuites.push_back(testSuite);
		}
		else
		{
			//test suite is not the best candidate
			continue;
		}
	}
	//select one of the best suites
	vector<TestCase> selectedSuite = bestSuites[rand() % bestSuites.size()];

	//output the suite to a file named testsuite.txt
	outputSuiteFile(selectedSuite);
	
	//output the information to the console in addition to the file format
	outputSuiteAnalytics(selectedSuite, smallestSuiteSize, largestSuiteSize, totalCases);

	return selectedSuite;
}

/**
 *
 *	This function sends all test cases from the selected
 *  suite to an output file. The total number of test
 *  cases is printed on the first line followed by a blank
 *  line. Then each test case's components are printed
 *  on their own line in a space delimited format.
 *
 *	Returns no value(s).
 *
 */
void outputSuiteFile(vector<TestCase>& selectedSuite)
{
	//open a file stream to output the suite to a file called testsuite.txt
	ofstream outputFile;
	outputFile.open("testsuite.txt");

	//print the total number of test cases in the suite
	outputFile << selectedSuite.size() << endl;
	outputFile << endl;

	//print out the suite to the text file in the requested format
	for (int i = 0; i != selectedSuite.size(); i++)
	{
		//print each test case line by line
		vector<int> outputTest = selectedSuite[i].getTest();
		for (int j = 0; j != outputTest.size(); j++)
		{
			outputFile << outputTest[j] << " ";
		}
		outputFile << endl;
	}
	//close the file stream
	outputFile.close();
}

/**
 *
 *	This function prints all test cases from the selected
 *  suite to the console. The total number of test cases
 *  is printed on the first line followed by a blank
 *  line. Then each test case's components are printed
 *  on their own line in a space delimited format. The
 *  smallest, largest, and average (rounded down) suite
 *  sizes are output to the console as well.
 *
 *	Returns no value(s).
 *
 */
void outputSuiteAnalytics(vector<TestCase>& selectedSuite, unsigned int smallestSuiteSize, unsigned int largestSuiteSize, int totalCases)
{
	//print the total number of test cases in the suite
	cout << selectedSuite.size() << endl;
	cout << endl;
	
	//print each test case line by line
	for (int i = 0; i != selectedSuite.size(); i++)
	{
		selectedSuite[i].printTestCase();
	}
	//print analytics to the console
	cout << "********** Analytics **********" << endl;
	cout << "Smallest suite size: " << smallestSuiteSize << endl;
	cout << "Largest suite size: " << largestSuiteSize << endl;
	cout << "Average suite size (rounded down): " << totalCases / 100 << endl;
}