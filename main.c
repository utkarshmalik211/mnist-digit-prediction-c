// include basic c functions
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
// include opencv
#include <cv.h>
#include <highgui.h>
// include maths functions
#include <math.h>

//include neural network functions
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

	// for (int i = 0; i < 59990; i++)
	// {
	// 	MNIST_Image img = getImage(trainImageFile);
	// 	MNIST_Label lbl = getLabel(trainLableFile);
	// 	Vector *image = getVectorFromImage(&img);
	// 	feedInput(a,image);
	// 	feedForwardNetwork(a);
	// 	backPropagateNetwork(a,lbl);
  //       // printf("\rIn progress %d", i/600);
  //       printf("%d   %d\n",getNetworkClassification(a),lbl);
	// }
	// printf("\n");
	// for (int i = 0; i < 20; i++)
	// {
	// 	MNIST_Image img = getImage(testImageFile);
	// 	MNIST_Label lbl = getLabel(testLableFile);
	// 	Vector *image = getVectorFromImage(&img);
	// 	feedInput(a,image);
	// 	feedForwardNetwork(a)
	// 	// backPropagateNetwork(a,(int)lbl);
 //       // printf("\rIn progress %d", i/600);
	// 	printf("%d   %d\n",getNetworkClassification(a),lbl);
	// }
	double t = cvThreshold(img,img,128,255,0);
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
	int b[28*28],c[28][28];
	// invert the image
	for(int i=0;i<height;i++){
	  for(int j=0;j<width;j++){
	    for(int k=0;k<channels;k++){
	      data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
	     	c[i][j] = (data[i*step+j*channels+k]) ? 1:0 ;
			}
	  }
	}
	double sum1,sum2;
	int drow[28],dcol[28];
	for(int i=0;i<height;i++){
		sum1=0.0;sum2=0.0;
	  for(int j=0;j<width;j++){
			sum1+=c[i][j];
			sum2+=c[j][i];
		}
		if(sum1==0){
			drow[i]=1;
		}else{
			drow[i]=0;
		}
		if(sum2==0){
			dcol[i]=1;
		}else{
			dcol[i]=0;
		}
	}
	int row_z=0,col_z=0;
	for(int i=0;i<28;i++){
		if(drow[i]==0)
			row_z++;
		if(dcol[i]==0)
			col_z++;
	}
	int first_0_row,last_0_row,first_0_col,last_0_col;
	for(int i=0;i<28;i++){
		if(drow[i]==0){
			first_0_row=i;
			break;}
	}
	for(int i=0;i<28;i++){
		if(dcol[i]==0){
			first_0_col=i;
			break;
		}
	}
	for(int i=27;i>0;i--){
		if(drow[i]==0){
			last_0_row=i;
			break;
		}
	}
	for(int i=27;i>0;i--){
		if(dcol[i]==0){
			last_0_col=i;
			break;
		}
	}
	int l,m,p_image[row_z][col_z];
	printf("%d %d\n",row_z,col_z);

	for(int i=first_0_row;i<=last_0_row;i++){
		for(int j=first_0_col;j<=last_0_col;j++){
			l=i-first_0_row;m=j-first_0_col;
			p_image[l][m]=c[i][j];
			printf("%d",p_image[l][m]);
		}
		printf("\n");
	}
	IplImage* crpd= cvCreateImage(cvSize( col_z, row_z), img->depth, img->nChannels );
	// get the image data
	height    = crpd->height;
	width     = crpd->width;
	step      = crpd->widthStep;
	channels  = crpd->nChannels;
	data      = (uchar *)crpd->imageData;

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			for(int k=0;k<channels;k++){
				data[i*step+j*channels+k]=(p_image[i][j])? 255 : 0;
			}
		}
	}

	IplImage* crp= cvCreateImage(cvSize( 20,20), img->depth, img->nChannels );
	cvResize(crpd, crp,CV_INTER_AREA );

	height    = crp->height;
	width     = crp->width;
	step      = crp->widthStep;
	channels  = crp->nChannels;
	data      = (uchar *)crp->imageData;
	int crp_image[20][20];
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			for(int k=0;k<channels;k++){
				crp_image[i][j]= ((int)data[i*step+j*channels+k]) ? 1 : 0;
			}
		}
	}

	Vector *image1 = (Vector*)malloc(sizeof(double)+(sizeof(double)*784));
	image1->size = 28*28;
	for(int j=0;j<image1->size;j++){
	   image1->vals[j]=b[j];
	  }
	for(int j=0;j<image1->size;j++){
		 if(j%20==0){
			if(j!=0)
		 		printf("\n");
		}
		printf("%d",(int)image1->vals[j]);
	}
	printf("\n");
	printf("%d %d %d %d",first_0_row,last_0_row,first_0_col,last_0_col);

	// feedInput(a,image1);
	// feedForwardNetwork(a);
	// printf("%d\n",getNetworkClassification(a));
	// show the image
	cvShowImage("mainWin", crp );
	// wait for a key
	cvWaitKey(0);
	// release the image
	cvReleaseImage(&crp );
	printf("\n");

	}
