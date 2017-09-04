#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>

#include "neural_net.h"
#include "read_mnist.h"
#include "train_net.h"

void main(int argc, char *argv[]){
	FILE *trainImageFile = openMNISTImageFile("data/train-images-idx3-ubyte");
	FILE *trainLableFile = openMNISTImageFile("data/train-labels-idx1-ubyte");

	FILE *testImageFile = openMNISTImageFile("data/t10k-images-idx3-ubyte");
	FILE *testLableFile = openMNISTImageFile("data/t10k-labels-idx1-ubyte");

	IplImage* img = 0; 
  	int height,width,step,channels;
  	uchar *data;
 
	if(argc<2){
	printf("Usage: main <image-file-name>\n\7");
	exit(0);
	}
 
  	// load an image  
  	img=cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  	if(!img){
    	printf("Could not load image file: %s\n",argv[1]);
    	exit(0);
  	}

	Network *a = createNetwork(748,20,10);
	initNetwork(a,748,20,10);
	initWeights(a,HIDDEN);
	initWeights(a,OUTPUT);

	for (int i = 0; i < 59990; i++)
	{
		MNIST_Image img = getImage(trainImageFile);
		MNIST_Label lbl = getLabel(trainLableFile);
		Vector *image = getVectorFromImage(&img);
		feedInput(a,image);
		feedForwardNetwork(a);
		backPropagateNetwork(a,lbl);
        // printf("\rIn progress %d", i/600);
        printf("%d   %d\n",getNetworkClassification(a),lbl);
	}
	printf("\n");
	for (int i = 2; i < 10; i++)
	{
		MNIST_Image img = getImage(testImageFile);
		MNIST_Label lbl = getLabel(testLableFile);
		Vector *image = getVectorFromImage(&img);
		feedInput(a,image);
		feedForwardNetwork(a);
		// backPropagateNetwork(a,(int)lbl);
        // printf("\rIn progress %d", i/600);
		printf("%d   %d\n",getNetworkClassification(a),lbl);
	}

	IplImage* dst= cvCreateImage(cvSize( 28, 28), img->depth, img->nChannels );

	cvResize(img, dst,CV_INTER_AREA );
	img = dst;
	// get the image data
	height    = img->height;
	width     = img->width;
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;
	printf("Processing a %dx%d image with %d channels\n",height,width,channels); 
	// create a window
	cvNamedWindow("mainWin", CV_WND_PROP_FULLSCREEN); 
	cvMoveWindow("mainWin", 100, 100);
	double b[28][28];
	// invert the image
	for(int i=0;i<height;i++){ 
		for(int j=0;j<width;j++){ 
			for(int k=0;k<channels;k++)
				data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
			b[i][j] = ((int) data[i*step+j*channels] == 0) ? 1:0;
		}
	} 
	// for(i=0;i<28;i++){
	//   for(j=0;j<28;j++){
	//     printf("%d",(int)a[i][j]);
	//   }
	// }
	Vector *image1 = (Vector*)malloc(sizeof(double)+(sizeof(double)*784));
	image1->size = 28*28;
	for(int i=0;i<28;i++){
	  for(int j=0;j<28;j++){
	   image1->vals[i*28 + j]=b[i][j];
	  }
	}
	feedInput(a,image1);
	feedForwardNetwork(a);
	printf("%d\n",getNetworkClassification(a));
	// show the image
	cvShowImage("mainWin", img ); 
	// wait for a key
	cvWaitKey(0); 
	// release the image
	cvReleaseImage(&img );
	printf("\n");

	}