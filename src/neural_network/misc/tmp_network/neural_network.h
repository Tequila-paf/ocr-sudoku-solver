#ifndef NEURAL_NETWORK_H_
#define NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>
#include "../tools/tools.h"
#include <time.h>
#include <float.h>
#include <err.h>
#include <string.h>

struct Network
{
	int nb_input;
	int nb_output;
	int nb_hidden;

	// learning rate
	
	double eta;

	// accuracy
	double accuracy;
	double max_accuracy;

	// layers
	double input[784];
	double hidden[30];
	double output[9];

	double softmax_layer[9];


	// weights w_{layer_1}{layer_2}
	
	double w_i_h[30][784];
	double w_h_o[9][30];


	// biases b_{hidden / output}
	
	double b_h[30];
	double b_o[9];

	// error vectors err_{hidden / output}
	
	double err_o[9];
	double err_h[30];

	// goal vector
	
	double goal[9];

	// delta matrices (weights and biases)
	
	double d_w_i_h[30][784];
	double d_w_h_o[9][30];

	double d_b_h[30];
	double d_b_o[9];
};

/*----------------Initialize network--------------------*/
struct Network *Init();
void FillGoal(struct Network *net, int val);
void LoadTrainingSample(struct Network *net, char *img_path);

/*------Feed Forward - Back Propagation - Training------*/

void FeedForward(struct Network *net);

void BackPropagation(struct Network *net);

void UpdateWeights(struct Network *net);

void UpdateBiases(struct Network *net);

int GetResult(struct Network *net);

void OCR_TrainingSample(struct Network *net, char *img_path);

void Train(struct Network *net, int nbEpoch);

float GetValidationAccuracy(struct Network *net);
/*---------------Miscellaneous functions----------------*/
double Random();
double Sigmoid(double x);
double d_Sigmoid(double x);
void Shuffle(int *array, int size);
void Softmax(struct Network *net);
void d_Softmax(struct Network *net);

/*----------------------File Handling-------------------*/

struct Network *LoadNetwork(char *path);

void SaveNetwork(struct Network *net, char *path);

void BuildResultFile();
/*-----------------------Detection----------------------*/

int DetectDigit(struct Network *net, char *img_path);

int OCR(char *img_path);

/*-------------------Display Results*--------------------*/

void DisplayResultsTrainingSample(struct Network *net);

#endif
