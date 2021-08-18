#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>
#include "../tools/tools.h"
#include <time.h>
#include <float.h>
#include <err.h>
#include <string.h>
//#include "mnist_loader.h"

struct Network
{
	int nb_input;
	int nb_output;
	int nb_hidden;

	// learning rate
	
	long eta;

	// accuracy
	long accuracy;
	long max_accuracy;

	long error_rate;

	// layers
	long input[784];
	long hidden[50];
	long output[9];

	long softmax_layer[9];


	// weights w_{layer_1}{layer_2}
	
	long w_i_h[50][784];
	long w_h_o[9][50];


	// biases b_{hidden / output}
	
	long b_h[50];
	long b_o[9];

	// error vectors err_{hidden / output}
	
	long err_o[9];
	long err_h[50];

	// goal vector
	
	long goal[9];

	// delta matrices (weights and biases)
	
	long d_w_i_h[50][784];
	long d_w_h_o[9][50];

	long d_b_h[50];
	long d_b_o[9];
};

/*----------------Initialize network--------------------*/
struct Network *Init();
void FillGoal(struct Network *net, int val);
void LoadTrainingSample(struct Network *net, char *img_path);
void Load(struct Network *net, char *img_path);

/*------Feed Forward - Back Propagation - Training------*/

void FeedForward(struct Network *net);

void BackPropagation(struct Network *net);

void UpdateWeights(struct Network *net);

void UpdateBiases(struct Network *net);

int GetResult(struct Network *net);

void OCR_TrainingSample(struct Network *net, char *img_path);


void Train(struct Network *net, int nbEpoch);

float GetValidationAccuracy(struct Network *net);

void MeanSquaredError(struct Network *net);
/*---------------Miscellaneous functions----------------*/
long Random();
long Sigmoid(double x);
long d_Sigmoid(double x);
void Shuffle(int *array, int size);
void Softmax(struct Network *net);
void d_Softmax(struct Network *net);

/*----------------------File Handling-------------------*/

struct Network *LoadNetwork(char *path);

void SaveNetwork(struct Network *net, char *path);

void BuildGridToSolve();
/*-----------------------Detection----------------------*/

int DetectDigit(struct Network *net, char *img_path);

int OCR(char *img_path);

/*-------------------Display Results*--------------------*/

void DisplayResultsTrainingSample(struct Network *net);

#endif
