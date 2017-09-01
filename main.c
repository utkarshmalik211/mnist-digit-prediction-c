#include <stdio.h>
#include <stdlib.h>
#include "neural_net.h"

void main(){
	Network *a = createNetwork(748,20,10);
	printf("%d",a->inpNodeSize);
}