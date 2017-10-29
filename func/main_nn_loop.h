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
								Vector *v = (Vector*)malloc(sizeof(Vector)+(MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT*sizeof(double)));
								v->size = MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT;
								for(int i = 0; i <v->size; i++) {
																v->vals[i] = img->pixel[i] ? 1 : 0;
								}
								return v;
}

void trainNet(Network *nn){
								FILE *trainImageFile = openMNISTImageFile(MNIST_TRAINING_SET_IMAGE_FILE_NAME);
								FILE *trainLableFile = openMNISTLabelFile(MNIST_TRAINING_SET_LABEL_FILE_NAME);
								printf("Training Network : \n");
								int count = 0;
								for (int i = 0; i < MNIST_MAX_TRAINING_IMAGES; i++) {
																MNIST_Image img = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img);
																feedInput(nn,image);
																feedForwardNetwork(nn);
																backPropagateNetwork(nn,(int) lbl);
																displayProgress(i,MNIST_MAX_TRAINING_IMAGES);
								}
								printf("\n");
								fclose(trainLableFile);
								fclose(trainImageFile);
}
void testNet(Network *nn){
								FILE *trainImageFile = openMNISTImageFile(MNIST_TESTING_SET_IMAGE_FILE_NAME);
								FILE *trainLableFile = openMNISTLabelFile(MNIST_TESTING_SET_LABEL_FILE_NAME);
								printf("Testing Network :\n");
								int count = MNIST_MAX_TESTING_IMAGES;
								for (int i = 0; i < MNIST_MAX_TESTING_IMAGES; i++) {
																MNIST_Image img = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img);
																feedInput(nn,image);
																feedForwardNetwork(nn);
																if(lbl!=getNetworkClassification(nn)) {
																								count--;
																}
																backPropagateNetwork(nn,(int) lbl);
																displayProgress(i,MNIST_MAX_TESTING_IMAGES);
								}
								printf("\nTrain set accuracy achieved = %d percent	\n",(count/100));
								fclose(trainLableFile);
								fclose(trainImageFile);
}
