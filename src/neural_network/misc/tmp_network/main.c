#include "neural_network.h"
 
 // Colors 
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

int main(int argc, char **argv)
{
	if (!(argc == 3 || argc == 2 || argc == 4))
	{
		errx(1, "Error : incorrect arguments number\n");
	}

	if (!strcmp(argv[1], "-help"))
	{
		printf("\n\n\t+---------------------- Help ------------------------+\n\n");

		printf("\t  - main : args :  -{train *[eta]/ ocr / ocr-sample}\n");

		printf("\t    {nbEpoch / img_path (from images file /path)}   \n");

		printf("\t                                                    \n");

		printf("\t  - train: init new network ;trains for nbEpochs    \n");

		printf("\t    training samples path: ../images; init eta      \n");

		printf("\t                                                    \n");

		printf("\t  - ocr: detects digit in image given as argument   \n");

		printf("\t                                                    \n");

		printf("\t  - ocr-sample: detect digit in image from training \n");

		printf("\t    sample - format : ../images/{digit}/{#}         \n");

		printf("\n\t    ex: ./main -train 5000 0.6		      \n");

		printf("\t    init net (eta optional)- train for nbEpoch      \n");

		printf("\n\t+----------------------------------------------------+\n\n");


	}
	if (!strcmp(argv[1], "-train"))
	{
		int nbEpoch = strtoul(argv[2], NULL, 10);

		srand(time (NULL));

		struct Network *net = Init();

		if (argc == 4)
			net -> eta = atof(argv[3]);

		printf("%f\n", net -> eta);

		Train(net, nbEpoch);

		DisplayResultsTrainingSample(net);
	}

	if (!strcmp(argv[1], "-ocr-sample"))
	{
		struct Network *net = LoadNetwork("save_files/net_info\0");

		int res = DetectDigit(net, argv[2]);

		printf("Expected: " BLUE "%c" RESET "  |  Got: " YELLOW " %i " RESET, argv[2][10], res);

		if (res == argv[2][10] - 48)
			printf(GREEN " OK\n" RESET);
		else
			printf(RED " FOK\n" RESET);

		printf("\n+-----------------Softmax Layer---------------------+\n");

		for (int i = 0; i < net -> nb_output; i++)
		{
			printf("%.3f ", net -> softmax_layer[i]);
		}

		printf("\n+---------------------------------------------------+\n");
	}
	
	if (!strcmp(argv[1], "-ocr"))
	{
		int digit = OCR(argv[2]);

		printf("\nThe digit is a " GREEN "%i\n" RESET, digit);
	}

	if (!strcmp(argv[1], "-build"))
	{
		BuildResultFile();
	}

	return EXIT_SUCCESS;
}
/*
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		return EXIT_FAILURE;
	}

	if (argv == NULL)
	{
		return EXIT_FAILURE;
	}

	srand(time (NULL));

	struct Network *net = LoadNetwork("save_files/net_info\0");
	
//	struct Network *net = Init();
	
//	Train(net, 50000);

	char path[19] = "../images/0/0.bmp\0";

	int nbOK = 0;
	int nb = 0;

	for (int epoch = 0; epoch < 50000; epoch++)
	{
		for (int i = 0; i <= 9; i++)
		{
			path[12] = i + 48;
	
			for (int j = 1; j <= 9; j++)
			{
				path[10] = j + 48;
	
				OCR_TrainingSample(net, path);
				int res = GetResult(net);

				if (res == path[10] - 48)
				{
					nbOK += 1;
				}

				nb += 1;
			}
		}

		if (epoch % 100 == 0)
		{
			printf("%i : %i / %i\n", epoch, nbOK, nb);

		}
	}
	printf("%i : %i / %i\n", 5000, nbOK, nb);

	SaveNetwork(net, "save_files/net_info");
	
	DisplayResultsTrainingSample(net);
	printf("%i\n\n", DetectDigit(net, argv[1]));

	for (int i = 0; i < net -> nb_output; i++)
	{
		printf("%f ", net -> output[i]);
	}
	free(net);
	return EXIT_SUCCESS;
}

*/
