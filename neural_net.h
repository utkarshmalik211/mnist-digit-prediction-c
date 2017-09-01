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

void initNetwork(Network *nn,int inpCount,int hidCount,int outCount){
	uint8_t *sbptr = (uint8_t*) nn->layers; //single byte pointer for fast access

	Layer *il = createInputLayer(inpCount);
	memcpy(nn->layers,il,nn->inpLayerSize);
	free(il);

	subptr += nn->inpLayerSize; // move pointer to begining of hidden layer

	Layer *hl = createLayer(hidCount, inpCount);
	memcpy(subptr,hl,nn->hidLayerSize);
	free(hl);
	subptr += nn->hidLayerSize;

	Layer *ol = createLayer(outCount,hidCount);
	memcpy(subptr,ol,nn->outLayerSize);
	free(ol);

}

Layer *createInputLayer(int inpCount){
	int inpNodeSize = sizeof(Node);
	int inpLayerSize= sizeof(Layer) + (inpCount * inpNodeSize);

	Layer *il = malloc(inpLayerSize);
	il->ncount = inpCount;

	Node iln;
	iln.bias = 0;
	iln.output = 0;
	iln.wcount = 0;

	uint8_t *sbptr = (uint8_t*) il->nodes;

	for (int i = 0; i < il->ncount; i++){
		memcpy(sbptr,&iln,inpNodeSize);
		sbptr += inpNodeSize;
	}
	return il;
}

Layer *createLayer(int nodeCount,int weightCount){
	int nodeSize = sizeof(Node) + (weightCount * sizeof(double));
	Layer *l = (Layer*)malloc(sizeof(Layer)+(nodeCount*nodeSize));
}