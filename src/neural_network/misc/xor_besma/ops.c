#include "ops.h"

int _not(int input)
{
	if (input)
		return 0;
	return 1;
}

int nand(int input1, int input2)
{
	int bias = 3;
	int w1 = -2;
	int w2 = -1;

	return (input1 * w1 + input2 * w2 + bias > 0);
}

int _or(int input1, int input2)
{
        int bias = 0;
        int w1 = 1;
        int w2 = 1;

        return (input1 * w1 + input2 * w2 + bias > 0);
}

int _and(int input1, int input2)
{
        int bias = -2;
        int w1 = 2;
        int w2 = 1;

        return (input1 * w1 + input2 * w2 + bias > 0);
}

int _xor(int input1, int input2)
{
	int NANDNode = nand(input1, input2);
	int ORNode = _or(input1, input2);

	int FinalNode = _and(NANDNode, ORNode);

	return FinalNode > 0;
}

/*double sigmoid(double z)
{
	return 1.0 / (1.0 + exp(-z));
}*/
