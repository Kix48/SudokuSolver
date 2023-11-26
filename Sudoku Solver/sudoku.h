#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

class Sudoku
{
	// Can be initialized for testing certain puzzle scenarios
	int squares[9][9] = { 0, 0, 8, 0, 3, 0, 4, 2, 1,
						  2, 0, 3, 4, 0, 0, 0, 0, 0,
					      0, 0, 0, 0, 2, 0, 0, 0, 3,
					      0, 0, 0, 0, 6, 4, 0, 1, 0,
					      0, 0, 0, 0, 0, 0, 2, 3, 8,
					      0, 0, 0, 0, 0, 7, 0, 4, 0,
					      4, 0, 0, 0, 0, 2, 0, 0, 5,
					      0, 1, 0, 0, 4, 3, 8, 0, 2,
					      0, 6, 2, 5, 0, 9, 0, 7, 4 };

	std::pair<std::vector<int>, std::vector<int>> FindMissingNumbersInRow(int row_index);
	std::pair<std::vector<int>, std::vector<int>> FindMissingNumbersInColumn(int column_index);
	std::pair<std::vector<int>, std::vector<std::pair<int, int>>> FindMissingNumbersInBox(int box_index);
	bool PuzzleSolved();

public:
	bool Initialize();
	bool Solve();
	void Print();
};