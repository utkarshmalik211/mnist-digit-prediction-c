typedef struct Node{
	double bias;
	double output;
	int wcount;
	double weights[];
} Node;

typedef struct Layer{
	int ncount;
	Node nodes[];
} Layer;

typedef struct Network{
	int inpNodeSize;
	int inpLayerSize;
	int hidNodeSize;
	int hidLayerSize;
	int outNodeSize;
	int outLayerSize;
	Layer layers[];
} Network;

Network *createNetwork(int inpCount,int hidCount,int outCount){
	int inpNodeSize  = sizeof(Node);
	int inpLayerSize = sizeof(Layer) + (inpCount * inpNodeSize);

	int hidWeightsCount = inpCount;
	int hidNodeSize  = 	sizeof(Node) + (hidWeightsCount * sizeof(double));
	int hidLayerSize = sizeof(Layer) + (hidCount * hidNodeSize);

	int outWeightsCount = hidCount;
	int outNodeSize  = sizeof(Node) + (outWeightsCount * sizeof(double));
	int outLayerSize = sizeof(Layer) + (outNodeSize * outCount);

	Network *nn = (Network*)malloc(sizeof(Network) + inpLayerSize + hidLayerSize + outLayerSize);

	nn->inpNodeSize   = inpNodeSize;
    nn->inpLayerSize  = inpLayerSize;
    nn->hidNodeSize   = hidNodeSize;
    nn->hidLayerSize  = hidLayerSize;
    nn->outNodeSize   = outNodeSize;
    nn->outLayerSize  = outLayerSize;

	return nn;
}