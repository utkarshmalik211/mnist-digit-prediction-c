#include "neural_net.h"
#include "read_mnist.h"

void main(){
	Network *a = createNetwork(748,20,10);
	initNetwork(a,748,20,10);
	initWeights(a,HIDDEN);
	initWeights(a,OUTPUT);
	printf("%d\n",a->inpNodeSize);
}