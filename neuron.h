#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"
#include "string.h"

#define MAX_INPUTS = 1024

typedef struct {
    int *inputs;
    int num_inputs;
    int *weights;
    int output;
    void (*init)(struct Neuron *neuron, int num_inputs);
    void (*propagate)(struct Neuron *neuron, char* activation_function);
} Neuron;

typedef struct {
	int *inputs;
	int num_inputs;
	int *weights;
	int *output;
	void (*init)(struct Neuron *neuron, int num_inputs);
    void (*softmax_propagate)(struct Neuron *neuron);
} Output_Neuron;

typedef struct {
   int num_neurons;
   Neuron *neurons;
} Layer;

void init_neuron(Neuron *neuron, int num_inputs){
    neuron->num_inputs = num_inputs;
    neuron->inputs = malloc(sizeof(int) * num_inputs);
    neuron->weights = malloc(sizeof(int) * num_inputs);
    for(int i=0;i<num_inputs;i++){
        neuron->inputs[i]= rand()/RAND_MAX ;
    }
}

void init_output_neuron(Output_Neuron *neuron, int num_inputs){
    neuron->num_inputs = num_inputs;
    neuron->inputs = malloc(sizeof(int) * num_inputs);
    neuron->weights = malloc(sizeof(int) * num_inputs);
    for(int i=0;i<num_inputs;i++){
        neuron->inputs[i]= rand()/RAND_MAX ;
    }
}


void propagate(Neuron *neuron, char *activation_function){
    neuron->output = 0;
    for(int i = 0 ; i<neuron->num_inputs ; i++){
        neuron->output += neuron->weights[i]*neuron->inputs[i];
    }
    if(strcmp(activation_function, "relu")==0){
        if (neuron->output < 0){ neuron->output = 0;}
        else { neuron->output = (int)(neuron->output); }
    }
}

void softmax_propagate(Output_Neuron* neuron) {
    neuron->output = 0;

    // Calculate the sum of the exponential outputs
    double sum_exp_outputs = 0.0;
    for (int i = 0; i < neuron->num_inputs; i++) {
        sum_exp_outputs += exp(neuron->weights[i] * neuron->inputs[i]);
    }

    // Calculate the softmax values
    for (int i = 0; i < neuron->num_inputs; i++) {
        double exp_output = exp(neuron->weights[i] * neuron->inputs[i]);
        neuron->output[i] = exp_output / sum_exp_outputs;
    }
}


Layer fully_connected(int num_neurons, int num_inputs){
    Layer layer;
    layer.num_neurons = num_neurons;
    layer.neurons = malloc(sizeof(Neuron) * num_neurons);

    for (int i = 0; i < num_neurons; i++){
        init_neuron(&(layer.neurons[i]), num_inputs);
    }

   return layer;
}

Layer output_layer(int num_inputs, int num_outputs){
	Layer layer;
	layer.num_neurons = num_outputs;
	layer.neurons = malloc(sizeof(Output_Neuron) * num_outputs);

    for (int i = 0; i < num_outputs; i++){
        init_output_neuron(&(layer.neurons[i]), num_inputs);
    }

	return layer;
}

typedef struct{
    int num_layers;
    Layer *layers;
} Net;

Net create_net(int num_inputs, int num_outputs){
    Net net;
    net.num_layers = num_outputs;
    net.layers = malloc(sizeof(Layer) * num_outputs);
    // CREATE EACH LAYER IN THE NET
    for (int i = 0; i < num_outputs; i++){
        if ( i == 0 ){
            //INPUT
			net.layers[i] = fully_connected(num_outputs, num_inputs);
        }
        else{
            //HIDDEN
            net.layers[i] = fully_connected(num_outputs,num_outputs);
        }
    }
    return net;
}


