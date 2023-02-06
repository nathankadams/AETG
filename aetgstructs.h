#pragma once
#include <iostream>
#include <vector>

/**
 *
 *  This data structure holds all selected components for each
 *  instance and maintains a count of how many new pairs are
 *	covered by the case.
 *		
 */
class TestCase
{
private:
	std::vector<int> testComponents;
	int newPairs;
public:
	//constructor to create vector of components based on number of factors
	TestCase(int factors)
	{
		testComponents = std::vector<int>(factors, -1);
		newPairs = 0;
	}
	
	//returns current number of new pairs covered by the test case
	int newPairsCount()
	{
		return newPairs;
	}

	//returns how many factors are included in the test case
	int testSize()
	{
		return testComponents.size();
	}

	//returns which component is selected for a given factor (index)
	int atIndex(int index)
	{
		return testComponents[index];
	}

	//sets number of new pairs for the case directly
	void setNewPairs(int numPairs)
	{
		newPairs = numPairs;
	}

	//increases the number of new pairs for the case by one
	void incrementNewPairs()
	{
		newPairs++;
	}

	//decreases the number of new pairs for the case by one
	void decrementNewPairs()
	{
		newPairs--;
	}

	//places a selected component into the test case at the correct factor's index
	void setComponent(int factor, int componentNumber)
	{
		testComponents[factor] = componentNumber;
	}

	std::vector<int> getTest()
	{
		return testComponents;
	}

	//print all of the components in a test case on a single line
	void printTestCase()
	{
		for (int i = 0; i != testComponents.size(); i++)
		{
			std::cout << testComponents[i] << " ";
		}
		std::cout << std::endl;
	}
};