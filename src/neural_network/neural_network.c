#include "neural_network.h"
 // Colors 
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

#define NB_IMAGES 50
#define NB_DIRECTORIES 9

struct Network *Init()
{
	struct Network *net = malloc(sizeof (struct Network));

	net -> nb_input = 784;
	net -> nb_output = 9;
	net -> nb_hidden = 30; 

	net -> eta = 0.5;

	net -> max_accuracy = -1;
	net -> accuracy = -1;

	net -> error_rate = 0;


	for (int i = 0; i < net -> nb_hidden; i++)
	{
		for (int j = 0; j < net -> nb_input; j++)
		{
			net -> w_i_h[i][j] = Random();
		}

		net -> b_h[i] = Random();
	}

	for (int i = 0; i < net -> nb_output; i++)
	{
		for (int j = 0; j < net -> nb_hidden; j++)
		{
			net -> w_h_o[i][j] = Random();

			net -> softmax_layer[i] = 0;
		}

		net -> b_o[i] = Random();
	}


	return net;

}

void FeedForward(struct Network *net)
{
	double sum, w, b, in; // w: weight; b : bias; in: input neuron activation; sum: weighted sum

	// input -> hidden
	for (int i = 0; i < net -> nb_hidden; i++)
	{
		sum = 0;

		for (int j = 0; j < net -> nb_input; j++)
		{
			w = net -> w_i_h[i][j];
			in = net -> input[j];

			sum += w * in;
		}

		b = net -> b_h[i];
		sum += b;

		net -> hidden[i] = Sigmoid(sum);
	}

	// hidden -> output
	
	for (int i = 0; i < net -> nb_output; i++)
	{
		sum = 0;

		for (int j = 0; j < net -> nb_hidden; j++)
		{
			w = net -> w_h_o[i][j];
			in = net -> hidden[j];

			sum += w * in;
		}

		b = net -> b_o[i];
		sum += b;

		net -> output[i] = Sigmoid(sum);
	}
}

void FillGoal(struct Network *net, int val)
{
	for (int i = 0; i < net -> nb_output; i++)
	{
		net -> goal[i] = 0;
	}

	net -> goal[val] = 1;
}

void Load(struct Network *net, char *img_path)
{
	init_sdl();

	/*begin init input vector from img pixel values*/
	SDL_Surface *img = load_image(img_path);

	Uint32 pixel;
	Uint8 r, g, b;
	
	for (int i = 0; i < img -> h; i++)
	{
		for (int j = 0; j < img -> w; j++)
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				net -> input[i * (img -> w) + j] = 1;
			}
			else
			{
				net -> input[i * (img -> w) + j] = 0;
			}
		}
	}


	/*end init input vector from img pixel values*/

	//SDL_FreeSurface(img);
	free(img);
	

}

int OCR(char *img_path)
{
	struct Network *net = LoadNetwork("../neural_network/save_files/net_info\0");
	
	Load(net, img_path);
	
	FeedForward(net);
		
	int res = GetResult(net);
		
	free(net);

	return res;
}


void LoadTrainingSample(struct Network *net, char *img_path)
{
	/*begin init input vector from img pixel values*/
	SDL_Surface *img = load_image(img_path);
	
	if (img == NULL)
	{
		return;
	}

	Uint32 pixel;
	Uint8 r, g, b;

	for (int i = 0; i < img -> h; i++)
	{
		for (int j = 0; j < img -> w; j++)
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0 && g == 0 && b == 0)
			{
				net -> input[i * (img -> w) + j] = 1;
			}
			else
			{
				net -> input[i * (img -> w) + j] = 0;
			}
		}
	}


	/*end init input vector from img pixel values*/

	SDL_FreeSurface(img);

	FillGoal(net, img_path[10] - 49);
}

void MeanSquaredError(struct Network *net)
{
	double sum = 0;

	for (int i = 0; i < net -> nb_output; i++)
	{
		sum += (net -> goal[i] - net -> output[i]) * (net -> goal[i] - net -> output[i]);
	}

	net -> error_rate += sum;
}

void BackPropagation(struct Network *net)
{
	/*-----------------output layer errors--------------------*/
	for (int i = 0; i < net -> nb_output; i++)
	{
		net -> err_o[i] = (net -> goal[i] - net -> output[i]); 
	}


	/*-----------------hidden layer errors--------------------*/
	double sum, w, err; // sum: weighted sum; w: weight; err: error from next layer
	
	double w_h_o_t[net -> nb_hidden][net -> nb_output]; // w_h_o_t: transpose matrix of w_h_o

	/*begin transpose operation*/

	for (int i = 0; i < net -> nb_output; i++)
	{
		for (int j = 0; j < net -> nb_hidden; j++)
		{
			w_h_o_t[j][i] = net ->  w_h_o[i][j];
		}
	}

	/*end transpose operation*/


	for (int i = 0; i < net -> nb_hidden; i++)
	{
		sum = 0;
		for (int j = 0; j < net -> nb_output; j++)
		{
			err = net -> output[j];
			w = w_h_o_t[i][j];

			sum += err * w;
		}

		net -> err_h[i] = sum;
	}
}

void UpdateWeights(struct Network *net)
{
	double eta = net -> eta;
	double err, act, d_act; // err: error from next layer act: activation from current layer; d_act: d_sigmoid of next layer activation

	// update weights between hidden and output layer (w_h_o)
	for (int i = 0; i < net -> nb_output; i++)
	{
		for (int j = 0; j < net -> nb_hidden; j++)
		{
			err = net -> err_o[i];
			d_act = d_Sigmoid(net -> output[i]);
			act = net -> hidden[j];

			net -> d_w_h_o[i][j] = eta * err * d_act * act;
			
			net -> w_h_o[i][j] += net -> d_w_h_o[i][j];
		}
	}

	// update weights between input layer and hidden layer (w_i_h)
	for (int i = 0; i < net -> nb_hidden; i++)
	{
		for (int j = 0; j < net -> nb_input; j++)
		{
			err = net -> err_h[i];
			d_act = d_Sigmoid(net -> hidden[i]);
			act = net -> input[j];

			net -> d_w_i_h[i][j] = eta * err * d_act * act;

			net -> w_i_h[i][j] += net -> d_w_i_h[i][j];
		}
	}
}


void UpdateBiases(struct Network *net)
{
	double eta = net -> eta;

	double err, d_act;
	// biases output
	
	for (int i = 0; i < net -> nb_output; i++)
	{
		err = net -> err_o[i];
		d_act = d_Sigmoid(net -> output[i]);

		net -> d_b_o[i] = eta * err * d_act;

		net -> b_o[i] += net -> d_b_o[i] ;
	}
	

	// biases hidden
	
	for (int i = 0; i < net -> nb_hidden; i++)
	{
		err = net -> err_h[i];
		d_act = d_Sigmoid(net -> hidden[i]);

		net -> d_b_h[i] = eta * err * d_act;

		net -> b_h[i] += net -> d_b_h[i];
	}
}

void OCR_TrainingSample(struct Network *net, char *img_path)
{
	LoadTrainingSample(net, img_path);

	FeedForward(net);

	BackPropagation(net);

	UpdateWeights(net);

	UpdateBiases(net);
}

int GetResult(struct Network *net)
{
	int pos = 0;
	double max_act = - DBL_MAX;

	for (int i = 0; i < net -> nb_output; i++)
	{
		if (net -> output[i] > max_act)
		{
			max_act = net -> output[i];
			pos = i;
		}
	}

	return pos + 1;
}

int DetectDigit(struct Network* net, char *img_path)
{
	LoadTrainingSample(net, img_path);
	FeedForward(net);
	Softmax(net);
	return GetResult(net);
}

float GetValidationAccuracy(struct Network *net)
{
	char path[27] = "../images/validation/0/00\0";

	int nb_dir = 9;
	int nb_img = 9;

	int size_test_data = nb_dir * nb_img;

	int nbOK = 0;


	for (int i = 1 ; i <= nb_dir; i++)
	{
		path[21] = i + 48;

		for (int j = 1;j <= nb_img; j++)
		{
			path[23] = (j / 10) + 48;
			path[24] = (j % 10) + 48;

			if (j <= 9)
			{
				path[23] = j + 48;
				path[24] = '\0';
			}

			if (DetectDigit(net, path) == i)
			{
				nbOK += 1;
			}
		}
	}


	return ( ((double) nbOK) / ((double) size_test_data)) * 100;
}

void Train(struct Network *net, int nbEpoch)
{
	char path[16] = "../images/0/00\0";

	int nbOK = 0;
	int nb = NB_IMAGES * NB_DIRECTORIES;

	int *values = malloc((NB_IMAGES + 1) * sizeof(int));
	int *dir = malloc(NB_DIRECTORIES * sizeof(int));

	for (int i = 0; i <= NB_IMAGES; i++)
	{
		values[i] = i;
	}

	for (int i = 1; i <= NB_DIRECTORIES; i++)
	{
		dir[i - 1] = i;
	}


	int rand_val, rand_dir;

	Shuffle(values, NB_IMAGES + 1);
	Shuffle(dir, NB_DIRECTORIES);
	
//	int early_exit = 0;
	
	for (int epoch = 0; epoch < nbEpoch; epoch++)
	{
		Shuffle(values, NB_IMAGES + 1);
		net -> error_rate = 0;

		for (int i = 0; i <= NB_IMAGES; i++)
		{
			rand_val = values[i];

			path[12] = (rand_val / 10) + 48;
			path[13] = (rand_val % 10) + 48;

			if (rand_val <= 9)
			{
				path[13] = '\0';
				path[12] = rand_val + 48;
			}

			Shuffle(dir, NB_DIRECTORIES);
			for (int j = 1; j <= NB_DIRECTORIES; j++)
			{
				rand_dir = dir[j - 1];

				path[10] = rand_dir + 48;

				OCR_TrainingSample(net, path);
				MeanSquaredError(net);

				//printf("\n%s \n", path);

				int res = GetResult(net);

				if (res == path[10] - 48)
					nbOK += 1;
			}
		}

		
		if (epoch % 2 == 0)
		{
			double test_accuracy = GetValidationAccuracy(net);

			net -> accuracy = test_accuracy;

			if (net -> accuracy > net -> max_accuracy)
			{
				//SaveNetwork(net, "../neural_network/save_files/net_info\0");

				net -> max_accuracy = net -> accuracy;
				printf("\n" BLUE "Epoch : %i Saved !" RESET "\n", epoch);
			}

			if (net -> max_accuracy > 80)
			{
			//	early_exit = 1;
				break;
			}


		}
		

		if (epoch % 100 == 0)
		{
			if (net -> error_rate < 30)
			{
				//SaveNetwork(net, "../neural_network/save_files/net_info\0");
				//early_exit = 1;
				break;
			}
			printf("Epoch #%i : %i / %i ; error : %f\n", epoch, nbOK, nb, net -> error_rate);
		}
		nbOK = 0;
	}

	free(values);
	free(dir);
	
	//if (early_exit == 0)
		//SaveNetwork(net, "../neural_network/save_files/net_info\0");
}

/*-------------------Miscellaneous functions----------------------*/

double Random()
{
	return ((double) rand()) / ((double) RAND_MAX / 2) - 1; ;
}

double Sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

double d_Sigmoid(double x)
{
	return x * (1 - x); // or sigmoid(x) * (1 - sigmoid(x)) : why ? x has already been "sigmoided" during feedforward
}

void Shuffle(int *array, int size)
{
	if (size > 1)
	{
		int j, t;

		for (int i = 0; i < size - 1; i++)
		{
			j = i + rand() / (RAND_MAX / (size - i) + 1);
			t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}

}

// apply softmax function to net's output layer
void Softmax(struct Network *net)
{
	double denominator = 0;

	for (int i = 0; i < net -> nb_output; i++)
	{
		denominator += exp(net -> output[i]);
	}

	for (int i = 0; i < net -> nb_output; i++)
	{
		net -> softmax_layer[i] = exp(net -> output[i]) / denominator;
	}
}
/*
void d_Softmax(struct Network *net)
{


}
*/

/*-------------------------File Handling---------------------------*/

/*Load Network (returns struct network from previously saved files)*/

void load_w_i_h(struct Network *net, char *path)
{
	FILE *fp = fopen(path, "r");
	//double value;
	char *line;
	size_t len = 0;

	for (int i = 0; i < net -> nb_hidden; i++)
	{
		for (int j = 0; j < net -> nb_input; j++)
		{
			if (getline(&line, &len, fp) != -1)
			{
				net -> w_i_h[i][j] = atof(line);
			}
			
		}
	}
	fclose(fp);
}

void load_w_h_o(struct Network *net, char *path)
{
	FILE *fp = fopen(path, "r");
	//double value;
	char *line;
	size_t len = 0;

	for (int i = 0; i < net -> nb_output; i++)
	{
		for (int j = 0; j < net -> nb_hidden; j++)
		{
			if (getline(&line, &len, fp) != -1)
			{
				net -> w_h_o[i][j] = atof(line);
			}
		}
	}

	fclose(fp);
}

void load_b_h(struct Network *net, char *path)
{
	FILE *fp = fopen(path, "r");
	//double value;
	char *line;
	size_t len = 0;

	for (int i = 0; i < net -> nb_hidden; i++)
	{
		if (getline(&line, &len, fp) != -1)
		{
			net -> b_h[i] = atof(line);
		}				
	}

	fclose(fp);
}

void load_b_o(struct Network *net, char *path)
{
	FILE *fp = fopen(path, "r");
	//double value;
	char *line;
	size_t len = 0;

	for (int i = 0; i < net -> nb_output; i++)
	{
		if (getline(&line, &len, fp) != -1)
		{
			net -> b_o[i] = atof(line);
		}
	}
	fclose(fp);
}

struct Network *LoadNetwork(char *path)
{
	struct Network *net = malloc(sizeof (struct Network));

	FILE *fp = fopen(path, "r");
	
	if (fp == NULL)
	{
		printf("net path null");
		return NULL;
	}
	//int value;
	//double eta;
	char *line;
	size_t len = 0;

	if (getline(&line, &len, fp) != -1)
	{
		net -> nb_input = (int) atof(line);
	}
	

	if (getline(&line, &len, fp) != -1)
	{
		net -> nb_output = (int) atof(line);
	}

	if (getline(&line, &len, fp) != -1)
	{
		net -> nb_hidden = (int) atof(line);
	}

	if (getline(&line, &len, fp) != -1)
	{
		net -> eta = (int) atof(line);
	}

	fclose(fp);

	load_w_i_h(net, "../neural_network/save_files/w_i_h"); 
	load_w_h_o(net, "../neural_network/save_files/w_h_o");
	load_b_h(net, "../neural_network/save_files/b_h");
	load_b_o(net, "../neural_network/save_files/b_o");

	printf("\nNetwork Loaded !\n");

	return net;
}

/*save network in 5 different files : net info - w_i_h - w_h_o - b_h - b_o*/

void save_w_i_h(struct Network* net, char *path)
{
	FILE *fp = fopen(path, "w");

	for (int i = 0; i < net -> nb_hidden; i++)
	{
		for (int j = 0; j < net -> nb_input; j++)
		{
			fprintf(fp, "%.3f\n", (float) net -> w_i_h[i][j]);
		}
	}

	fclose(fp);
}

void save_w_h_o(struct Network* net, char *path)
{
	FILE *fp = fopen(path, "w");

	for (int i = 0; i < net -> nb_output; i++)
	{
		for (int j = 0; j < net -> nb_hidden; j++)
		{
			fprintf(fp, "%.3f\n", (float) net -> w_h_o[i][j]);
		}
	}

	fclose(fp);
}

void save_b_h(struct Network* net, char *path)
{
	FILE *fp = fopen(path, "w");

	for (int i = 0; i < net -> nb_hidden; i++)
	{
		fprintf(fp, "%.3f\n", (float) net -> b_h[i]);
	}

	fclose(fp);
}

void save_b_o(struct Network* net, char *path)
{
	FILE *fp = fopen(path, "w");

	for (int i = 0; i < net -> nb_output; i++)
	{
		fprintf(fp, "%.3f\n", (float) net -> b_o[i]);
	}

	fclose(fp);
}

void SaveNetwork(struct Network* net, char *path)
{
	FILE *fp = fopen(path, "w");

	if (fprintf(fp, "%i\n",net -> nb_input) == 6)
		return;

	if(fprintf(fp, "%i\n", net -> nb_output) == 6)
		return;

	if (fprintf(fp, "%i\n", net -> nb_hidden) == 6)
		return;

	if(fprintf(fp, "%f\n", (float) net -> eta) == 6)
		return;


	save_w_i_h(net, "../neural_network/save_files/w_i_h");
	save_w_h_o(net, "../neural_network/save_files/w_h_o");
	save_b_h(net, "../neural_network/save_files/b_h");
	save_b_o(net, "../neural_network/save_files/b_o");

	fclose(fp);
}

/*------------------------------Display Results-------------------------------*/

// Colors for print
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define YELLOW "\x1b[33m"

void DisplayResultsTrainingSample(struct Network *net)
{
	char path[16] = "../images/0/00\0";

	printf("\n \t+----------Results----------+\n");
	for (int i = 0; i <= NB_IMAGES; i++)
	{
		path[12] = (i / 10) + 48;
		path[13] = (i % 10) + 48;

		if (i <= 9)
		{
			path[13] = '\0';
			path[12] = i + 48;
		}

		for (int j = 1; j <= NB_DIRECTORIES; j++)
		{
			path[10] = j + 48;

			int res = DetectDigit(net, path);
			if (res == j)
			{
				printf(" \tInput : " GREEN "%i" RESET " - " YELLOW "%i " RESET "\t|   Got: %i " BLUE "OK\n" RESET, j, i, res);
			}
			else
			{
				printf(" \tInput : " GREEN "%i" RESET " - " YELLOW "%i " RESET "\t|   Got: %i " RED "FOK\n" RESET, j, i, res);
			}
		}
		
		printf("	+----------------------------+\n");

	}

}


/*-----------------------------parse input image file----------------------------------*/

void BuildGridToSolve()
{
	FILE *fp = fopen("../image_processing/grid\0", "w");
	
	int *grid = malloc(81 * sizeof(int));


	SDL_Surface *img;
	char path[31] = "../image_processing/digits/00\0";

	for (int i = 0; i < 81; i++)
		grid[i] = 0;


	printf("\n");
	for(int i = 0; i < 81; i++)
	{
		path[27] = (i / 10) + 48;
		path[28] = (i % 10) + 48;
		
		if (i <= 9)
		{
			path[27] = i + 48;
			path[28] = '\0';
		}
		
		img = load_image(path);
		if (img == NULL)
		{
			printf(RED "NULL" RESET"%s.\n", path);
		}
		if (img != NULL)
		{	
			printf(GREEN "OK" RESET "%s.\n", path);
			grid[i] = OCR(path);
			//printf("%i ", grid[i]);
		}
	}

	printf("\n");

	free(img);
	


	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (grid[i * 9 + j] != 0)
			{
				printf("%i", grid[i * 9 + j]);
				fputc(grid[i * 9 + j] + 48, fp);
			}
			else
			{
				fputc(46, fp);
				printf(".");
			}

			if ((j + 1) % 3 == 0 && (j != 8))
			{
				fputc(32, fp);
				printf(" ");
			}
		}

		if((i + 1) % 3 == 0)
		{
			printf("\n");
			fputc(10, fp);
		}
		fputc(10, fp);
		printf("\n");
	}


	fclose(fp);
	free(grid);
}

