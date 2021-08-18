# Ocr Sudoku Solver
C application to solve a sudoku from input image (cs group project for 3rd semester at EPITA)

# Description
The application takes an image containing a sudoku as an input, processes the image, builds the grid to solve, and prints the solved sudoku.
To detect the digits, I implemented a neural network, trained on binarized 28x28 digits stored in the "images" folder.

All the code is written in C, and I used the SDL library to manipulate the images. I wrote most of the code, except for the binarization algorithm (using otsu's method), written by another memeber of my group.

But the project is incomplete, the neural network doesn't work as expected. The NN perfoms well on the training set, but fails to recognize new images.
Also, the line detection method (using hough transform and sobel filters) is relatively slow.

The Sudoku solving algorithm I used is from GeeksForGeeks (I could have tried to write one myself but I ran out of time)

Also, I wasn't able to find a way to resize large images and conserve the aspect ratio (using the SDL library), so when the images are too big the application can't display them entirely.

# Getting started
> To launch the UI: go to "ocr-sudoku-solver/src/gui", enter "make", and enter "./ui". Then select a grid and click on "Solve Sudoku".

# TODO
- Fix NN training
- Find a way to resize large images
- optimize line detection algorithm to decrease run time

# Authors

Sanassi Mory Cisse
sanassicisse@outlook.fr
