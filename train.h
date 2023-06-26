#include "neuron.h"

void gradient_descent(Net *net, double learning_rate, int num_epochs, double **training_data, double **labels, int num_samples){

    for (int epoch = 0; epoch < num_epochs; epoch++){
        
        for (int sample=0; sample < num_samples; sample++){

            //FORWARD PROPAGATION
            double *input = training_data[sample];
            Layer *input_layer = &(net->layers[0]);
            
                // set the input values of the input layer neurons
            
            for (int i=0; i < input_layer->num_neurons; i++){
                Neuron *neuron = &(input_layer->neurons[i]);
                for (int j=0; j<neuron->num_inputs; j++){
                    neuron->inputs[j] = input[j];
                }
            }

                //propagate through the layers
            for(int layer_idx = 0; layer_idx<net->num_layers; layer_idx++){
                Layer *current_layer = &(net->layers[layer_idx]);
                Layer *next_layer = &(net->layers[layer_idx+1]);

                    //propagte thru each neuron in the current layer

                for(int neuron_idx = 0; neuron_idx < current_layer->num_neurons; neuron_idx++){
                    Neuron *current_neuron=&(current_layer->neurons[neuron_idx]);

                        //calculate output
                    current_neuron->propagate(current_neuron, "relu");

                        //pass the output to the next layer
                    if(next_layer != NULL){
                        for(int i=0; i<next_layer->num_neurons; i++){
                            next_layer->neurons[i].inputs[neuron_idx]=current_neuron->output;

                        }
                    }
                    
                }

            } 
            
                // Special handling for the output layer with softmax
            Layer *output_layer = &(net->layers[net->num_layers -1]);
            
            for(int neuron_idx = 0; neuron_idx < output_layer->num_neurons; neuron_idx++){
                Output_Neuron *output_neuron = (Output_Neuron*)&(output_layer->neurons[neuron_idx]);
                output_neuron->softmax_propagate(output_neuron);
            }

            // COMPUTE LOSS
            output_layer = &(net->layers[net->num_layers -1]);
            
            int num_classes = output_layer->num_neurons;

            double loss = 0.0;

            for(int neuron_idx = 0; neuron_idx < num_classes; neuron_idx++){
                Output_Neuron *output_neuron = (Output_Neuron*)&(output_layer->neurons[neuron_idx]);
                double *predicted_output = output_neuron->output;
                double *true_label = labels[sample];

                for (int class_idx = 0; class_idx < num_classes; class_idx++){
                    loss += true_label[class_idx] * log(predicted_output[class_idx]);
                }
            }

            loss *= -1.0;

            //BACK PROPAGATION
            
            for (int layer_idx = net->num_layers -1; layer_idx >= 0; layer_idx--){
                Layer *current_layer = &(net->layers[layer_idx]);
                Layer *prev_layer = (layer_idx >0) ? &(net->layers[layer_idx -1]) : NULL;

                // Iterate neurons in current_layer

                for (int neuron_idx = 0; neuron_idx < current_layer->num_neurons; neuron_idx++){

                    if (layer_idx == net->num_layers -1){ //Output Layer
                        Output_Neuron *current_neuron = (Output_Neuron*)&(current_layer->neurons[neuron_idx]);
                        //compute the gradients

                        double predicted_output = current_neuron->output;
                        double true_label = labels[sample];
                        
                        double gradient = predicted_value - true_value;
                        // directly update the  
                    }

                }
            }


            //UPDATE WEIGHTS
            //

            for (int layer_idx = net->num_layers -1; layer_idx >0; layer_idx--){
                Layer *current_layer = &(net->layers[layer_idx]);
                Layer *prev_layer = &(net->layers[layer_idx -1]);

                for(int neuron_idx = 0; neuron_idx < current_layer->num_neurons; neuron_idx++){
                    Neuron *current_neuron = &(current_layer->neurons[neuron_idx]);

                    //update
                    for (int weight_idx = 0; weight_idx < current_neuron->num_inputs; weight_idx++){
                        //calculate weigth w LR and grad
                        //
                        double weight_update = learning_rate * current_neuron->gradients[weight_idx];
                        current_neuron->gradients[weight_idx] -= weight_update;
                    }
                }
            }
        }

    }

}

double calculate_cross_entropy_loss(double *predicted_output, double *true_label, int num_classes){
    double loss = 0.0;
    for (int i = 0; i<num_classes; i++){
        loss += true_label[i] * log(predicted_output[i]);
    }
    loss *= -1.0;
    return loss;
}
