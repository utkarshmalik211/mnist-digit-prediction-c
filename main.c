// include basic c functions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// include maths functions
#include <math.h>
// include graphics lib
#include <gtk/gtk.h>

//include neural network functions
#include "neural_net.h"
#include "read_mnist.h"
#include "train_net.h"

//include image Processing
#include "preprocess_image.h"

#include "gui.h"

void displayProgress(float iter,float total){
								char c[101];
								float point = (iter/(total-1))*100;
								for(int i = 0; i < point; i++) {
																c[i]='#';
								}
								for(int i = point; i < 100; i++) {
																c[i]='-';
								}
								c[100]='\0';
								printf("|%s|\r",c);
}

Vector *getVectorFromImage(MNIST_Image *img){
								Vector *v = (Vector*)malloc(sizeof(Vector)+(28*28*sizeof(double)));
								v->size = 28*28;
								for(int i = 0; i <v->size; i++) {
																v->vals[i] = img->pixel[i] ? 1 : 0;
								}
								return v;
}

void trainNet(Network *nn){
								FILE *trainImageFile = openMNISTImageFile("data/train-images-idx3-ubyte");
								FILE *trainLableFile = openMNISTLabelFile("data/train-labels-idx1-ubyte");
								printf("Training Network : \n");
								int count = 0;
								for (int i = 0; i < 60000; i++) {
																MNIST_Image img = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img);
																feedInput(nn,image);
																feedForwardNetwork(nn);
																backPropagateNetwork(nn,(int) lbl);
																displayProgress(i,60000);
								}
								printf("\n");
								fclose(trainLableFile);
								fclose(trainImageFile);
}
void testNet(Network *nn){
								FILE *trainImageFile = openMNISTImageFile("data/t10k-images-idx3-ubyte");
								FILE *trainLableFile = openMNISTLabelFile("data/t10k-labels-idx1-ubyte");
								printf("Testing Network :\n");
								int count = 10000;
								for (int i = 0; i < 10000; i++) {
																MNIST_Image img = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img);
																feedInput(nn,image);
																feedForwardNetwork(nn);
																if(lbl!=getNetworkClassification(nn)) {
																								count--;
																}
																backPropagateNetwork(nn,(int) lbl);
																displayProgress(i,10000);
								}
								printf("\nTrain set accuracy achieved = %d percent	\n",(count/100));
								fclose(trainLableFile);
								fclose(trainImageFile);
}

void main(int argc, char *argv[]){
								opengui(argc, argv);
								Network *nn = createNetwork(28*28,20,10);
								clearScreen();
								trainNet(nn);
								testNet(nn);

								Vector *image = convert_to_MNIST_Image(argv[1]);
								feedInput(nn,image);
								feedForwardNetwork(nn);
								printf("Predicted output for above image is : %d\n",getNetworkClassification(nn));

}
