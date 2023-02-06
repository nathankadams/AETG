#pragma once
#include "aetgstructs.h"

//definitions found in grid.cpp
void inputFactorLevels(std::vector<int>& factorLevels);
int countComponents(std::vector<int>& levels);
std::vector<std::vector<int>> componentGrid(int factors, std::vector<int>& levels, int totalComponents);
void printGrid(std::vector<std::vector<int>> &grid);
std::vector<int> initializeUncovered(std::vector<int>& levels, int totalComponents);
std::vector<int> factorStartingNums(std::vector<int>& levels);

//definitions found in testcases.cpp
TestCase firstTestGenerator(int factors, std::vector<int>& levels, std::vector<std::vector<int>>& grid);
TestCase testGenerator(int factors, std::vector<int>& levels, std::vector<int>& pairsRemaining, std::vector<int>& factorBegin, int totalComponents, std::vector<std::vector<int>>& grid);
void factorShuffle(std::vector<int>& factorOrder);
void countNewPairs(TestCase& currentTestCase, std::vector<std::vector<int>>& grid);
TestCase selectCandidate(int factors, std::vector<int>& levels, std::vector<int>& pairsRemaining, std::vector<int>& factorBegin, int totalComponents, std::vector<std::vector<int>>& grid);
void addToSuite(TestCase currentTestCase, std::vector<std::vector<int>>& grid, std::vector<int>& pairsRemaining);
std::vector<TestCase> selectSuite(std::vector<int>& factorLevels);
void outputSuiteFile(std::vector<TestCase>& selectedSuite);
void outputSuiteAnalytics(std::vector<TestCase>& selectedSuite, unsigned int smallestSuiteSize, unsigned int largestSuiteSize, int totalCases);