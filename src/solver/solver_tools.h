#ifndef SOLVER_TOOLS_H_
#define SOLVER_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>

// N is the size of the 2D matrix N*N
#define N 9

/* A utility function to print grid */
void print(int arr[N][N]);

// Checks whether it will be legal 
// to assign num to the
// given row, col
int isSafe(int grid[N][N], int row, int col, int num);

/* Takes a partially filled-in grid and attempts
to assign values to all unassigned locations in
such a way to meet the requirements for
Sudoku solution (non-duplication across rows,
columns, and boxes) */
int solveSudoku(int grid[N][N], int row, int col);

/*
int main()
{
	// 0 means unassigned cells
	int grid[N][N] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
					{ 5, 2, 0, 0, 0, 0, 0, 0, 0 },
					{ 0, 8, 7, 0, 0, 0, 0, 3, 1 },
					{ 0, 0, 3, 0, 1, 0, 0, 8, 0 },
					{ 9, 0, 0, 8, 6, 3, 0, 0, 5 },
					{ 0, 5, 0, 0, 9, 0, 6, 0, 0 },
					{ 1, 3, 0, 0, 0, 0, 2, 5, 0 },
					{ 0, 0, 0, 0, 0, 0, 0, 7, 4 },
					{ 0, 0, 5, 2, 0, 6, 3, 0, 0 } };

	if (solveSuduko(grid, 0, 0)==1)
		print(grid);
	else
		printf("No solution exists");

	return 0;
	// This is code is contributed by Pradeep Mondal P
}

*/

#endif
