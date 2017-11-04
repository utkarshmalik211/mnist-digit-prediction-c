// include opencv
#include <cv.h>
#include <highgui.h>


Vector* convert_to_MNIST_Image(char* path){
        int height,width,step,channels;
        uchar *data;
        int b[28*28],c[28][28];
        IplImage* cc_img = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
        IplImage* src = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);
        IplImage* cc_img_th_in = cvLoadImage(path, CV_LOAD_IMAGE_GRAYSCALE);;

        CvMemStorage *mem;
        mem = cvCreateMemStorage(0);
        CvSeq *contours = 0;
        cvDilate(cc_img, cc_img,NULL,1 );
        // edges returned by Canny might have small gaps between them, which causes some problems during contour detection
        // Simplest way to solve this s to "dilate" the image.
        double t = cvThreshold(src,cc_img,90,255,0);

        height    = cc_img->height;
        width     = cc_img->width;
        step      = cc_img->widthStep;
        channels  = cc_img->nChannels;
        data      = (uchar *)cc_img->imageData;
        printf("Coverting image to a %dx%d...Done\n",height,width);
        int b[28*28],c[28][28];
        // invert the image
        printf("Inverting image...Done\n");
        for(int i=0; i<height; i++) {
                for(int j=0; j<width; j++) {
                        for(int k=0; k<channels; k++) {
                                data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
                        }
                }
        }
        cvCopy(cc_img, cc_img_th_in, NULL);
        int n = cvFindContours( cc_img, mem, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
        int i=0;
        CvSeq* ptr = 0;
        CvRect rects[10];
        for (ptr = contours; ptr != NULL; ptr = ptr->h_next) {
                rects[i] = cvBoundingRect(ptr,0);
                CvPoint tl = cvPoint(rects[i].x,rects[i].y);
                CvPoint br = cvPoint(rects[i].x + rects[i].width - 1,rects[i].y + rects[i].height - 1);

                cvSetImageROI(cc_img_th_in, rects[i]);
                // cvShowImage("Tutorial", cc_img_th_in);
                //cvSaveImage("out.png", cc_img);
                // cvRectangle(src,tl,br, CV_RGB( 255, 255, 255 ), 1, CV_AA, 0 );
                // cvWaitKey(0);



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

                i++;
        }
        printf("Padding image to 28x28...Done\n");
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
        printf("\nVectorizing image...Done\n");

        return image1;
}
