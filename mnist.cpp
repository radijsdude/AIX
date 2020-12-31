

#include"mnist.h"

//using namespace std;


auto reverseInt = [](int i) {
    unsigned char c1, c2, c3, c4;
    c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
};

std::vector<float> mnist_image_to_paddedvector(int xpadding, int ypadding, int root, uchar* train_image)
{
    std::vector<float> input;
    int row_size = 28;
    for(int i=0; i<root; i++)
    {
        for(int j=0; j<root; j++)
        {
            if((i >= xpadding && i < xpadding+row_size) && (j >= ypadding && j < ypadding+row_size))
            {
                input.push_back((float)(train_image[row_size * (i-xpadding) + (j-ypadding)])/256.0f);
            }
            else
            {
                input.push_back(0.0f);
            }
        }
    }
    return input;
}

void mnist_train_region_subnets(Region* region, const char* path_image_train, const char* path_labels_train)
{
    int train_cycle = 1000;
    int number_of_images;
    uchar** train_images;
    uchar* train_labels;
    int row_size = 28;
    int imagesize = row_size*row_size;
    train_images = read_mnist_images(path_image_train,number_of_images,imagesize);
    train_labels = read_mnist_labels(path_labels_train,number_of_images);
    int interupt = 0;
    while(interupt == 0)
    {
        printf("MNIST Training Thread running\n");
        if(region->training==2)
        {
            interupt = 1;
            break;
        }
        if(region->training==1)
        {
            std::vector<std::vector<float>> inputs;
            std::vector<std::vector<float>> solutions;
            for(int t=0; t<train_cycle; t++)
            {
                int xpadding = rand()%(region->root - row_size);
                int ypadding = rand()%(region->root - row_size);
                int rimg = rand()%number_of_images;
                inputs.push_back(mnist_image_to_paddedvector(xpadding,ypadding,region->root,train_images[rimg]));
                solutions.push_back(mnist_label_to_vector_length(train_labels[rimg],64));
            }
            Region_Train_Subnets(region,inputs,solutions);
        }
        else
        {
            if(region->training == 0)
            {
                Sleep(1000);
            }
        }
    }
}


uchar** read_mnist_images(std::string full_path, int& number_of_images, int& image_size)
{
    std::ifstream file(full_path, std::ios::binary);

    if(file.is_open()) {
        int magic_number = 0, n_rows = 0, n_cols = 0;

        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2051) throw std::runtime_error("Invalid MNIST image file!");

        file.read((char *)&number_of_images, sizeof(number_of_images)), number_of_images = reverseInt(number_of_images);
        file.read((char *)&n_rows, sizeof(n_rows)), n_rows = reverseInt(n_rows);
        file.read((char *)&n_cols, sizeof(n_cols)), n_cols = reverseInt(n_cols);

        image_size = n_rows * n_cols;

        uchar** _dataset = new uchar*[number_of_images];
        for(int i = 0; i < number_of_images; i++)
        {
            _dataset[i] = new uchar[image_size];
            file.read((char *)_dataset[i], image_size);
        }
        file.close();
        return _dataset;
    }
    else
    {
        throw std::runtime_error("Cannot open file `" + full_path + "`!");
    }
}

uchar* read_mnist_labels(std::string full_path, int& number_of_labels)
{

    typedef unsigned char uchar;

    std::ifstream file(full_path, std::ios::binary);

    if(file.is_open()) {
        int magic_number = 0;
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2049) throw std::runtime_error("Invalid MNIST label file!");

        file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

        uchar* _dataset = new uchar[number_of_labels];
        for(int i = 0; i < number_of_labels; i++) {
            file.read((char*)&_dataset[i], 1);
        }
        file.close();
        return _dataset;
    } else {
        throw std::runtime_error("Unable to open file `" + full_path + "`!");
    }
}

std::vector<float> mnist_image_to_vector(uchar* image)
{
    std::vector<float> vimage;
    for(int j=0; j<28*28;j++)
    {
        vimage.push_back((float)((int)image[j])/256.0f);
    //printf("%d ",(int)train_images[0][i]);
    }
    return vimage;
}
std::vector<float> mnist_label_to_vector(uchar label)
{
    std::vector<float> vlabel;
    for(int j=0; j < 62; j++)
    {
        vlabel.push_back(0.0f);
    }
    int ttt = (int)label;
    vlabel.at(ttt) = 0.990f;
    return vlabel;
}
std::vector<float> mnist_label_to_vector_length(uchar label, int length)
{
    std::vector<float> vlabel;
    for(int j=0; j < length; j++)
    {
        vlabel.push_back(0.0f);
    }
    int ttt = (int)label;
    if(ttt<length)
    {
        vlabel.at(ttt) = 0.990f;
    }
    return vlabel;
}
