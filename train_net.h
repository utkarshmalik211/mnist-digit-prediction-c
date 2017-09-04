#include<math.h>

void feedInput(Network *nn,Vector *v){
	Layer *il = nn->layers;

	Node *iln = il->nodes;

	for (int i = 0; i < v->size; i++){
		iln->output = v->vals[i];
		iln++;
	}
}

/////////////feed forward///////////////
void clacNodeOutput(Network *nn,LayerType ltype,int id){
	Layer *calcLayer = getLayer(nn,ltype);
	Node *calcNode = getNode(calcLayer,id);

	Layer *prevLayer;
	int prevLayerNodeSize = 0;

	if(ltype==HIDDEN){
		prevLayer = getLayer(nn,INPUT);
		prevLayerNodeSize = nn->inpNodeSize;
	}
	else{
		prevLayer = getLayer(nn,HIDDEN);
		prevLayerNodeSize = nn->hidNodeSize;
	}

	uint8_t *sbptr = (uint8_t*) prevLayer->nodes;
	// start by adding bias

	calcNode->output = calcNode->bias;
	// bias + input* weight
	for(int i =0;i<prevLayer->ncount;i++){
		Node *prevLayerNode = (Node*)sbptr;
		calcNode->output += prevLayerNode->output * calcNode->weights[i];
		sbptr += prevLayerNodeSize;

	}

}

void activateNode(Network *nn,LayerType ltype,int id){
	Layer *l = getLayer(nn, ltype);
	Node *n = getNode(l, id);

	ActFctType actFct;

	if(ltype==HIDDEN){
		actFct = nn->hidLayerActType;
	}else{
		actFct = nn->outLayerActType;
	}

	if(actFct==TANH){
		n->output = tanh(n->output);
	}else{
		n->output = 1/(1+(exp((double) -(n->output))));
	}
}

void calcLayer(Network *nn,LayerType ltype){
	Layer *l;
	l = getLayer(nn,ltype);

	for(int i = 0;i<l->ncount;i++){
		clacNodeOutput(nn,ltype,i);
		activateNode(nn,ltype,i);
	} 
}

void feedForwardNetwork(Network *nn){
	calcLayer(nn, HIDDEN);
	calcLayer(nn, OUTPUT);
}



///////////Back propagate///////////

double getActFctDerivative(Network *nn,LayerType ltype ,double outVal){

	double dVal = 0 ;
	ActFctType actFct;

	if (ltype==HIDDEN) actFct = nn->hidLayerActType;
                  else actFct = nn->outLayerActType;
    
    if (actFct==TANH) dVal = 1-pow(tanh(outVal),2);
                 else dVal = outVal * (1-outVal);
    
    return dVal;

} 

void updateNodeWeights(Network *nn, LayerType ltype, int id, double error){
    
    Layer *updateLayer = getLayer(nn, ltype);
    Node *updateNode = getNode(updateLayer, id);
    
    Layer *prevLayer;
    int prevLayerNodeSize = 0;
    if (ltype==HIDDEN) {
        prevLayer = getLayer(nn, INPUT);
        prevLayerNodeSize = nn->inpNodeSize;
    } else {
        prevLayer = getLayer(nn, HIDDEN);
        prevLayerNodeSize = nn->hidNodeSize;
    }
    
    uint8_t *sbptr = (uint8_t*) prevLayer->nodes;
    
    for (int i=0; i<updateNode->wcount; i++){
        Node *prevLayerNode = (Node*)sbptr;
        updateNode->weights[i] += (nn->learningRate * prevLayerNode->output * error);
        sbptr += prevLayerNodeSize;
    }
    
    // update bias weight
    updateNode->bias += (nn->learningRate * 1 * error);
    
}

void backPropagateOutputLayer(Network *nn, int targetClassification){
    
    Layer *ol = getLayer(nn, OUTPUT);
    
    for (int o=0;o<ol->ncount;o++){
        
        Node *on = getNode(ol,o);
        
        int targetOutput = (o==targetClassification)?1:0;
        
        double errorDelta = targetOutput - on -> output;
        double errorSignal = errorDelta * getActFctDerivative(nn, OUTPUT, on->output);
        
        updateNodeWeights(nn, OUTPUT, o, errorSignal);
        
    } 
}

void backPropagateHiddenLayer(Network *nn, int targetClassification){
    
    Layer *ol = getLayer(nn, OUTPUT);
    Layer *hl = getLayer(nn, HIDDEN);
    
    for (int h=0;h<hl->ncount;h++){
        Node *hn = getNode(hl,h);
        
        double outputcellerrorsum = 0;
        
        for (int o=0;o<ol->ncount;o++){
            
            Node *on = getNode(ol,o);
            
            int targetOutput = (o==targetClassification)?1:0;
            
            double errorDelta = targetOutput - on->output;
            double errorSignal = errorDelta * getActFctDerivative(nn, OUTPUT, on->output);
            
            outputcellerrorsum += errorSignal * on->weights[h];
        }
        
        double hiddenErrorSignal = outputcellerrorsum * getActFctDerivative(nn, HIDDEN, hn->output);
        
        updateNodeWeights(nn, HIDDEN, h, hiddenErrorSignal);
    }
    
}

void backPropagateNetwork(Network *nn, int targetClassification){
    
    backPropagateOutputLayer(nn, targetClassification);
    
    backPropagateHiddenLayer(nn, targetClassification); 
}

int getNetworkClassification(Network *nn){
    
    Layer *l = getLayer(nn, OUTPUT);
    
    double maxOut = 0;
    int maxInd = 0;
    
    for (int i=0; i<l->ncount; i++){
        
        Node *on = getNode(l,i);
        
        if (on->output > maxOut){
            maxOut = on->output;
            maxInd = i;
        }
    }
    
    return maxInd;
}