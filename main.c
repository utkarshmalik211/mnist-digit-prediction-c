// include basic c functions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// include maths functions
#include <math.h>
//include neural network functions
#include "neural_net.h"
#include "read_mnist.h"
#include "train_net.h"
//include image Processing
#include "preprocess_image.h"


void main(int argc, char *argv[]){
								FILE *trainImageFile = openMNISTImageFile("data/train-images-idx3-ubyte");
								FILE *trainLableFile = openMNISTImageFile("data/train-labels-idx1-ubyte");

								Network *a = createNetwork(28*28,20,10);
								initNetwork(a,748,20,10);
								initWeights(a,HIDDEN);
								initWeights(a,OUTPUT);
								clearScreen();
								for (int i = 0; i < 55000; i++) {
																MNIST_Image img1 = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img1);
																feedInput(a,image);
																feedForwardNetwork(a);
																backPropagateNetwork(a,lbl);
																printf("\rIn progress %d", i/600);
																// printf("%d   %d\n",getNetworkClassification(a),lbl);
								}
								printf("\n");
								// for (int i = 0; i < 20; i++){
								//  MNIST_Image img = getImage(testImageFile);
								//  MNIST_Label lbl = getLabel(testLableFile);
								//  Vector *image = getVectorFromImage(&img);
								//  feedInput(a,image);
								//  feedForwardNetwork(a);
								//  // backPropagateNetwork(a,(int)lbl);
								//      // printf("\rIn progress %d", i/600);
								//  printf("%d   %d\n",getNetworkClassification(a),lbl);
								// }

								Vector *image = convert_to_MNIST_Image(argv[1]);
								feedInput(a,image);
								feedForwardNetwork(a);
								printf("%d\n",getNetworkClassification(a));
}
