// include opencv
#include <cv.h>
#include <highgui.h>


IplImage* open_image(char*  a){
  IplImage* img=cvLoadImage(a,CV_LOAD_IMAGE_GRAYSCALE);
  if(!img){
    printf("Could not load image file: %s\n",a);
    exit(0);
  }
  return img;
}

Vector* convert_to_MNIST_Image(char* path){
  IplImage* img = open_image(path);
	int height,width,step,channels;
	uchar *data;
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  printf("Processing a %dx%d image with %d channels...Done\n",height,width,channels);
  printf("Thresholding Image (increasing contrast)...Done\n");
  double t = cvThreshold(img,img,128,255,0);

  IplImage* dst = cvCreateImage(cvSize( 28, 28), img->depth, img->nChannels );
  cvResize(img, dst,CV_INTER_AREA );

  img = dst;
  // get the image data

  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  printf("Coverting image to a %dx%d...Done\n",height,width);
  int b[28*28],c[28][28];
  // invert the image
  printf("Inverting image...Done\n");
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      for(int k=0;k<channels;k++){
        data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
        c[i][j] = (data[i*step+j*channels+k]) ? 1:0 ;
      }
    }
  }
  printf("Croping digit pixels...Done\n");
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
      break;
    }
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
  // printf("%d %d\n",row_z,col_z);

  for(int i=first_0_row;i<=last_0_row;i++){
    for(int j=first_0_col;j<=last_0_col;j++){
      l=i-first_0_row;m=j-first_0_col;
      p_image[l][m]=c[i][j];
      // printf("%d",p_image[l][m]);
    }
  }
  printf("Converting to 20x20...Done\n");
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
  for(int i=0;i<28;i++){
    for(int j=0;j<28;j++){
      c[i][j]=0;
    }
  }
  for(int i=0;i<20;i++){
    for(int j=0;j<20;j++){
      c[i+4][j+4]=crp_image[i][j];
    }
  }
  for(int i=0;i<28;i++){
    for(int j=0;j<28;j++){
      b[i*28+j]=c[i][j];
    }
  }
  printf("Padding image to 28x28...Done\n");
  Vector *image1 = (Vector*)malloc(sizeof(double)+(sizeof(double)*784));
  image1->size = 28*28;
  for(int j=0;j<image1->size;j++){
     image1->vals[j]=b[j];
    }
  for(int j=0;j<image1->size;j++){
     if(j%28==0){
      if(j!=0)
        printf("\n");
    }
    printf("%d",(int)image1->vals[j]);
  }
  printf("\nVectorizing image...Done\n");

  return image1;
}
