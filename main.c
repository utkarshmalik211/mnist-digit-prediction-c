// include basic c functions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // contains uint_8 for faster processing of labels
#include <string.h>

// include maths functions
#include <math.h>

// include graphics lib
#include <gtk/gtk.h>

//include neural network functions
#include "func/read_mnist.h"
#include "func/neural_net.h"
#include "func/train_net.h"

//include image Processing
#include "func/preprocess_image.h"

//include functions for training and testing network + displaying progress
#include "func/main_nn_loop.h"

// include gui code that uses functions from above files
#include "func/gui.h"

void main(int argc, char *argv[]){
								Network *nn = createNetwork(28*28,20,10);
								int epoch = 13;
								clearScreen();
								for(int i=0; i<epoch; i++) {
																trainNet(nn,i+1);
																testNet(nn,i+1);
								}
								char c = 'y';
								while(c=='y') {
																opengui(argc, argv);
																Vector *image = convert_to_MNIST_Image("test_pics/temp.png");
																feedInput(nn,image);
																feedForwardNetwork(nn);
																// printf("Predicted output for above image is : %d\n",getNetworkClassification(nn));
																printNetworkClassification(nn);
																printf("Reopen Drawing Area?(y/n)\n");
																scanf("%c",&c);
								}
								free(nn);

}
