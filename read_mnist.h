typedef uint8_t MNIST_Label;

typedef struct MNIST_Image{
	uint8_t pixel[28*28];
} MNIST_Image;

typedef struct Vector{
	int size;
	double vals[];
} Vector;

// convert mnist image structure to Vector structure
Vector *getVectorFromImage(MNIST_Image *img){
	
	Vector *v = (Vector*)malloc(sizeof(Vector)+(28*28*sizeof(double)));
	v->size = 28*28;

	for(int i=0;i<v->size;i++)
		v->vals[i] = img->pixel[i] ? 1 :0;
	return v;
}