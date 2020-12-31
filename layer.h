#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED


#include"neuron.h"

struct Layer
{
    std::vector<Neuron> neurons;
    int net;
    int id;
    int type;
    int activation;
    int recursive;
    int convolution;
    int biased;
};

void L_init(Layer* l);
Layer L_copy(Layer l);
void L_print(Layer l);

Layer L_new(int id, int s, int type, int activation, int recursive, int convolution, int biased);
bool L_compare(Layer* l1, Layer* l2, int verbose, int serious);
Layer L_blanck();


void L_set_potentials(Layer* l, std::vector<float> potentials);
void L_set_errors(Layer* l, std::vector<float> errors);

void L_calculate(Layer* l);
void L_correct(Layer* l);

void L_connect_fully(Layer* l1, Layer* l2);
void L_connect_single(Layer* l1, Layer* l2);
void L_connect_convolution(Layer* linput, Layer* lconv, Layer* loutput);
void L_connect_convolution_T(Layer* linput, Layer* lconv, Layer* loutput);
void L_connect_convolution_strafe(Layer* linput, Layer* lconv, Layer* loutput);

void L_add(Layer* l1, Layer* l2, Layer* l3);
int L_max(Layer* layer);
std::vector<float> L_loss(Layer*l, std::vector<float> solutions, int loss_function, int total);



void Layers_calculate(std::vector<Layer*> layers);
void Layers_correct(std::vector<Layer*> layers);
void Layers_learn(std::vector<Layer*> layers, std::vector<float> inputs, std::vector<float> solutions);



#endif // LAYER_H_INCLUDED
