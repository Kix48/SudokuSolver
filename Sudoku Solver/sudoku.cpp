#include "sudoku.h"

std::pair<std::vector<int>, std::vector<int>> Sudoku::FindMissingNumbersInRow(int row_index)
{
	std::vector<int> missing_numbers;
	std::vector<int> missing_indexes;

	std::vector<int> found_numbers;
	for (int i = 0; i < ARRAYSIZE(this->squares[row_index]); i++)
	{
		if (this->squares[row_index][i] == 0)
		{
			missing_indexes.push_back(i);
		}
		else
		{
			found_numbers.push_back(this->squares[row_index][i]);
		}
	}

	for (int j = 1; j <= 9; j++)
	{
		if (std::find(found_numbers.begin(), found_numbers.end(), j) == found_numbers.end())
		{
			missing_numbers.push_back(j);
		}
	}
	
	return std::pair<std::vector<int>, std::vector<int>>(missing_numbers, missing_indexes);
}

std::pair<std::vector<int>, std::vector<int>> Sudoku::FindMissingNumbersInColumn(int column_index)
{
	std::vector<int> missing_numbers;
	std::vector<int> missing_indexes;

	std::vector<int> found_numbers;
	for (int i = 0; i < ARRAYSIZE(this->squares); i++)
	{
		if (this->squares[i][column_index] == 0)
		{
			missing_indexes.push_back(i);
		}
		else
		{
			found_numbers.push_back(this->squares[i][column_index]);
		}
	}

	for (int j = 1; j <= 9; j++)
	{
		if (std::find(found_numbers.begin(), found_numbers.end(), j) == found_numbers.end())
		{
			missing_numbers.push_back(j);
		}
	}

	return std::pair<std::vector<int>, std::vector<int>>(missing_numbers, missing_indexes);
}

std::pair<std::vector<int>, std::vector<std::pair<int, int>>> Sudoku::FindMissingNumbersInBox(int box_index)
{
	std::vector<int> missing_numbers;
	std::vector<std::pair<int, int>> missing_locations;

	int base_row = (box_index / 3) * 3;
	int base_column = (box_index % 3) * 3;

	std::vector<int> found_numbers;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (this->squares[base_row + i][base_column + j] == 0)
			{
				missing_locations.push_back(std::make_pair<int, int>(base_row + i, base_column + j));
			}
			else
			{
				found_numbers.push_back(this->squares[base_row + i][base_column + j]);
			}
		}
	}

	for (int k = 1; k <= 9; k++)
	{
		if (std::find(found_numbers.begin(), found_numbers.end(), k) == found_numbers.end())
		{
			missing_numbers.push_back(k);
		}
	}

	return std::pair<std::vector<int>, std::vector<std::pair<int, int>>>(missing_numbers, missing_locations);
}

bool Sudoku::PuzzleSolved()
{
	for (int i = 0; i < ARRAYSIZE(this->squares); i++)
	{
		for (int j = 0; j < ARRAYSIZE(this->squares[i]); j++)
		{
			if (this->squares[i][j] == 0)
			{
				return false;
			}
		}
	}

	return true;
}

// Setup initial puzzle from user input
bool Sudoku::Initialize()
{
	for (int i = 0; i < ARRAYSIZE(this->squares); i++)
	{
		for (int j = 0; j < ARRAYSIZE(this->squares[i]); j++)
		{
			this->squares[i][j] = -1;
		}
	}

	for (int i = 0; i < ARRAYSIZE(this->squares); i++)
	{
		if (i == 0)
		{
			system("cls");
			printf("Please enter the puzzle left-to-right with '0' being a blank space:\n");
		}

		for (int j = 0; j < ARRAYSIZE(this->squares[i]); j++)
		{
			if (this->squares[i][j] == -1)
			{
				int input = -1;
				std::cin >> input;

				if (input >= 0 && input <= 9)
				{
					this->squares[i][j] = input;
				}

				i = -1;
				break;
			}

			printf("%d ", this->squares[i][j]);	
		}

		printf("\n");
	}

	system("cls");

	return true;
}

// Solve the puzzle
// 1. Each row goes 1-9
// 2. Each column goes 1-9
// 3. Each 3x3 goes 1-9
bool Sudoku::Solve()
{
	bool number_added = false;

	do
	{
		number_added = false;

		// Check boxes
		for (int box = 0; box < 9; box++)
		{
			auto result_box = this->FindMissingNumbersInBox(box);
			if (result_box.first.size() == 1)
			{
				this->squares[result_box.second.at(0).first][result_box.second.at(0).second] = result_box.first.at(0);
				//printf("Added [%d][%d] -> %d\n", result_box.second.at(0).second, result_box.second.at(0).first, result_box.first.at(0));
				number_added = true;
				break;
			}
			else
			{
				for (auto missing_number : result_box.first)
				{
					int blocked_count = 0;
					for (auto missing_index : result_box.second)
					{
						auto result_column = this->FindMissingNumbersInColumn(missing_index.second);
						auto result_row = this->FindMissingNumbersInRow(missing_index.first);
						if (std::find(result_column.first.begin(), result_column.first.end(), missing_number) == result_column.first.end() ||
							std::find(result_row.first.begin(), result_row.first.end(), missing_number) == result_row.first.end())
						{
							// Number is already in column/row so it cannot be in this index of the box
							blocked_count++;
						}
					}

					if (blocked_count == (result_box.first.size() - 1))
					{
						for (auto missing_index : result_box.second)
						{
							auto result_column = this->FindMissingNumbersInColumn(missing_index.second);
							auto result_row = this->FindMissingNumbersInRow(missing_index.first);
							if (std::find(result_column.first.begin(), result_column.first.end(), missing_number) != result_column.first.end() &&
								std::find(result_row.first.begin(), result_row.first.end(), missing_number) != result_row.first.end())
							{
								this->squares[missing_index.first][missing_index.second] = missing_number;
								//printf("Added [%d][%d] -> %d\n", missing_index.second, missing_index.first, missing_number);
								number_added = true;
							}
						}

						break;
					}
				}
			}
		}

		// Check rows
		for (int row = 0; row < ARRAYSIZE(this->squares); row++)
		{
			auto result_row = this->FindMissingNumbersInRow(row);
			if (result_row.first.size() == 1)
			{
				this->squares[row][result_row.second.at(0)] = result_row.first.at(0);
				//printf("Added [%d][%d] -> %d\n", result_row.second.at(0), row, result_row.first.at(0));
				number_added = true;
				break;
			}
			else
			{
				for (auto missing_number : result_row.first)
				{
					int blocked_count = 0;
					for (auto missing_index : result_row.second)
					{ 
						int box = ((row / 3) * 3) + (missing_index / 3);
						auto result_box = this->FindMissingNumbersInBox(box);
						auto result_column = this->FindMissingNumbersInColumn(missing_index);

						// Checking for found numbers in row, column, and box
						std::vector<int> found_numbers;
						for (int k = 1; k <= 9; k++)
						{
							if ((std::find(result_row.first.begin(), result_row.first.end(), k) == result_row.first.end()) && (std::find(found_numbers.begin(), found_numbers.end(), k) == found_numbers.end())) found_numbers.push_back(k);
							if ((std::find(result_column.first.begin(), result_column.first.end(), k) == result_column.first.end()) && (std::find(found_numbers.begin(), found_numbers.end(), k) == found_numbers.end())) found_numbers.push_back(k);
							if ((std::find(result_box.first.begin(), result_box.first.end(), k) == result_box.first.end()) && (std::find(found_numbers.begin(), found_numbers.end(), k) == found_numbers.end())) found_numbers.push_back(k);
						}

						if (found_numbers.size() == 8 && std::find(found_numbers.begin(), found_numbers.end(), missing_number) == found_numbers.end())
						{
							this->squares[row][missing_index] = missing_number;
							//printf("Added Special [%d][%d] -> %d\n", missing_index, row, missing_number);
							number_added = true;
							break;
						}

						if (std::find(result_column.first.begin(), result_column.first.end(), missing_number) == result_column.first.end())
						{
							// Number is already in column so it cannot be in this index of the row
							blocked_count++;
							continue;
						}
						//else
						//{
						//	int old_blocked_count = blocked_count;

						//	for (auto missing_index_column : result_column.second)
						//	{
						//		int current_box = ((missing_index_column / 3) * 3) + (missing_index / 3);
						//		auto result_current_box = this->FindMissingNumbersInBox(current_box);

						//		// Check if the other 2 columns in the box are blocked. If so, then the number has to be in our current column.
						//		for (auto missing_number_box : result_current_box.first)
						//		{
						//			if (missing_number_box == missing_number)
						//			{
						//				int column_check_1 = ((missing_index / 3) * 3) + ((missing_index + 1) % 3);
						//				int column_check_2 = ((missing_index / 3) * 3) + ((missing_index + 2) % 3);

						//				auto result_column_check_1 = this->FindMissingNumbersInColumn(column_check_1);
						//				auto result_column_check_2 = this->FindMissingNumbersInColumn(column_check_2);

						//				if ((std::find(result_column_check_1.first.begin(), result_column_check_1.first.end(), missing_number) == result_column_check_1.first.end()) &&
						//					(std::find(result_column_check_2.first.begin(), result_column_check_2.first.end(), missing_number) == result_column_check_2.first.end()))
						//				{
						//					// Other 2 columns are blocked in this box so the missing number must be in the current column which makes it so it can't be in this index of the row
						//					blocked_count++;
						//					printf("Blocked due to vertical potentials!\n");
						//					break;
						//				}
						//			}
						//		}

						//		if (old_blocked_count < blocked_count) break;
						//	}

						//	if (old_blocked_count < blocked_count) continue;
						//}

						if (std::find(result_box.first.begin(), result_box.first.end(), missing_number) == result_box.first.end())
						{
							// Number is already in box so it cannot be in this index of the row
							blocked_count++;
							continue;
						}
					}

					if (number_added) break;

					if (blocked_count == (result_row.first.size() - 1))
					{
						for (auto missing_index : result_row.second)
						{
							int box = ((row / 3) * 3) + (missing_index / 3);
							auto result_box = this->FindMissingNumbersInBox(box);
							auto result_column = this->FindMissingNumbersInColumn(missing_index);
							if ((std::find(result_column.first.begin(), result_column.first.end(), missing_number) != result_column.first.end()) && (std::find(result_box.first.begin(), result_box.first.end(), missing_number) != result_box.first.end()))
							{
								this->squares[row][missing_index] = missing_number;
								//printf("Added [%d][%d] -> %d\n", missing_index, row, missing_number);
								number_added = true;
							}
						}

						break;
					}
				}

				if (number_added) break;
			}
		}

		// Check columns
		for (int column = 0; column < ARRAYSIZE(this->squares[0]); column++)
		{
			auto result_column = this->FindMissingNumbersInColumn(column);
			if (result_column.first.size() == 1)
			{
				this->squares[result_column.second.at(0)][column] = result_column.first.at(0);
				//printf("Added [%d][%d] -> %d\n", column, result_column.second.at(0), result_column.first.at(0));
				number_added = true;
				break;
			}
			else
			{
				for (auto missing_number : result_column.first)
				{
					int blocked_count = 0;
					for (auto missing_index : result_column.second)
					{
						auto result_row = this->FindMissingNumbersInRow(missing_index);
						if (std::find(result_row.first.begin(), result_row.first.end(), missing_number) == result_row.first.end())
						{
							// Number is already in row so it cannot be in this index of the column
							blocked_count++;
							continue;
						}
						//else
						//{
						//	int old_blocked_count = blocked_count;

						//	for (auto missing_index_row : result_row.second)
						//	{
						//		int current_box = ((missing_index_row / 3) * 3) + (missing_index / 3);
						//		auto result_current_box = this->FindMissingNumbersInBox(current_box);

						//		// Check if the other 2 columns in the box are blocked. If so, then the number has to be in our current column.
						//		for (auto missing_number_box : result_current_box.first)
						//		{
						//			if (missing_number_box == missing_number)
						//			{
						//				int row_check_1 = ((missing_index / 3) * 3) + ((missing_index + 1) % 3);
						//				int row_check_2 = ((missing_index / 3) * 3) + ((missing_index + 2) % 3);

						//				auto result_row_check_1 = this->FindMissingNumbersInRow(row_check_1);
						//				auto result_row_check_2 = this->FindMissingNumbersInRow(row_check_2);

						//				if ((std::find(result_row_check_1.first.begin(), result_row_check_1.first.end(), missing_number) == result_row_check_1.first.end()) &&
						//					(std::find(result_row_check_2.first.begin(), result_row_check_2.first.end(), missing_number) == result_row_check_2.first.end()))
						//				{
						//					// Other 2 rows are blocked in this box so the missing number must be in the current row which makes it so it can't be in this index of the column
						//					blocked_count++;
						//					printf("Blocked due to horizontal potentials!\n");
						//					break;
						//				}
						//			}
						//		}

						//		if (old_blocked_count < blocked_count) break;
						//	}

						//	if (old_blocked_count < blocked_count) continue;
						//}

						int box = ((missing_index / 3) * 3) + (column / 3);
						auto result_box = this->FindMissingNumbersInBox(box);
						if (std::find(result_box.first.begin(), result_box.first.end(), missing_number) == result_box.first.end())
						{
							// Number is already in box so it cannot be in this index of the column
							blocked_count++;
							continue;
						}
					}

					if (blocked_count == (result_column.first.size() - 1))
					{
						for (auto missing_index : result_column.second)
						{
							int box = ((missing_index / 3) * 3) + (column / 3);
							auto result_box = this->FindMissingNumbersInBox(box);
							auto result_row = this->FindMissingNumbersInRow(missing_index);
							if ((std::find(result_row.first.begin(), result_row.first.end(), missing_number) != result_row.first.end()) && (std::find(result_box.first.begin(), result_box.first.end(), missing_number) != result_box.first.end()))
							{
								this->squares[missing_index][column] = missing_number;
								//printf("Added [%d][%d] -> %d\n", column, missing_index, missing_number);
								number_added = true;
							}
						}

						break;
					}
				}

				if (number_added) break;
			}
		}

	} while (number_added);

	return this->PuzzleSolved();
}

// Print the puzzle
void Sudoku::Print()
{
	for (int i = 0; i < ARRAYSIZE(this->squares); i++)
	{
		for (int j = 0; j < ARRAYSIZE(this->squares[i]); j++)
		{
			printf("%d ", this->squares[i][j]);
		}

		printf("\n");
	}

	printf("\n");
}