#include "region.h"


void Region_Open_Net(Region* region, char* path, int type, int threadable, int subnet, int calculationstep)
{
    Net n;
    region->nets.push_back(n);
    Net_Open(&region->nets.back(), path);
    //region->threadables.push_back(threadable);
    if(type==NETINPUT)
    {
        region->inputnets.push_back(region->nets.size()-1);
    }
    if(type==NETOUTPUT)
    {
        region->outputsnets.push_back(region->nets.size()-1);
    }
    if(subnet==1)
    {
        region->subnets.push_back(region->nets.size()-1);
    }
    if(calculationstep==0)
    {
        region->calculationorder.insert(region->calculationorder.begin(),region->nets.size()-1);
    }
    if(calculationstep==-1)
    {
        region->calculationorder.push_back(region->nets.size()-1);
    }
    if(calculationstep!=-1 && calculationstep!=0)
    {
        region->calculationorder.insert(region->calculationorder.begin() + calculationstep,region->nets.size()-1);
    }
    if(threadable!=-1)
    {
        if(region->threadables.size()==0)
        {
            std::vector<int> s;
            s.push_back(region->nets.size()-1);
            region->threadables.push_back(s);
        }
        else
        {
            int test=0;
            for(int i=0; i< region->threadables.size();i++)
            {
                for(int j=0; j<region->threadables[i].size();j++)
                {
                    if(threadable==region->threadables[i][j])
                    {
                        region->threadables[i].push_back(region->nets.size()-1);
                        test = 1;
                    }
                }
            }
            if(test==0)
            {
                std::vector<int> s;
                s.push_back(region->nets.size()-1);
                region->threadables.push_back(s);
            }
        }
    }
    else
    {
        std::vector<int> s;
        s.push_back(region->nets.size()-1);
        region->threadables.push_back(s);
    }
}

void Region_Connect_Net(Region* region, int net1, int layer1, int net2, int layer2, int proper)
{
    std::vector<int> temp;
    temp.push_back(net1);
    temp.push_back(layer1);
    temp.push_back(net2);
    temp.push_back(layer2);
    temp.push_back(proper);
    if(proper==1)
    {
        L_connect_fully(&region->nets[net1].layers[layer1],&region->nets[net2].layers[layer2]);
    }
}

void Region_Update_Net_weights(Region* region, int netid, Net* net)
{
    for(int i; i<region->nets.size();i++)
    {
        if(region->nets[i].id==netid)
        {
            for(int j=0; j<net->layers.size(); j++)
            {
                for(int k=0; k<net->layers[j].neurons.size(); k++)
                {
                    for(int l=0; l<net->layers[j].neurons[k].weights.size(); l++)
                    {
                        region->nets[i].layers[j].neurons[k].weights[l] = net->layers[j].neurons[k].weights[l];
                    }
                }
            }
        }
    }

}

void Region_Name(Region* region)
{
    for(int i=0;i<region->nets.size(); i++)
    {
        region->nets.at(i).id = i;
        Net_Name_layers(&region->nets.at(i));
    }
}

void Region_get_neurons(Region* region,std::vector<std::vector<int>>* neurons)
{
    neurons->clear();
    std::vector<int> temp;
    for(int i=0; i<region->nets.size(); i++)
    {
        for(int j=0; j<region->nets[i].layers.size(); j++)
        {
            for(int k=0; k<region->nets[i].layers[j].neurons.size(); k++)
            {
                temp.clear();
                temp.push_back(region->nets[i].layers[j].neurons[k].net);
                temp.push_back(region->nets[i].layers[j].neurons[k].layer);
                temp.push_back(region->nets[i].layers[j].neurons[k].id);
                neurons->push_back(temp);
            }
        }
    }
}
void Region_get_connections(Region* region,std::vector<std::vector<int>>* connections)
{
    connections->clear();
    std::vector<int> temp;
    for(int i=0; i<region->nets.size(); i++)
    {
        for(int j=0; j<region->nets[i].layers.size(); j++)
        {
            for(int k=0; k<region->nets[i].layers[j].neurons.size(); k++)
            {
                for(int l=0; l<region->nets[i].layers[j].neurons[k].connections.size(); l++)
                {
                    temp.clear();
                    temp.push_back(region->nets[i].layers[j].neurons[k].net);
                    temp.push_back(region->nets[i].layers[j].neurons[k].layer);
                    temp.push_back(region->nets[i].layers[j].neurons[k].id);

                    temp.push_back(region->nets[i].layers[j].neurons[k].connections[l]->net);
                    temp.push_back(region->nets[i].layers[j].neurons[k].connections[l]->layer);
                    temp.push_back(region->nets[i].layers[j].neurons[k].connections[l]->id);

                    connections->push_back(temp);
                }
            }
        }
    }
}

void Region_Save(Region* region, char* path)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        Region_Name(region);
        int amount_nets = region->nets.size();
        int amount_layers = 0;
        int amount_neurons = 0;
        int amount_connections = 0;
        int amount_weights = 0;
        int amount_errors = 0;
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                amount_layers += 1;
                for(int k=0; k<region->nets.at(i).layers.at(j).neurons.size(); k++)
                {
                    amount_neurons += 1;
                    amount_connections += region->nets.at(i).layers.at(j).neurons.at(k).connections.size();
                    amount_weights += region->nets.at(i).layers.at(j).neurons.at(k).weights.size();
                    amount_errors += region->nets.at(i).layers.at(j).neurons.at(k).errors.size();
                }
            }
        }
        file << region->id << "\n";

        file << region->root << "\n";
        file << region->subnets.size() << "\n";
        for(int i=0; i< region->subnets.size(); i++)
        {
            file << region->subnets[i] << "\n";
        }
        file << region->inputnets.size() << "\n";
        for(int i=0; i< region->inputnets.size(); i++)
        {
            file << region->inputnets[i] << "\n";
        }
        file << region->outputsnets.size() << "\n";
        for(int i=0; i< region->outputsnets.size(); i++)
        {
            file << region->outputsnets[i] << "\n";
        }
        file << region->calculationorder.size() << "\n";
        for(int i=0; i< region->calculationorder.size(); i++)
        {
            file << region->calculationorder[i] << "\n";
        }
        file << amount_nets << "\n";
        file << amount_layers << "\n";
        file << amount_neurons << "\n";
        file << amount_connections << "\n";
        file << amount_weights << "\n";
        file << amount_errors << "\n";

        for(int i=0; i<amount_nets; i++)
        {
                file << region->nets[i].id << "\n";
                file << region->nets[i].inputtype << "\n";
                file << region->nets[i].root << "\n";
        }
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                file << region->nets.at(i).layers[j].net << "\n";
                file << region->nets.at(i).layers[j].id << "\n";
                file << region->nets.at(i).layers[j].type << "\n";
                file << region->nets.at(i).layers[j].activation << "\n";
                file << region->nets.at(i).layers[j].recursive << "\n";
                file << region->nets.at(i).layers[j].convolution << "\n";
                file << region->nets.at(i).layers[j].biased << "\n";
            }
        }
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                for(int k=0; k<region->nets.at(i).layers.at(j).neurons.size(); k++)
                {
                    file << region->nets[i].layers[j].neurons[k].net << "\n";
                    file << region->nets[i].layers[j].neurons[k].layer << "\n";
                    file << region->nets[i].layers[j].neurons[k].id << "\n";
                    file << region->nets[i].layers[j].neurons[k].type << "\n";
                    file << region->nets[i].layers[j].neurons[k].active << "\n";
                    file << region->nets[i].layers[j].neurons[k].activation << "\n";
                    file << region->nets[i].layers[j].neurons[k].recursive << "\n";
                    file << region->nets[i].layers[j].neurons[k].convolution << "\n";
                    file << region->nets[i].layers[j].neurons[k].convolutional_position << "\n";
                    file << region->nets[i].layers[j].neurons[k].potential << "\n";
                    file << region->nets[i].layers[j].neurons[k].recurse_potential << "\n";
                    file << region->nets[i].layers[j].neurons[k].recurse_weight << "\n";
                    file << region->nets[i].layers[j].neurons[k].connections.size() << "\n";
                    file << region->nets[i].layers[j].neurons[k].weights.size() << "\n";
                }
            }
        }
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                for(int k=0; k<region->nets.at(i).layers.at(j).neurons.size(); k++)
                {
                    for(int l=0; l<region->nets.at(i).layers.at(j).neurons.at(k).connections.size(); l++)
                    {
                        file << region->nets.at(i).layers.at(j).neurons.at(k).net << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).connections[l]->net << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).layer << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).connections[l]->layer << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).id << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).connections[l]->id << "\n";
                    }
                }
            }
        }
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                for(int k=0; k<region->nets.at(i).layers.at(j).neurons.size(); k++)
                {
                    for(int l=0; l<region->nets.at(i).layers.at(j).neurons.at(k).weights.size(); l++)
                    {
                        file << region->nets.at(i).layers.at(j).neurons.at(k).net << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).layer << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).id << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).weights[l] << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).dtweights[l] << "\n";
                    }
                }
            }
        }
        for(int i=0; i<amount_nets; i++)
        {
            for(int j=0; j<region->nets.at(i).layers.size(); j++)
            {
                for(int k=0; k<region->nets.at(i).layers.at(j).neurons.size(); k++)
                {
                    for(int l=0; l<region->nets.at(i).layers.at(j).neurons.at(k).errors.size(); l++)
                    {
                        file << region->nets.at(i).layers.at(j).neurons.at(k).net << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).layer << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).id << "\n";
                        file << region->nets.at(i).layers.at(j).neurons.at(k).errors[l] << "\n";
                    }
                }
            }
        }
    }
}

void Region_Open(Region* region, char* path)
{
    std::ifstream file(path);
    if (file.is_open())
    {

        region->nets.clear();
        int region_id;
        int region_root;
        int region_subnetsize;
        int region_inputnetsize;
        int region_outputnetsize;
        int region_calculationordersize;

        int amount_nets;
        int amount_layers;
        int amount_neurons;
        int amount_connections;
        int amount_weights;
        int amount_errors;
        file >> region_id;
        file >> region_root;
        file >> region_subnetsize;
        for(int i=0; i<region_subnetsize; i++)
        {
            int s;
            file >> s;
            region->subnets.push_back(s);
        }
        file >> region_inputnetsize;
        for(int i=0; i<region_inputnetsize; i++)
        {
            int s;
            file >> s;
            region->inputnets.push_back(s);
        }
        file >> region_outputnetsize;
        for(int i=0; i<region_outputnetsize; i++)
        {
            int s;
            file >> s;
            region->outputsnets.push_back(s);
        }
        file >> region_calculationordersize;
        for(int i=0; i<region_calculationordersize; i++)
        {
            int s;
            file >> s;
            region->calculationorder.push_back(s);
        }

        file >> amount_nets;
        file >> amount_layers;
        file >> amount_neurons;
        file >> amount_connections;
        file >> amount_weights;
        file >> amount_errors;
        for(int i=0; i<amount_nets; i++)
        {
            Net n;
            region->nets.push_back(n);
            file >> region->nets.back().id;
            file >> region->nets.back().inputtype;
            file >> region->nets.back().root;
        }
        for(int i=0; i<amount_layers;i++)
        {
            int net,id,type,activation,recursive,convolution,biased;
            file >> net >> id >> type >> activation >> recursive >> convolution >> biased;
            region->nets.at(net).layers.push_back(L_new(id,0,type,activation,recursive,convolution, biased));
            region->nets.at(net).layers.back().net = net;
        }
        for(int i=0; i<amount_neurons;i++)
        {
            int net, layer,id,type,active,activation,recursive,convolution,convolutional_position,connection_size,weight_size;
            float potential,recurse_potential,recurse_weight;
            file >> net >> layer>> id >> type >>active>>activation>>recursive>>convolution>>convolutional_position>>potential>>recurse_potential>>recurse_weight>>connection_size>>weight_size;
            region->nets.at(net).layers.at(layer).neurons.push_back(N_blanck());
            region->nets.at(net).layers.at(layer).neurons.back().net = net;
            region->nets.at(net).layers.at(layer).neurons.back().layer = layer;
            region->nets.at(net).layers.at(layer).neurons.back().id = id;
            region->nets.at(net).layers.at(layer).neurons.back().type = type;
            region->nets.at(net).layers.at(layer).neurons.back().active = active;
            region->nets.at(net).layers.at(layer).neurons.back().activation = activation;
            region->nets.at(net).layers.at(layer).neurons.back().recursive = recursive;
            region->nets.at(net).layers.at(layer).neurons.back().convolution = convolution;
            region->nets.at(net).layers.at(layer).neurons.back().convolutional_position = convolutional_position;
            region->nets.at(net).layers.at(layer).neurons.back().potential = potential;
            region->nets.at(net).layers.at(layer).neurons.back().recurse_potential = recurse_potential;
            region->nets.at(net).layers.at(layer).neurons.back().recurse_weight = recurse_weight;
        }
        for(int i=0; i<amount_connections;i++)
        {
            int net1, net2, layer1, layer2, id1, id2;
            file >> net1 >> net2 >> layer1>> layer2 >> id1 >> id2;
            Neuron* n1 = &region->nets.at(net1).layers.at(layer1).neurons.at(id1);
            Neuron* n2 = &region->nets.at(net2).layers.at(layer2).neurons.at(id2);
            n1->connections.push_back(n2);
        }
        for(int i=0; i<amount_weights;i++)
        {
            int net, layer, id;
            float weight, dtweight;
            file >> net >> layer >> id >> weight >> dtweight;
            region->nets.at(net).layers.at(layer).neurons.at(id).weights.push_back(weight);
            region->nets.at(net).layers.at(layer).neurons.at(id).dtweights.push_back(dtweight);
        }
        for(int i=0; i<amount_errors;i++)
        {
            int net, layer, id;
            float error;
            file >> net >> layer >> id >> error;
            region->nets.at(net).layers.at(layer).neurons.at(id).errors.push_back(error);
        }
        file.close();
    }
}

void Region_Train_Subnet(Net* net, std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> solutions)
{
    int ssize = inputs.size();
    for(int k=0; k<inputs.size(); k++)
    {
        Net_Train(net,inputs[k],solutions[k],0);
        if(k%(ssize/10)==0 && k!= 0)
        {
            printf("Net: %d is %d percent done.\n",net->id,k*100/ssize);
        }
    }
}

void Region_Train_Subnets(Region* region, std::vector<std::vector<float>> inputs, std::vector<std::vector<float>> solutions)
{
    //printf("Traini-ng subnets \n");
    std::vector<std::thread> threads;
    for(int i=0; i<region->subnets.size(); i++)
    {
        threads.push_back(std::thread(Region_Train_Subnet,&region->nets[region->subnets[i]],inputs,solutions));
    }
    for (auto& th : threads) {
        th.join();
    }
    //&isdone = 1;
}

void Region_Set_Inputs(Region* region, std::vector<float> inputs)
{
    for(auto i : region->inputnets)
    {
        Net_Input(&region->nets[i],inputs);
    }
}

void Region_Calculate(Region* region)
{
    for(int i=0; i<region->calculationorder.size(); i++)
    {
        Net_Calculate(&region->nets[region->calculationorder[i]],1);
    }
}

































