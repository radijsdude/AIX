#include"layer.h"
void L_init(Layer* l)
{
    l->id = 0;
    l->type = 0;
    l->activation = 0;
    l->recursive = 0;
    l->convolution = 0;
    l->biased = 0;
}

bool L_compare(Layer* l1, Layer* l2, int verbose, int serious)
{
    int t = 1;
    if(l1->id != l2->id)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers ID mismatch %d != %d\n",l1->id,l2->id);
        }
    }
    if(l1->type != l2->type)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers type mismatch %d != %d\n",l1->type,l2->type);
        }
    }
    if(l1->activation != l2->activation)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers activation mismatch %d != %d\n",l1->activation,l2->activation);
        }
    }
    if(l1->recursive != l2->recursive)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers recursive mismatch %d != %d\n",l1->recursive,l2->recursive);
        }
    }
    if(l1->convolution != l2->convolution)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers convolution mismatch %d != %d\n",l1->convolution,l2->convolution);
        }
    }
    if(l1->biased != l2->biased)
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers biased mismatch %d != %d\n",l1->biased,l2->biased);
        }
    }
    if(l1->neurons.size() != l2->neurons.size())
    {
        t = 0;
        if(verbose==1)
        {
            printf("Layers size mismatch %d != %d\n",l1->convolution,l2->convolution);
        }
    }
    else
    {
        for(int i=0; i<l1->neurons.size(); i++)
        {
            if(N_compare(&l1->neurons[i],&l2->neurons[i],verbose,serious)==0)
            {
                t = 0;
            }
        }
    }
    return t;
}

Layer L_copy(Layer l)
{
    Layer ll;
    ll.id = l.id;
    ll.type = l.type;
    ll.activation = l.activation;
    ll.recursive = l.recursive;
    ll.convolution = l.convolution;
    ll.biased = l.biased;
    for(int i=0; i<l.neurons.size(); i++)
    {
        ll.neurons.push_back(N_copy(l.neurons[i]));
    }
    return ll;
}

void L_print(Layer l)
{
    printf("\nLayer\n\n");
    printf("ID : %d\n",l.id);
    printf("type : %d\n",l.type);
    printf("activation : %d\n",l.activation);
    printf("recursive : %d\n",l.recursive);
    printf("convolution : %d\n",l.convolution);
    printf("biased : %d\n",l.biased);
}

Layer L_new(int id, int s, int type, int activation, int recursive, int convolution, int biased)
{
    Layer l;
    l.id = id;
    l.type = type;
    l.activation = activation;
    l.recursive = recursive;
    l.convolution = convolution;
    l.biased = biased;
    for(int i=0; i<s; i++)
    {
        l.neurons.push_back(N_new(id,i,type,activation,recursive,convolution));
    }
    if(biased==1)
    {
        l.neurons.push_back(N_new(id,-1,0,0,0,0));
    }
    return l;
}

Layer L_blanck()
{
    Layer l;
    L_init(&l);
    return l;
}

void L_set_potentials(Layer* l, std::vector<float> potentials)
{
    if(potentials.size()>=l->neurons.size())
    {
        for(int i=0; i<l->neurons.size(); i++)
        {
            l->neurons.at(i).potential = potentials.at(i);
        }
        if(potentials.size()>l->neurons.size())
        {
            printf("L input potentials bigger then neurons size \n");
        }
    }
    if(potentials.size()<l->neurons.size())
    {
        for(int i=0; i<potentials.size(); i++)
        {
            l->neurons.at(i).potential = potentials.at(i);
        }
    }
}

void L_set_errors(Layer* l, std::vector<float> errors)
{
    //printf("L_set_errors\n");
    if(l->neurons.size()<=errors.size())
    {
        for(int i=0; i<l->neurons.size(); i++)
        {
            l->neurons.at(i).errors.at(0) = errors.at(i);
        }
    }
    else
    {
        printf("L_set_errors: l->neurons.size() > errors.size()\n");
        for(int i=0; i<errors.size(); i++)
        {
            l->neurons.at(i).errors.at(0) = errors.at(i);
        }
    }

}

void L_calculate(Layer* l)
{

    for(int i=0; i<l->neurons.size(); i++)
    {
        N_calculate(&l->neurons.at(i));
    }
}

void L_correct(Layer* l)
{

    for(int i=0; i<l->neurons.size(); i++)
    {
        N_correct(&l->neurons.at(i));
    }
}

void L_connect_fully(Layer* l1, Layer* l2)
{
    for(int i=0; i<l1->neurons.size(); i++)
    {
        for(int j=0; j<l2->neurons.size(); j++)
        {
            N_connect(&l1->neurons.at(i),&l2->neurons.at(j),(float)(rand()%100 - 40)/50.0f);
        }
    }
}
void L_connect_single(Layer* l1, Layer* l2)
{
    for(int i=0; i<l1->neurons.size(); i++)
    {
        N_connect(&l1->neurons.at(i),&l2->neurons.at(i),(float)(rand()%100 - 40)/50.0f);
    }
}

void L_connect_convolution(Layer* linput, Layer* lconv, Layer* loutput)
{
    int outputsize = linput->neurons.size() / lconv->neurons.size();
    loutput->neurons.clear();
    for(int i=0; i< outputsize; i++)
    {
        Neuron n = N_new(loutput->id,i,0,1,1,0);
        n.convolutional_position = i;
        loutput->neurons.push_back(n);
        for(int j=0; j< lconv->neurons.size(); j++)
        {
            N_connect(&lconv->neurons.at(j),&linput->neurons.at(i*lconv->neurons.size() + j),1.0f);
            N_connect(&loutput->neurons.at(i),&lconv->neurons.at(j),(float)(rand()%100 - 40)/50.0f);
        }
    }
}

void L_connect_convolution_T(Layer* linput, Layer* lconv, Layer* loutput)
{
    int outputsize = linput->neurons.size() / lconv->neurons.size();
    loutput->neurons.clear();
    for(int i=0; i< outputsize; i++)
    {
        Neuron n = N_new(loutput->id,i,0,1,1,0);
        n.convolutional_position = i;
        loutput->neurons.push_back(n);
    }
    for(int j=0; j< lconv->neurons.size(); j++)
    {
        for(int i=0; i< outputsize; i++)
        {
            N_connect(&lconv->neurons.at(j),&linput->neurons.at(i + j*outputsize),1.0f);
            N_connect(&loutput->neurons.at(i),&lconv->neurons.at(j),(float)(rand()%100 - 40)/50.0f);
        }
    }
}

void L_connect_convolution_strafe(Layer* linput, Layer* lconv, Layer* loutput)
{
    loutput->neurons.clear();
    for(int i=0; i< linput->neurons.size() - lconv->neurons.size(); i++)
    {
        for(int j=0; j<lconv->neurons.size(); j++)
        {
            N_connect(&lconv->neurons[j],&linput->neurons[i+j],1.0f);
        }
    }
    for(int i=0; i< linput->neurons.size() - lconv->neurons.size(); i++)
    {
        Neuron n = N_new(loutput->id,i,0,1,1,0);
        n.convolutional_position = i;
        loutput->neurons.push_back(n);
        for(int j=0; j<lconv->neurons.size(); j++)
        {
            N_connect(&loutput->neurons.back(),&lconv->neurons[j],1.0f);
        }
    }
}

void L_add(Layer* l1, Layer* l2, Layer* l3)
{
    l3->neurons = l1->neurons;
    l3->neurons.insert(l3->neurons.end(),l2->neurons.begin(),l2->neurons.end());
}

int L_max(Layer* layer)
{
    float x = 0.0f;
    int j = 0.0f;
    for(int i=0; i <layer->neurons.size();i++)
    {
        if(layer->neurons.at(i).potential>x)
        {
            j = i;
            x = layer->neurons.at(i).potential;
        }
    }
    return j;
}

std::vector<float> L_loss(Layer*l, std::vector<float> solutions, int loss_function, int total)
{
    std::vector<float> errors;
    float y = 0.0f;
    if(l->neurons.size() <= solutions.size())
    {
        if(loss_function==quadratic)
        {
            for(int i=0; i<l->neurons.size(); i++)
            {
                float x = -(l->neurons.at(i).potential - solutions.at(i));
                if(total==0)
                {
                    errors.push_back(x);
                }
                else
                {
                    y += x;
                }
            }
        }
        if(loss_function==crossentropy)
        {
            for(int i=0; i<l->neurons.size(); i++)
            {
                float x = solutions.at(i) / l->neurons.at(i).potential + (solutions.at(i) - 1.0f)/(1.0f-l->neurons.at(i).potential);
                if(total==0)
                {
                    errors.push_back(x);
                }
                else
                {
                    y += x;
                }
            }
        }
        if(total==1)
        {
            errors.push_back(y);
            return errors;
        }
        else
        {
            return errors;
        }
    }
    else
    {
        printf("L_loss l->neurons.size() > solutions.size()\n");
    }
    return errors;
}

std::vector<Layer*> layerstest()
{

    Layer l0 = L_new(0,12,0,0,0,0,0);
    Layer l1 = L_new(1,4,summation,sigmoid,0,1,0);
    Layer l2 = L_new(2,4,summation,sigmoid,0,0,0);
    L_connect_convolution_T(&l0,&l1,&l2);
    Layer l3 = L_new(3,8,summation,sigmoid,0,0,0);
    L_connect_fully(&l3,&l2);

    std::vector<Layer*> layers;
    layers.push_back(&l0);
    layers.push_back(&l1);
    layers.push_back(&l2);
    layers.push_back(&l3);

    std::vector<float> potentials1;
    std::vector<float> solutions1;

    std::vector<float> potentials2;
    std::vector<float> solutions2;
    for(int i=0; i < l0.neurons.size(); i++)
    {
        potentials1.push_back((float)(i%2) +0.01f);
        potentials2.push_back((float)((i+1)%2+0.01f));
    }
    for(int i=0; i < l3.neurons.size(); i++)
    {
        solutions1.push_back((float)((i+1)%2+0.01f));
        solutions2.push_back((float)((i+0)%2+0.01f));
    }


    for(int i=0; i< 100000; i++)
    {
        if(i%2==0)
        {
            L_set_potentials(&l0,potentials1);
        }
        else
        {
            L_set_potentials(&l0,potentials2);
        }
        L_calculate(&l1);
        L_calculate(&l2);
        L_calculate(&l3);
        std::vector<float> errors;
        if(i%2==0)
        {
            errors = L_loss(&l3,solutions1,quadratic,0);
        }
        else
        {
            errors = L_loss(&l3,solutions2,quadratic,0);
        }
        L_set_errors(&l3,errors);
        L_correct(&l3);
        L_correct(&l2);
        L_correct(&l1);
    }
    return layers;
}

void Layers_calculate(std::vector<Layer*> layers)
{
    for(int i=1; i<layers.size();i++)
    {
        L_calculate(layers.at(i));
    }
}
void Layers_correct(std::vector<Layer*> layers)
{
    for(int i=layers.size()-1; i>=0;i--)
    {
        L_correct(layers.at(i));
    }
}

void Layers_learn(std::vector<Layer*> layers, std::vector<float> inputs, std::vector<float> solutions)
{
    L_set_potentials(layers.at(0),inputs);
    Layers_calculate(layers);
    std::vector<float> errors = L_loss(layers.back(),solutions,crossentropy,0);
    L_set_errors(layers.back(),errors);
    Layers_correct(layers);
}





















