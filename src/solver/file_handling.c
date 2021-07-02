#include "file_handling.h"

void read_file(char *name, int grid[9][9])
{
	FILE *file = fopen(name, "r");
	int c;
	int i = 0;
	int j = 0;

	while ((c = fgetc(file)) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\r' || c == '\0' || c == '\f')
		{
			continue;
		}

		if (c == '.')
		{
			grid[i][j] = 0;
			j += 1;
		}
		else
		{
			grid[i][j] = (int) c - 48;
			j += 1;
		}

		if (j == 9)
		{
			i += 1;
			j = 0;
		}
	}

	fclose(file);

}

size_t my_str_len(char str[])
{
	size_t len = 0;

	while (str[len] != '\0')
	{
		len += 1;
	}

	return len;

}

void write_file(char *name, int grid[9][9])
{
	// crée le nom du fichier sous le bon format

	char result_name[my_str_len(name) + 7 + 1];

	for (size_t i = 0; i < my_str_len(name); i++)
	{
		result_name[i] = name[i];
	}

	char extension[] = ".result";

	for (size_t i = 0; i < my_str_len(extension); i++)
	{
		result_name[my_str_len(name) + i] = extension[i];
	}

	result_name[my_str_len(name) + 7] = '\0'; // dernier character 0 : fin de la string


	// fin écriture nom fichier


	FILE *file;

	file = fopen(result_name, "w");


	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			fputc(48 + grid[i][j], file);
			if ((j + 1) % 3 == 0 && (j != 8))
			{
				fputc(32,file);
			}
		}

		if ((i + 1) % 3 == 0)
		{
			fputc(10, file);
		}
		fputc(10, file);
	}

	fclose(file);
}
