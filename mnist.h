#ifndef MNIST_H_INCLUDED
#define MNIST_H_INCLUDED


#include <iostream>
#include <vector>
#include <fstream>
#include "region.h"
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

typedef unsigned char uchar;

//std::string path_image_train = "C:\\Users\\Thomas\\Downloads\\train-images.idx3-ubyte";
//std::string path_labels_train = "C:\\Users\\Thomas\\Downloads\\train-images.idx3-ubyte";

//std::string path_image_test = "C:\\Users\\Thomas\\Downloads\\t10k-images-idx3-ubyte";
//std::string path_labels_test = "C:\\Users\\Thomas\\Downloads\\t10k-labels-idx3-ubyte";

std::vector<float> mnist_image_to_paddedvector(int xpadding, int ypadding, int root, uchar* train_image);
uchar** read_mnist_images(std::string full_path, int& number_of_images, int& image_size);
uchar* read_mnist_labels(std::string full_path, int& number_of_labels);


std::vector<float> mnist_image_to_vector(uchar* image);
std::vector<float> mnist_label_to_vector(uchar label);


void mnist_train_region_subnets(Region* region, const char* path_image_train, const char* path_labels_train);
std::vector<float> mnist_label_to_vector_length(uchar label, int length);

#endif // MNIST_H_INCLUDED
