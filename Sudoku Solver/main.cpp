#include <Windows.h>
#include <iostream>
#include "sudoku.h"

int main()
{
	SetConsoleTitleA("Sudoku Solver by Kix");

	Sudoku* game = new Sudoku();

	if (!game->Initialize())
	{
		MessageBoxA(nullptr, "Sudoku Solver - Error", "Failed to initialize!", MB_OK);
		return -1;
	}

	if (!game->Solve())
	{
		MessageBoxA(nullptr, "Sudoku Solver - Error", "Failed to solve!", MB_OK);
		return -1;
	}

	game->Print();

	system("pause");

	return 0;
}