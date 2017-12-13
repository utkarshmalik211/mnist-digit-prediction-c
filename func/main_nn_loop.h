// function to display training progress on Terminal
void displayProgress(float iter,float total,int epoch,int state){
								char c[21];
								float point = ((iter/(total-1))*100)/5;
								for(int i = 0; i < point; i++) {
																c[i]='=';
								}
								c[(int)point]='>';
								for(int i = point+1; i < 20; i++) {
																c[i]=' ';
								}
								c[20]='\0';
								if(state == 0)
																printf("\rTraining Network epoch(%d)|%s|",epoch,c);
								else
																printf("\rTesting Network epoch(%d) |%s|",epoch,c);
}

// function to convert image from mnist dataset to a vector of 0's and 1's
Vector *getVectorFromImage(MNIST_Image *img){
								Vector *v = (Vector*)malloc(sizeof(Vector)+(MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT*sizeof(double)));
								v->size = MNIST_IMG_WIDTH*MNIST_IMG_HEIGHT;
								for(int i = 0; i <v->size; i++) {
																v->vals[i] = img->pixel[i] ? 1 : 0;
								}
								return v;
}
// main function to train neural network on training dataset 
// input args : 
// Network *nn ie pointer to the network on which backpropagation will occur
// int epoch ie no of time function is training network (mainly used for displaying) 
void trainNet(Network *nn,int epoch){
								FILE *trainImageFile = openMNISTImageFile(MNIST_TRAINING_SET_IMAGE_FILE_NAME);
								FILE *trainLableFile = openMNISTLabelFile(MNIST_TRAINING_SET_LABEL_FILE_NAME);
								int count = 0;
								for (int i = 0; i < MNIST_MAX_TRAINING_IMAGES; i++) {
																MNIST_Image img = getImage(trainImageFile);
																MNIST_Label lbl = getLabel(trainLableFile);
																Vector *image = getVectorFromImage(&img);
																feedInput(nn,image);
																feedForwardNetwork(nn);
																backPropagateNetwork(nn,(int) lbl);
																displayProgress(i,MNIST_MAX_TRAINING_IMAGES,epoch,0);
								}
								fclose(trainLableFile);
								fclose(trainImageFile);
}
// main function to testing neural network on training dataset 
// input args : 
// Network *nn ie pointer to the network on which backpropagation will occur
// int epoch ie no of time function is testing network (mainly used for displaying)
// as above function here backpropagation doesn't occur as testing data set is not used to backpropagate the network

void testNet(Network *nn,int epoch){
								FILE *trainImageFile = openMNISTImageFile(MNIST_TESTING_SET_IMAGE_FILE_NAME);
								FILE *trainLableFile = openMNISTLabelFile(MNIST_TESTING_SET_LABEL_FILE_NAME);
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
																displayProgress(i,MNIST_MAX_TESTING_IMAGES,epoch,1);
								}
								printf("\nTrain accuracy for %d epoch = %d Percent\n",epoch,(count/100));
								fclose(trainLableFile);
								fclose(trainImageFile);
}
