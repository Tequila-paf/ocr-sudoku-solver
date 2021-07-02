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
		init_sdl();

		int nbEpoch = strtoul(argv[2], NULL, 10);

		srand(time (NULL));

		struct Network *net = Init();

		if (argc == 4)
			net -> eta = atof(argv[3]);

		printf("%f\n", net -> eta);

		Train(net, nbEpoch);
				
		DisplayResultsTrainingSample(net);
		
		free(net);
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
		BuildGridToSolve();
	}

	if (!strcmp(argv[1], "-display"))
	{
		struct Network *net = LoadNetwork("../neural_network/save_files/net_info");

		DisplayResultsTrainingSample(net);

		free(net);
	}


	return EXIT_SUCCESS;
}
