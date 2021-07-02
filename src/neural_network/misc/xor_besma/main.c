#include "ops.h"

int main(int argc, char **argv)
{
	if (argc == 3)
	{
		/*int height = argv[0];
		int width = argv[1];
		unsigned long input_layer = height * width;
		unsigned long hidden_layer = 15;
		unsigned long output_layer = 10;*/

		char *a = argv[1];
		char *b = argv[2];
		int input1 = atoi(a);
		int input2 = atoi(b);

		int XorFinal = _xor(input1, input2);

		printf("Input : [%d, %d, 1(bias)], Output : %d\n", input1, input2, XorFinal);
	}

	return 0;
}
