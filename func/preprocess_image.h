// include opencv well ignore these 
#include <cv.h>
#include <highgui.h>
// function to convert networks classification into char to print it on photos
char getint(int a){
        char c;
        switch(a) {
        case 0:
                c='0';
                break;

        case 1:
                c='1';
                break;
        case 2:
                c='2';
                break;
        case 3:
                c='3';
                break;
        case 4:
                c='4';
                break;
        case 5:
                c='5';
                break;
        case 6:
                c='6';
                break;
        case 7:
                c='7';
                break;
        case 8:
                c='8';
                break;
        case 9:
                c='9';
                break;
        }
        return c;

}

//@function
void convert_to_MNIST_Image(Network* nn,char* path){
        int height,width,step,channels;
        uchar *data;
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1,1,1,8);
        IplImage* cc_img = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
        IplImage* src = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
        IplImage* cc_img_th_in = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);;

        CvMemStorage *mem;
        mem = cvCreateMemStorage(0);
        CvSeq *contours = 0;

        // edges returned by Canny might have small gaps between them, which causes some problems during contour detection
        // Simplest way to solve this s to "dilate" the image.
        double t = cvThreshold(src,cc_img,threshold,255,0);
        // well tried adaptive threshold but it just does'nt work so stickingto normal threshold
        // cvAdaptiveThreshold(src,cc_img, 170, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,3, 5 );
        height    = cc_img->height;
        width     = cc_img->width;
        step      = cc_img->widthStep;
        channels  = cc_img->nChannels;
        data      = (uchar *)cc_img->imageData;
        int b[28*28],c[28][28];
        // invert the image
        for(int i=0; i<height; i++) {
                for(int j=0; j<width; j++) {
                        for(int k=0; k<channels; k++) {
                                data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
                        }
                }
        }
        cvCopy(cc_img, cc_img_th_in, NULL);
        cvDilate(cc_img_th_in, cc_img_th_in,cvCreateStructuringElementEx(3,3,0,0,CV_SHAPE_RECT ,NULL),7);
        int n = cvFindContours( cc_img_th_in, mem, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
        int i=0;
        CvSeq* ptr = 0;
        CvRect rects[10];
        for (ptr = contours; ptr != NULL; ptr = ptr->h_next) {
                rects[i] = cvBoundingRect(ptr,0);
                double height = rects[i].height;
                double width = rects[i].width;
                if (rects[i].width > rects[i].height){
                    rects[i].height = rects[i].width;
                    rects[i].y-=(width-height)/2;
                    rects[i].x-=4;
                }
                else{
                  rects[i].width = rects[i].height;
                  rects[i].x-=(height-width)/2;
                  rects[i].y+=4;
                }
                CvPoint tl = cvPoint(rects[i].x,rects[i].y);
                CvPoint br = cvPoint(rects[i].x + rects[i].width - 1,rects[i].y + rects[i].height - 1);

                cvSetImageROI(cc_img_th_in, rects[i]);

                //cvSaveImage("out.png", cc_img);
                cvRectangle(src,tl,br, CV_RGB( 0,0,0 ), 1, CV_AA, 0 );
                IplImage* temp = cvCreateImage(cvSize( 20, 20), cc_img_th_in->depth, cc_img_th_in->nChannels );
                cvResize(cc_img_th_in,temp,CV_INTER_AREA);
                height    = temp->height;
                width     = temp->width;
                step      = temp->widthStep;
                channels  = temp->nChannels;
                data      = (uchar *)temp->imageData;
                int crp_image[20][20];
                for(int i=0; i<height; i++) {
                        for(int j=0; j<width; j++) {
                                for(int k=0; k<channels; k++) {
                                        crp_image[i][j]= ((int)data[i*step+j*channels+k]) ? 1 : 0;
                                }
                        }
                }
                for(int i=0; i<28; i++) {
                        for(int j=0; j<28; j++) {
                                c[i][j]=0;
                        }
                }
                for(int i=0; i<20; i++) {
                        for(int j=0; j<20; j++) {
                                c[i+4][j+4]=crp_image[i][j];
                        }
                }
                for(int i=0; i<28; i++) {
                        for(int j=0; j<28; j++) {
                                b[i*28+j]=c[i][j];
                        }
                }
                Vector *image1 = (Vector*)malloc(sizeof(double)+(sizeof(double)*784));
                image1->size = 28*28;
                for(int j=0; j<image1->size; j++) {
                        image1->vals[j]=b[j];
                }
                for(int j=0; j<image1->size; j++) {
                        if(j%28==0) {
                                if(j!=0)
                                        printf("\n");
                        }
                        if((int)image1->vals[j]==1) {
                                printf("%d",(int)image1->vals[j]);
                        }
                        else{
                                printf(" ");
                        }
                }
                printf("\n");
                feedInput(nn,image1);
                feedForwardNetwork(nn);
                int a = getNetworkClassification(nn);
                char c[2];
                c[0]=getint(a);
                c[1]='\0';
                printf("Predicted output for above image is : %s\n",c);
                printNetworkClassification(nn);
                cvPutText(src,&c[0], cvPoint(rects[i].x,rects[i].y-4 /*adjustments*/), &font,cvScalar(0,0,0,0));
                i++;
        }
        cvShowImage("Tutorial", src);
        cvWaitKey(0);
}
