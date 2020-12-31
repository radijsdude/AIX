#ifndef REGION_H_INCLUDED
#define REGION_H_INCLUDED

#define NETINPUT 0
#define NETHIDDEN 1
#define NETOUTPUT 2

#include "net.h"
#include <thread>

struct Region
{
    int id;
    int root;
    int training;
    std::vector<Net> nets;
    std::vector<int> types;
    std::vector<int> inputnets;
    std::vector<int> outputsnets;
    std::vector<int> subnets;
    std::vector<std::vector<int>> threadables;
    std::vector<int> calculationorder;
    std::vector<std::vector<int>> connections;
};

void Region_Name(Region* region);
void Region_get_neurons(Region* region,std::vector<std::vector<int>>* neurons);
void Region_get_connections(Region* region,std::vector<std::vector<int>>* connections);

void Region_Open_Net(Region* region, char* path, int type, int threadable, int subnet, int calculationstep);
void Region_Open(Region* region, char* path);
void Region_Save(Region* region, char* path);

void Region_Train_Subnet(Net* net, std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> solutions);

void Region_Train_Subnets(Region* region, std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> solutions);


void Region_Set_Inputs(Region* region, std::vector<float> inputs);

void Region_Calculate(Region* region);

#endif // REGION_H_INCLUDED
