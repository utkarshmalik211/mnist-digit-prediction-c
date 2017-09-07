IplImage* img = 0;
int height,width,step,channels;
uchar *data;
void openImage(){
  if(argc<2){
    printf("Usage: main <image-file-name>\n\7");
    exit(0);
  }
  img=cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!img){
    printf("Could not load image file: %s\n",argv[1]);
    exit(0);
  }
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
double b[28*28];
// invert the image
for(int i=0;i<height;i++){
  for(int j=0;j<width;j++){
    for(int k=0;k<channels;k++)
      data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
      b[i*28 + j] = data[i*step+j*channels+k]/255.0;
  }
}
Vector *image1 = (Vector*)malloc(sizeof(double)+(sizeof(double)*784));
image1->size = 28*28;
for(int j=0;j<image1->size;j++){
   image1->vals[j]=b[j];
  }

// show the image
cvShowImage("mainWin", img );
// wait for a key
cvWaitKey(0);
// release the image
cvReleaseImage(&img );
printf("\n");
