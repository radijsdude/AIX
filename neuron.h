#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED


#include<vector>
#include<math.h>
#include<stdio.h>
#include <stdlib.h>



#define learningrate 0.01
#define momentumfactor 0.01

const int none = 0;
#define sigmoid 1

const int summation = 0;
#define multiplice 1

#define quadratic 0
#define crossentropy 1

struct Neuron
{
    int net;
    int layer;
    int id;
    int type;

    int active;
    int activation;
    int recursive;
    int convolution;
    int convolutional_position;

    float potential;

    float recurse_potential;
    float recurse_weight;

    std::vector<float> errors;
    std::vector<Neuron*> connections;
    std::vector<float> weights;
    std::vector<float> dtweights;

};


void N_init(Neuron* n);
bool N_compare(Neuron* n1, Neuron* n2, int verbose, int serious);
Neuron N_copy(Neuron n);
void N_print(Neuron n);

Neuron N_new(int layer, int id, int type, int activation, int recursive, int convolution);
Neuron N_blanck();
void N_connect(Neuron* n1, Neuron* n2, float weight=0.0f);
void N_get_connections(Neuron* n, std::vector<std::vector<int>>* connections);

float N_calculate(Neuron* n);
void N_correct(Neuron* n);
float activation_function(float potential, int activation, int derivative);


int N_test1();
int N_test2();
int N_test3();

#endif // NEURON_H_INCLUDED
