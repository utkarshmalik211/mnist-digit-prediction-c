// include basic c libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // contains uint_8 for faster processing of labels
#include <string.h>

// location string
char *loc;
//threshhold
int threshold;
// gui / browse ??
typedef enum GUITYPE {DRAW, BROWSE} GUITYPE;

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
								GUITYPE GUITYPE;
								Network *nn = createNetwork(28*28,20,10);
								int epoch = 1;
								//epoch - no of times it will  train
								clearScreen();
								char temp;
								for(int i=0; i<epoch; i++) {
																trainNet(nn,i+1);
																testNet(nn,i+1);
								}

								char c = 'y';
								while(c == 'y') {

																printf("Input type :\n1.Draw\n2.Browse\n3.About\n4.Exit :(\n");
																int choice;
																scanf("%d",&choice);
																scanf("%c",&temp);

																if (choice == 1 ) GUITYPE = DRAW;
																else if(choice == 2) GUITYPE = BROWSE;
																else if(choice == 3) system("./about.sh"); //call About
																else if(choice == 4) break;
																else {
																								clearScreen();
																								printf("Invalid Choice!\n");
																								continue;
																}
																if(GUITYPE == BROWSE) {
																								open_browse(argc, argv);
																								//open browse menu
																								convert_to_MNIST_Image(nn,loc);
																								//convert the browsed image  to mnist constraints

																}
																else if(GUITYPE == DRAW) {
																								opengui(argc, argv);
																								//open drawing area
																								convert_to_MNIST_Image(nn,"test_pics/temp.png");
																								//convert the drawn image  to mnist constraints
																}

																printf("Reopen?(y/n)");
																scanf("%c",&c);
																scanf("%c",&temp);
																clearScreen();
								}
								free(nn);
}
