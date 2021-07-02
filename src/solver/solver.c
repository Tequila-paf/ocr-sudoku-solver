#include "solver.h"

void SolveGrid(char *grid_input)
{
	int grid[9][9];

	read_file(grid_input, grid);

	
	if (solveSudoku(grid, 0, 0) == 1)
	{
		
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				printf("%i", grid[i][j]);

				if ((j + 1) % 3 == 0)
				{
					printf(" ");
				}
			}

			printf("\n");

			if ((i + 1) % 3 == 0)
			{
				printf("\n");
			}
		}
		
	}
	
	write_file(grid_input, grid);

}
/*
int main(int argc, char ** argv)
{
	if (argc != 2)
		return 1;
	
	SolveGrid(argv[1]);
	return 0;
}
*/
/*
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return EXIT_FAILURE;
	}
	int grid[9][9];

	read_file(argv[1], grid);

	
	if (solveSudoku(grid, 0, 0) == 1)
	{
		
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				printf("%i", grid[i][j]);

				if ((j + 1) % 3 == 0)
				{
					printf(" ");
				}
			}

			printf("\n");

			if ((i + 1) % 3 == 0)
			{
				printf("\n");
			}
		}
		
	}

	

	else
	{
		return EXIT_FAILURE;
	}

	write_file(argv[1], grid);


	return EXIT_SUCCESS;

}
*/
