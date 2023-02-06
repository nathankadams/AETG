#include "aetgfunctions.h"
#include <stdlib.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
	//seed the random number generator for the entire program
	srand(rand() ^ time(0));
	
	//create vector for input and prompt user for desired factors and levels per factor
	vector<int> factorLevels;
	inputFactorLevels(factorLevels);

	//start counting execution time for generation of all test suites
	auto startTime = high_resolution_clock::now();

	vector<TestCase> selectedSuite = selectSuite(factorLevels);
	
	//stop counting execution time for generation of all test suites
	auto endTime = high_resolution_clock::now();

	//print total and average execution times in milliseconds
	auto duration = duration_cast<milliseconds>(endTime - startTime);
	cout << "Total generation time for all suites: " << duration.count() << " ms" << endl;
	cout << "Average suite generation time: " << duration.count() / 100 << " ms" << endl;

	return 0;
}