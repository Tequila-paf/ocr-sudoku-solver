# Ocr Sudoku Solver
C application to solve a sudoku from input image (cs group project for 3rd semester at EPITA)

# Description
The application takes an image containing a sudoku as an input, processes the image, builds the grid to solve, and prints the solved sudoku.
To detect the digits, I implemented a neural network, trained on binarized 28x28 digits stored in the "images" folder.

All the code is written in C, and I used the SDL library to manipulate the images. I wrote most of the code, except for the binarization algorithm (using otsu's method), written by another memeber of my group.

But the project is incomplete, the neural network doesn't work as expected. The NN perfoms well on the training set, but fails to recognize new images.


# AUTHORS

Sanassi Mory Cisse
sanassicisse@outlook.fr
