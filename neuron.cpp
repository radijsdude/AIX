
#include"neuron.h"


float activation_function(float potential, int activation, int derivative)
{
    if(derivative==0)
    {
        if(activation==none)
        {
            return potential;
        }
        if(activation==sigmoid)
        {
            if(potential>10.0f)
            {
                return 0.9990f;
            }
            if(potential<-10.0f)
            {
                return 0.00010f;
            }
            return 1.0f/(1.0f+std::exp(-potential));
        }
    }
    else
    {
        if(activation==none)
        {
            return 1.0f;
        }
        if(activation==sigmoid)
        {
            if(potential==1.0f)
            {
                potential = 0.9990f;
            }
            return potential*(1.0f-potential);
        }
    }
}

void N_init(Neuron* n)
{

    n->id = 0;
    n->type = 0;

    n->active = 1;
    n->activation = sigmoid;
    n->recursive = 0;
    n->convolution = 0;
    n->convolutional_position = 0;

    n->potential = 1.0f;
    n->recurse_potential = 0.0f;
    n->recurse_weight = 0.0f;
}

bool N_compare(Neuron* n1, Neuron* n2, int verbose, int serious)
{
    int t = 1;
    if(n1->layer != n2->layer)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons layer mismatch %d != %d\n",n1->layer,n2->layer);
        }
    }
    if(n1->id != n2->id)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons ID mismatch %d != %d\n",n1->id,n2->id);
        }
    }
    if(n1->type != n2->type)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons type mismatch %d != %d\n",n1->type,n2->type);
        }
    }
    if(n1->active != n2->active)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons active mismatch %d != %d\n",n1->active,n2->active);
        }
    }
    if(n1->activation != n2->activation)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons activation mismatch %d != %d\n",n1->activation,n2->activation);
        }
    }
    if(n1->recursive != n2->recursive)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons recursive mismatch %d != %d\n",n1->recursive,n2->recursive);
        }
    }
    if(n1->convolution != n2->convolution)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons convolution mismatch %d != %d\n",n1->convolution,n2->convolution);
        }
    }
    if(n1->convolutional_position != n2->convolutional_position)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons convolutional position mismatch %d != %d\n",n1->convolutional_position,n2->convolutional_position);
        }
    }
    if(n1->potential != n2->potential && serious!=1)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons potential mismatch %f != %f\n",n1->potential,n2->potential);
        }
    }
    if(n1->recurse_potential != n2->recurse_potential && serious!=1)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons recurse potential mismatch %f != %f\n",n1->recurse_potential,n2->recurse_potential);
        }
    }
    if(n1->recurse_weight != n2->recurse_weight && serious!=1)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons recurse weight mismatch %f != %f\n",n1->recurse_weight,n2->recurse_weight);
        }
    }
    if(n1->connections.size()!=n2->connections.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons connections size mismatch %d != %d\n",n1->connections.size(),n2->connections.size());
        }
    }
    else
    {
        if(serious==1)
        {
            for(int i=0; i<n1->connections.size(); i++)
            {
                if(n1->connections[i]->id != n2->connections[i]->id)
                {
                    t = 0;
                    printf("Neurons connections id mismatch %d != %d\n",n1->connections[i]->id,n2->connections[i]->id);
                }
                if(n1->connections[i]->layer != n2->connections[i]->layer)
                {
                    t = 0;
                    printf("Neurons connections layer mismatch %d != %d\n",n1->connections[i]->layer,n2->connections[i]->layer);
                }
            }

        }
    }
    if(n1->weights.size()!=n2->weights.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons weights size mismatch %d != %d\n",n1->weights.size(),n2->weights.size());
        }
    }
    else
    {
        if(serious==1)
        {
            for(int i=0; i<n1->weights.size(); i++)
            {
                if(n1->weights[i] != n2->weights[i])
                {
                    t = 0;
                    if(verbose==1)
                    {
                        printf("Neurons weights mismatch %f != %f\n",n1->weights[i],n2->weights[i]);
                    }
                }
            }
        }
    }
    if(n1->errors.size()!=n2->errors.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons errors size mismatch %d != %d\n",n1->errors.size(),n2->errors.size());
        }
    }
    else
    {
        if(serious==1)
        {
            for(int i=0; i<n1->errors.size(); i++)
            {
                if(n1->errors[i] != n2->errors[i])
                {
                    t = 0;
                    if(verbose == 1)
                    {
                        printf("Neurons errors mismatch %f != %f\n",n1->errors[i],n2->errors[i]);
                    }
                }
            }
        }
    }
    if(n1->dtweights.size()!=n2->dtweights.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Neurons dtweights size mismatch %d != %d\n",n1->dtweights.size(),n2->dtweights.size());
        }
    }
    else
    {
        if(serious==1)
        {
            for(int i=0; i<n1->dtweights.size(); i++)
            {
                if(n1->dtweights[i] != n2->dtweights[i])
                {
                    t = 0;
                    if(verbose==1)
                    {
                        printf("Neurons dtweights mismatch %f != %f\n",n1->dtweights[i],n2->dtweights[i]);
                    }
                }
            }
        }
    }
    return t;
}


Neuron N_copy(Neuron n)
{
    Neuron nn;
    nn.id = n.id;
    nn.type = n.type;

    nn.active = n.active;
    nn.activation = n.activation;
    nn.recursive = n.recursive;
    nn.convolution = n.convolution;
    nn.convolutional_position = n.convolutional_position;

    nn.potential = n.potential;
    nn.recurse_potential = n.recurse_potential;
    nn.recurse_weight = n.recurse_weight;
    for(int i=0; i<n.connections.size();i++)
    {
        nn.connections.push_back(n.connections[i]);
    }
    for(int i=0; i<n.weights.size();i++)
    {
        nn.weights.push_back(n.weights[i]);
    }
    for(int i=0; i<n.errors.size();i++)
    {
        nn.errors.push_back(n.errors[i]);
    }
    for(int i=0; i<n.dtweights.size();i++)
    {
        nn.dtweights.push_back(n.dtweights[i]);
    }
    return nn;
}

void N_print(Neuron n)
{
    printf("\nNeuron\n\n");
    printf("ID : %d\n",n.id);
    printf("type : %d\n",n.type);
    printf("active : %d\n",n.active);
    printf("activation : %d\n",n.activation);
    printf("recursive : %d\n",n.recursive);
    printf("convolution : %d\n",n.convolution);
    printf("\npotential : %f\n",n.potential);
    printf("\nconnections:\n");
    for(int i=0;i<n.connections.size();i++)
    {
        printf("  Id: %d\n",n.connections.at(i)->id);
    }
    printf("\npotentials:\n");
    for(int i=0;i<n.connections.size();i++)
    {
        printf("  x = %f\n",n.connections.at(i)->potential);
    }
    printf("\nweigths:\n");
    for(int i=0;i<n.weights.size();i++)
    {
        printf("  w = %f\n",n.weights.at(i));
    }
    if(n.recursive==1)
    {
        printf("\nrecurse:\n");
        printf("  p: %f   *   %f\n",n.recurse_potential,n.recurse_weight);
    }
    printf("\nEnd Neuron\n\n");
}

Neuron N_new(int layer, int id, int type, int activation, int recursive, int convolution)
{
    Neuron n;
    N_init(&n);

    n.layer = layer;
    n.id = id;
    n.type = type;
    n.activation = activation;
    n.recursive = recursive;
    n.convolution = convolution;
    if(n.convolution==1)
    {
        n.weights.push_back(0.5f);
        n.dtweights.push_back(0.0f);
    }
    else
    {
        n.errors.push_back(0.0f);
    }
    return n;
}

Neuron N_blanck()
{
    Neuron n;
    N_init(&n);
    return n;
}

void N_connect(Neuron* n1, Neuron* n2, float weight)
{
    if(n1->convolution==1)
    {
        n1->connections.push_back(n2);
        n1->errors.push_back(0.0f);
    }
    else
    {
        n1->connections.push_back(n2);
        n1->weights.push_back(weight);
        n1->dtweights.push_back(weight);
    }
}

void N_get_connections(Neuron* n, std::vector<std::vector<int>>* connections)
{
    for(int i=0; i<n->connections.size(); i++)
    {
        std::vector<int> x;
        x.push_back(n->id);
        x.push_back(n->connections.at(i)->id);
        connections->push_back(x);
        if(n->connections.at(i)->convolution==0)
        {
            N_get_connections(n->connections.at(i), connections);
        }
        else
        {
            x.clear();
            x.push_back(n->connections.at(i)->id);
            x.push_back(n->connections.at(i)->connections.at(n->convolutional_position)->id);
            connections->push_back(x);
            N_get_connections(n->connections.at(i)->connections.at(n->convolutional_position),connections);
        }

    }
}

float N_calculate(Neuron* n)
{
    if(n->connections.size()>0){
    float x = 0.0f;
    if(n->convolution!=1)
    {
        if(n->type==summation)
        {
            for(int i=0; i<n->connections.size(); i++)
            {
                if(n->connections.at(i)->convolution==1)
                {
                    x += n->connections.at(i)->connections.at(n->convolutional_position)->potential * n->connections.at(i)->weights.at(0) * n->weights.at(i);
                }
                else
                {
                    x += n->connections.at(i)->potential * n->weights.at(i);
                }
            }
            if(n->recursive==1)
            {
                x += n->recurse_potential * n->recurse_weight;
            }
        }
        if(n->type==multiplice)
        {
            for(int i=0; i<n->connections.size(); i++)
            {
                if(n->connections.at(i)->convolution==1)
                {
                    x *= n->connections.at(i)->connections.at(n->convolutional_position)->potential * n->connections.at(i)->weights.at(0);
                }
                else
                {
                    x *= n->connections.at(i)->potential;
                }
            }
            if(n->recursive==1)
            {
                x *= n->recurse_potential * n->recurse_weight;
            }
        }
        x = activation_function(x,n->activation,0);
        n->potential = x;
    }
    }
    return n->potential;

}

void N_correct(Neuron* n)
{
    if(n->convolution==1)
    {
        for(int i=0; i<n->errors.size(); i++)
        {
            float gradient = n->errors.at(i);
            if(n->connections.at(i)->convolution==1)
            {
                float dw = learningrate * gradient * n->connections.at(i)->connections.at(n->convolutional_position)->potential * n->connections.at(i)->weights.at(0);
                n->weights.at(0) += dw + momentumfactor*n->dtweights.at(0);
                n->dtweights.at(0) = dw;
                n->connections.at(i)->errors.at(n->convolutional_position) += gradient * n->weights.at(0);
            }
            else
            {
                float dw = learningrate * gradient * n->connections.at(i)->potential;
                n->weights.at(0) += dw + momentumfactor*n->dtweights.at(0);
                n->dtweights.at(0) = dw;
                n->connections.at(i)->errors.at(0) += gradient * n->weights.at(0);
            }
            n->errors.at(i) = 0.0f;
        }

    }
    else
    {
        if(n->type==summation)
        {
            float gradient = activation_function(n->potential,n->activation,1) * n->errors.at(0);
            for(int i=0; i<n->connections.size(); i++)
            {
                if(n->connections.at(i)->convolution==1)
                {
                    float dw = learningrate * gradient * n->connections.at(i)->connections.at(n->convolutional_position)->potential *  n->connections.at(i)->weights.at(0);
                    n->weights.at(i) += dw + momentumfactor*n->dtweights.at(i);
                    n->dtweights.at(i) = dw;
                    n->connections.at(i)->errors.at(n->convolutional_position) += gradient * n->weights.at(i);
                }
                else
                {
                    float dw = learningrate * gradient * n->connections.at(i)->potential;
                    n->weights.at(i) += dw + momentumfactor*n->dtweights.at(i);
                    n->dtweights.at(i) = dw;
                    n->connections.at(i)->errors.at(0) += gradient * n->weights.at(i);
                }
            }
            if(n->recursive==1)
            {
                n->recurse_weight += learningrate * gradient * n->recurse_potential;
            }
        }
        if(n->type==multiplice)
        {
            float gradient = activation_function(n->potential,n->activation,1) * n->errors.at(0);
            for(int i=0; i<n->connections.size(); i++)
            {
                if(n->connections.at(i)->convolution==1)
                {
                    n->connections.at(i)->errors.at(n->convolutional_position) += gradient * n->potential /
                    (n->connections.at(i)->connections.at(n->convolutional_position)->potential * n->connections.at(i)->weights.at(0));
                }
                else
                {
                    n->connections.at(i)->errors.at(0) += gradient * n->potential / n->connections.at(i)->potential;
                }
            }
            if(n->recursive==1)
            {
                n->recurse_weight += learningrate * gradient * n->potential / n->recurse_weight;
            }
        }
        n->errors.at(0) = 0.0f;
    }

}

void N_shift_recurse(Neuron* n)
{
    if(n->recursive)
    {
        n->recurse_potential = n->potential;
    }
}

int N_test1()
{
    int t = 1;
    printf("Neuron Test 1.\n");
    Neuron n1 = N_new(0,1,0,1,0,0);
    //N_print(n1);
    Neuron n2 = N_new(1,2,0,1,0,0);
    //N_print(n2);

    N_connect(&n1,&n2, 0.13f);
    //N_print(n1);
    n2.potential = 13.0f;
    //printf("13 == %f\n",n1.connections.at(0)->potential);
    if(n1.connections.at(0)->potential!=13.0f)
    {
        printf("Neuron misconnection %f\n",n1.connections.at(0)->potential);
        t = 0;
    }
    n2.potential = 1.0f;

    float s = 0.12345678987654321f;
    float error = 0.0f;
    for(int i=0;i<10000;i++)
    {
        error = N_calculate(&n1) - s;
        n1.errors.at(0) = -error;
        N_correct(&n1);
    }
    if((n1.potential - s)*(n1.potential - s)>0.001)
    {
        printf("Neuron miscalc %f\n",n1.potential - s);
        t = 0;
    }
    //N_print(n1);
    //printf("%f   ===   %f",s,n1.potential);
    if(t==1)
    {
        printf("Neuron Test 1 Succes\n");
    }
    else
    {
        printf("Neuron Test 1 Fail\n");
    }
    return t;
}

int N_test2()
{
    int t = 1;
    printf("Neuron Test 2.\n");

    Neuron n1 = N_new(0,1,0,0,0,0);
    Neuron n2 = N_new(1,2,0,0,0,0);

    Neuron n3 = N_new(2,3,0,0,0,1);

    Neuron n4 = N_new(3,4,0,0,0,0);


    N_connect(&n3,&n1, 0.13f);
    N_connect(&n3,&n2, 0.13f);
    N_connect(&n4,&n3, 0.13f);

    float s = 0.12345678987654321f;
    float error = 0.0f;
    for(int i=0;i<10000;i++)
    {
        N_calculate(&n3);
        error = N_calculate(&n4) - s;
        n4.errors.at(0) = -error;
        N_correct(&n4);
        N_correct(&n3);
    }
    //N_print(n4);
    if((n4.potential - s)*(n4.potential - s)>0.001)
    {
        printf("Neuron miscalc %f\n",n4.potential - s);
        t = 0;
    }
    if(t==1)
    {
        printf("Neuron Test 2 Succes\n");
    }
    else
    {
        printf("Neuron Test 2 Fail\n");
    }
    return t;
}

int N_test3()
{
    printf("Neuron Test 3.\n");
    int t = 1;
    Neuron n1 = N_new(0,1,0,0,0,0);
    Neuron n2 = N_new(0,2,0,0,0,0);
    Neuron n2_ = N_new(0,22,0,0,0,0);

    Neuron n3 = N_new(0,3,0,0,0,1);

    Neuron n4 = N_new(0,4,0,1,0,0);
    Neuron n5 = N_new(0,5,0,1,0,0);
    Neuron n6 = N_new(0,6,0,1,0,0);

    Neuron n7 = N_new(0,7,0,1,0,0);
    Neuron n8 = N_new(0,8,0,1,0,0);

    n4.convolutional_position = 0;
    n5.convolutional_position = 1;
    n6.convolutional_position = 2;


    N_connect(&n3,&n1, 0.13f);
    N_connect(&n3,&n2, 0.13f);
    N_connect(&n3,&n2_, 0.13f);

    N_connect(&n4,&n3, 0.13f);
    N_connect(&n5,&n3, 0.13f);
    N_connect(&n6,&n3, 0.13f);

    N_connect(&n7,&n4, 0.13f);
    N_connect(&n7,&n5, 0.13f);
    N_connect(&n7,&n6, 0.13f);
    N_connect(&n8,&n4, 0.13f);
    N_connect(&n8,&n5, 0.13f);
    N_connect(&n8,&n6, 0.13f);
    //even
    for(int i=0; i<100000; i++)
    {
        n1.potential = (float)(i%2) + 0.01f;
        n2.potential = (float)((i+1)%2) + 0.01f;
        n2_.potential = 1.0f;
        float e4 = (float)((i+0)%2) - N_calculate(&n4);
        float e5 = (float)((i+1)%2) - N_calculate(&n5);
        n4.errors.at(0) = e4;
        n5.errors.at(0) = e5;
        N_correct(&n5);
        N_correct(&n4);
        N_correct(&n3);
    }
    n1.potential = 0.01f;
    n2.potential = 0.99f;
    n2_.potential = 1.0f;
    N_calculate(&n4);
    N_calculate(&n5);
    if(n4.potential>0.5f)
    {
        t = 0;
    }
    if(n5.potential<0.5f)
    {
        t = 0;
    }
    if(t==0){printf("First fail\n");}

    n1 = N_new(0,1,0,0,0,0);
    n2 = N_new(0,2,0,0,0,0);

    n3 = N_new(0,3,0,0,0,1);

    n4 = N_new(0,4,0,1,0,0);
    n5 = N_new(0,5,0,1,0,0);
    n4.convolutional_position = 0;
    n5.convolutional_position = 1;


    N_connect(&n3,&n1, 0.13f);
    N_connect(&n3,&n2, 0.13f);
    N_connect(&n4,&n3, 0.13f);
    N_connect(&n5,&n3, 0.13f);
    // odd
    for(int i=0; i<100000; i++)
    {
        n1.potential = (float)(i%2) + 0.01f;
        n2.potential = (float)((i+1)%2) + 0.01f;
        float e4 = (float)((i+1)%2) - N_calculate(&n4);
        float e5 = (float)((i+0)%2) - N_calculate(&n5);
        n4.errors.at(0) = e4;
        n5.errors.at(0) = e5;
        N_correct(&n5);
        N_correct(&n4);
        N_correct(&n3);
    }
    n1.potential = 0.01f;
    n2.potential = 0.99f;
    N_calculate(&n4);
    N_calculate(&n5);
    int tt = 1;
    if(n4.potential<0.5f)
    {
        tt = 0;
    }
    if(n5.potential>0.5f)
    {
        tt = 0;
    }
    if(tt==0){printf("Second fail\n");}


    if(t==1&&tt==1)
    {
        printf("Neuron Test 3 Succes\n");
    }
    else
    {
        printf("Neuron Test 3 Fail\n");
    }
    return t;
}









