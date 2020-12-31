#include "net.h"

void hilbertpoints(std::vector<std::vector<float>>* points, float x0, float y0, float xi, float xj, float yi, float yj, int n)
{
    float x,y;
    if (n <= 0)
    {
        x = (x0 + (xi + yi) / 2.0f);
        y = (y0 + (xj + yj) / 2.0f);
        std::vector<float> temp;
        temp.push_back(x);
        temp.push_back(y);
        points->push_back(temp);
    }
    else
    {
        hilbertpoints(points,x0, y0, yi / 2.0f, yj / 2.0f, xi / 2.0f, xj / 2.0f, n - 1);
        hilbertpoints(points,x0 + xi / 2.0f, y0 + xj / 2.0f, xi / 2.0f, xj / 2.0f, yi / 2.0f, yj / 2.0f, n - 1);
        hilbertpoints(points,x0 + xi / 2.0f + yi / 2.0f, y0 + xj / 2.0f + yj / 2.0f, xi / 2.0f, xj / 2.0f, yi / 2.0f, yj / 2, n - 1);
        hilbertpoints(points,x0 + xi / 2.0f + yi, y0 + xj / 2.0f + yj, -yi / 2.0f, -yj / 2.0f, -xi / 2.0f, -xj / 2.0f, n - 1);
    }
}
std::vector<std::vector<int>> hilbertprocessing(std::vector<std::vector<float>> points)
{
    float x0 = 0.0f;
    float y0 = 0.0f;
    int xx0 = 0;
    int yy0 = 0;
    std::vector<int> temp;
    std::vector<std::vector<int>> sol;
    for(int i=0; i<points.size(); i++)
    {
        if(points[i][0] > x0){temp.push_back(xx0 + 1);}
        if(points[i][0] == x0){temp.push_back(xx0);}
        if(points[i][0] < x0){temp.push_back(xx0 - 1);}

        if(points[i][1] > y0){temp.push_back(yy0 + 1);}
        if(points[i][1] == y0){temp.push_back(yy0);}
        if(points[i][1] < y0){temp.push_back(yy0 - 1);}

        sol.push_back(temp);
        x0 = points[i][0];
        y0 = points[i][1];
        xx0 = temp[0];
        yy0 = temp[1];
        temp.clear();
    }
    return sol;
}
void Net_Input(Net* net, std::vector<float> potentials)
{
    int dx = potentials.size() - net->layers[0].neurons.size();
    if(dx < 0)
    {
        for(int i=0; i< -dx; i++)
        {
            potentials.push_back(0.0f);
        }
    }
    if(net->inputtype==normal)
    {
        L_set_potentials(&net->layers[0], potentials);
    }
    if(net->inputtype==hilbert)
    {
        std::vector<float> potentialsh;
        std::vector<std::vector<float>> potentials2D;
        std::vector<float> temp;
        for(int i=0; i<potentials.size(); i++)
        {
            if(i%net->root == 0 && i != 0)
            {
                potentials2D.push_back(temp);
                temp.clear();
            }
            temp.push_back(potentials[i]);
        }
        potentials2D.push_back(temp);
        std::vector<std::vector<float>> pointsf;
        std::vector<std::vector<int>> pointsi;
        hilbertpoints(&pointsf,0,0,1,0,0,1,5);
        pointsi = hilbertprocessing(pointsf);
        //printf("hilbertpointsize %d\n",pointsi.size());
        for(auto i : pointsi)
        {
            if(i[0]<=net->root&&i[1]<=net->root)
            {
                potentialsh.push_back(potentials2D[i[0]-1][i[1]-1]);
                //printf("hilbertpoint %d , %d \n",i[0],i[1]);
            }
        }
        L_set_potentials(&net->layers[0], potentialsh);
    }
}

bool Net_compare(Net* net1, Net* net2, int verbose, int serious)
{
    int t = 1;
    if(net1->layers.size() != net2->layers.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Net size mismatch %d != %d\n",net1->layers.size(),net2->layers.size());
        }
    }
    else
    {
        for(int i=0; i<net1->layers.size(); i++)
        {
            if(L_compare(&net1->layers[i],&net2->layers[i],verbose, serious))
            {
                t = 0;
            }
        }
    }
    return t;
}

void Net_Create(Net* net, int id , int type, int inputsize, int outputsize, int inputtype)
{
    net->layers.clear();
    net->id = id;
    net->inputtype = inputtype;
    int root = (int) sqrt((float)inputsize);
    net->root = root;
    switch(type)
    {
        case BASIC:
            Net_Append_Layer(net,L_new(0,inputsize,summation,sigmoid,0,0,0));
            Net_Connectconv_Layer(net,0,CONV,root);
            Net_Connectconv_Layer(net,0,CONVT,root);
            Net_Add_Layers(net,2,4);
            Net_Append_Layer(net,L_new(8,outputsize,summation,sigmoid,0,0,0));
            Net_Connect_Layer(net,net->layers.size() -1, net->layers.size() -2,FULLY);
            break;
        case BASIC32:
            root = 32;
            Net_Append_Layer(net,L_new(0,32*32,summation,sigmoid,0,0,0));
            Net_Connectconv_Layer(net,0,CONV,root);
            Net_Connectconv_Layer(net,0,CONV,root/2);
            Net_Add_Layers(net,net->layers.size() -1,net->layers.size() -3);
            Net_Connectconv_Layer(net,0,CONV,root/4);
            Net_Add_Layers(net,net->layers.size() -1,net->layers.size() -3);
            Net_Connectconv_Layer(net,0,CONV,root/8);
            Net_Add_Layers(net,net->layers.size() -1,net->layers.size() -3);
            Net_Connectconv_Layer(net,0,CONV,root/16);
            Net_Add_Layers(net,net->layers.size() -1,net->layers.size() -3);


            Net_Append_Layer(net,L_new(8,64,summation,sigmoid,0,0,0));
            Net_Connect_Layer(net,net->layers.size() -1, net->layers.size() -2,FULLY);
            break;

        case EXP:
            root = 32;
            Net_Append_Layer(net,L_new(0,32*32,summation,sigmoid,0,0,0));
            Net_Connectconv_Layer(net,0,STRAFE,root);
            Net_Append_Layer(net,L_new(8,64,summation,sigmoid,0,0,0));
            Net_Connect_Layer(net,net->layers.size() -1, net->layers.size() -2,FULLY);
            break;
    }
}

void Net_Append_Layer(Net* net, Layer layer)
{
    net->layers.push_back(layer);
    Net_Name_layers(net);
}

void Net_Name_layers(Net* net)
{
    for(int i=0; i<net->layers.size(); i++)
    {
        net->layers[i].net = net->id;
        net->layers[i].id = i;
        for(int j=0; j<net->layers[i].neurons.size(); j++)
        {

            net->layers[i].neurons[j].net = net->id;
            net->layers[i].neurons[j].layer = i;
            net->layers[i].neurons[j].id = j;
        }
    }
}

void Net_Add_Layers(Net* net, int layer1, int layer2)
{
    Layer l3 = L_copy(net->layers[layer1]);
    L_add(&net->layers[layer1],&net->layers[layer2],&l3);
    net->layers.push_back(l3);
    if(layer1 > layer2)
    {
        net->layers.erase(net->layers.begin() + layer1);
        net->layers.erase(net->layers.begin() + layer2);
    }
    else
    {
        net->layers.erase(net->layers.begin() + layer2);
        net->layers.erase(net->layers.begin() + layer1);
    }
    Net_Name_layers(net);
}

void Net_Connect_Layer(Net* net, int layer1, int layer2, int connection)
{
    switch (connection)
    {
    case FULLY:
        L_connect_fully(&net->layers[layer1],&net->layers[layer2]);
        break;
    case SINGLE:
        L_connect_single(&net->layers[layer1],&net->layers[layer2]);
        break;
    }
    Net_Name_layers(net);
}

void Net_Connectconv_Layer(Net* net, int layer1, int connection, int convsize)
{

    int l2 = net->layers.size();
    Layer layer2 = L_new(l2,convsize,0,0,0,1,0);
    net->layers.push_back(layer2);
    int l3 = net->layers.size();
    Layer layer3 = L_new(l3,2,summation,sigmoid,0,0,0);
    net->layers.push_back(layer3);
    switch (connection)
    {
        case CONV:
            L_connect_convolution(&net->layers[layer1],&net->layers[l2], &net->layers[l3]);
            break;
        case CONVT:
            L_connect_convolution_T(&net->layers[layer1],&net->layers[l2], &net->layers[l3]);
            break;
        case STRAFE:
            L_connect_convolution_strafe(&net->layers[layer1],&net->layers[l2], &net->layers[l3]);
            break;

    }
    Net_Name_layers(net);
}

void Net_Calculate(Net* net, int layer0)
{
    for(int i=(layer0 +1)%2; i<net->layers.size();i++)
    {
        L_calculate(&net->layers.at(i));
    }
}

void Net_Correct(Net* net, int layer0)
{
    for(int i=net->layers.size()-1; i>=(layer0 +1)%2;i--)
    {
        L_correct(&net->layers.at(i));
    }
}

void Net_Train(Net* net, std::vector<float> inputs, std::vector<float> solutions, int layer0)
{
    Net_Input(net,inputs);
    Net_Calculate(net,layer0);
    std::vector<float> errors = L_loss(&net->layers.back(),solutions,crossentropy,0);
    L_set_errors(&net->layers.back(),errors);
    Net_Correct(net, layer0);
}

void Net_Save(Net* net, char* path)
{
    std::ofstream file(path);
    if (file.is_open())
    {
        int amount_layers = net->layers.size();
        int amount_neurons = 0;
        int amount_connections = 0;
        int amount_weights = 0;
        int amount_errors = 0;
        for(int i=0; i<amount_layers; i++)
        {
            amount_neurons += net->layers[i].neurons.size();
            for(int j=0; j<net->layers[i].neurons.size(); j++)
            {
                amount_connections += net->layers[i].neurons[j].connections.size();
                amount_weights += net->layers[i].neurons[j].weights.size();
                amount_errors += net->layers[i].neurons[j].errors.size();
            }
        }
        file << net->id << "\n";
        file << net->inputtype << "\n";
        file << net->root << "\n";
        file << amount_layers << "\n";
        file << amount_neurons << "\n";
        file << amount_connections << "\n";
        file << amount_weights << "\n";
        file << amount_errors << "\n";

        for(int i=0; i<amount_layers; i++)
        {
                file << net->layers[i].id << "\n";
                file << net->layers[i].type << "\n";
                file << net->layers[i].activation << "\n";
                file << net->layers[i].recursive << "\n";
                file << net->layers[i].convolution << "\n";
                file << net->layers[i].biased << "\n";
        }

        for(int i=0; i<amount_layers; i++)
        {
            for(int j=0; j<net->layers[i].neurons.size(); j++)
            {
                file << net->layers[i].neurons[j].layer << "\n";
                file << net->layers[i].neurons[j].id << "\n";
                file << net->layers[i].neurons[j].type << "\n";
                file << net->layers[i].neurons[j].active << "\n";
                file << net->layers[i].neurons[j].activation << "\n";
                file << net->layers[i].neurons[j].recursive << "\n";
                file << net->layers[i].neurons[j].convolution << "\n";
                file << net->layers[i].neurons[j].convolutional_position << "\n";
                file << net->layers[i].neurons[j].potential << "\n";
                file << net->layers[i].neurons[j].recurse_potential << "\n";
                file << net->layers[i].neurons[j].recurse_weight << "\n";
                file << net->layers[i].neurons[j].connections.size() << "\n";
                file << net->layers[i].neurons[j].weights.size() << "\n";
            }
        }
        for(int i=0; i<amount_layers; i++)
        {
            for(int j=0; j<net->layers[i].neurons.size(); j++)
            {
                for(int k=0; k<net->layers[i].neurons[j].connections.size(); k++)
                {
                    file << net->layers[i].neurons[j].connections[k]->net << "\n";
                    file << net->layers[i].neurons[j].layer << "\n";
                    file << net->layers[i].neurons[j].connections[k]->layer << "\n";
                    file << net->layers[i].neurons[j].id << "\n";
                    file << net->layers[i].neurons[j].connections[k]->id << "\n";
                }
            }
        }
        for(int i=0; i<amount_layers; i++)
        {
            for(int j=0; j<net->layers[i].neurons.size(); j++)
            {
                for(int k=0; k<net->layers[i].neurons[j].weights.size(); k++)
                {
                    file << net->layers[i].neurons[j].layer << "\n";
                    file << net->layers[i].neurons[j].id << "\n";
                    file << net->layers[i].neurons[j].weights[k] << "\n";
                    file << net->layers[i].neurons[j].dtweights[k] << "\n";
                }
            }
        }

        for(int i=0; i<amount_layers; i++)
        {
            for(int j=0; j<net->layers[i].neurons.size(); j++)
            {
                for(int k=0; k<net->layers[i].neurons[j].errors.size(); k++)
                {
                    file << net->layers[i].neurons[j].layer << "\n";
                    file << net->layers[i].neurons[j].id << "\n";
                    file << net->layers[i].neurons[j].errors[k] << "\n";
                }
            }
        }
        file.close();
    }
}

void Net_Open(Net* net, char* path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        net->layers.clear();
        int amount_layers = 0;
        int amount_neurons = 0;
        int amount_connections = 0;
        int amount_weights = 0;
        int amount_errors = 0;
        file >> net->id;
        file >> net->inputtype;
        file >> net->root;

        file >> amount_layers;
        file >> amount_neurons;
        file >> amount_connections;
        file >> amount_weights;
        file >> amount_errors;
        for(int i=0; i<amount_layers;i++)
        {
            int id,type,activation,recursive,convolution,biased;
            file >> id >> type >> activation >> recursive >> convolution >> biased;
            net->layers.push_back(L_new(id,0,type,activation,recursive,convolution, biased));
        }
        for(int i=0; i<amount_neurons;i++)
        {
            int layer,id,type,active,activation,recursive,convolution,convolutional_position,connection_size,weight_size;
            float potential,recurse_potential,recurse_weight;
            file >>layer>> id >> type >>active>>activation>>recursive>>convolution>>convolutional_position>>potential>>recurse_potential>>recurse_weight>>connection_size>>weight_size;
            for(int j=0; j<net->layers.size();j++)
            {
                if(net->layers[j].id == layer)
                {
                    net->layers[j].neurons.push_back(N_blanck());
                    net->layers[j].neurons.back().net = net->id;
                    net->layers[j].neurons.back().layer = layer;
                    net->layers[j].neurons.back().id = id;
                    net->layers[j].neurons.back().type = type;
                    net->layers[j].neurons.back().activation = activation;
                    net->layers[j].neurons.back().recursive = recursive;
                    net->layers[j].neurons.back().convolution = convolution;
                    net->layers[j].neurons.back().convolutional_position = convolutional_position;
                    net->layers[j].neurons.back().potential = potential;
                    net->layers[j].neurons.back().recurse_potential = recurse_potential;
                    net->layers[j].neurons.back().recurse_weight = recurse_weight;
                }
            }

        }
        for(int i=0; i<amount_connections;i++)
        {
            int net2, layer1, layer2, id1, id2;
            file >> net2 >> layer1>> layer2 >> id1 >> id2;
            if(net2 == net->id)
            {
                Neuron* n1;
                Neuron* n2;
                for(int j=0; j<net->layers.size();j++)
                {
                    if(net->layers[j].id == layer1)
                    {
                        for(int k=0; k<net->layers[j].neurons.size(); k++)
                        {
                            if(net->layers[j].neurons[k].id == id1)
                            {
                                n1 = &net->layers[j].neurons[k];
                            }
                        }
                    }
                    if(net->layers[j].id == layer2)
                    {
                        for(int k=0; k<net->layers[j].neurons.size(); k++)
                        {
                            if(net->layers[j].neurons[k].id == id2)
                            {
                                n2 = &net->layers[j].neurons[k];
                            }
                        }
                    }
                }
                n1->connections.push_back(n2);
            }
        }

        for(int i=0; i<amount_weights;i++)
        {
            int layer, id;
            float weight, dtweight;
            file >> layer >> id >> weight >> dtweight;
            for(int j=0; j<net->layers.size();j++)
            {
                if(net->layers[j].id == layer)
                {
                    for(int k=0; k<net->layers[j].neurons.size(); k++)
                    {
                        if(net->layers[j].neurons[k].id == id)
                        {
                            net->layers[j].neurons[k].weights.push_back(weight);
                            net->layers[j].neurons[k].dtweights.push_back(dtweight);
                        }
                    }
                }
            }
        }
        for(int i=0; i<amount_errors;i++)
        {
            int layer, id;
            float error;
            file >> layer >> id >> error;
            for(int j=0; j<net->layers.size();j++)
            {
                if(net->layers[j].id == layer)
                {
                    for(int k=0; k<net->layers[j].neurons.size(); k++)
                    {
                        if(net->layers[j].neurons[k].id == id)
                        {
                            net->layers[j].neurons[k].errors.push_back(error);
                        }
                    }
                }
            }
        }
        file.close();
        //printf("Layers: %d, Neurons: %d\n",amount_layers,amount_neurons);
    }
}
