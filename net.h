#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include "layer.h"
#include <fstream>

#define FULLY 0
#define SINGLE 1
#define CONV 0
#define CONVT 1
#define STRAFE 2

#define BASIC 0
#define BASIC32 1
#define EXP 2

#define normal 0
#define hilbert 1

struct Net{
    int id;
    int inputtype;
    int root;
    std::vector<Layer> layers;
};

bool Net_compare(Net* net1, Net* net2, int verbose, int serious);
void Net_Name_layers(Net* net);
void Net_Append_Layer(Net* net, Layer layer);
void Net_Add_Layers(Net* net, int layer1, int layer2);
void Net_Connect_Layer(Net* net, int layer1, int layer2, int connection);
void Net_Connectconv_Layer(Net* net, int layer1, int connection, int convsize);


void Net_Calculate(Net* net, int layer0);
void Net_Correct(Net* net, int layer0);
void Net_Train(Net* net, std::vector<float> inputs, std::vector<float> solutions, int layer0);


void Net_Create(Net* net,int id, int type, int inputsize, int outputsize, int inputtype);
void Net_Input(Net* net, std::vector<float> potentials);

void Net_Save(Net* net, char* path);
void Net_Open(Net* net, char* path);

#endif // NET_H_INCLUDED
