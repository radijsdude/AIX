#include "includes.h"

std::string path_image_train = "C:\\Users\\Thomas\\Downloads\\emnist-byclass-train-images-idx3-ubyte";
std::string path_labels_train = "C:\\Users\\Thomas\\Downloads\\emnist-byclass-train-labels-idx1-ubyte";

std::string path_image_test = "C:\\Users\\Thomas\\Downloads\\emnist-byclass-test-images-idx3-ubyte";
std::string path_labels_test = "C:\\Users\\Thomas\\Downloads\\emnist-byclass-test-labels-idx1-ubyte";

int main(int argv, char** args)
{
    printf("Seraph\n");
    int testing = 0;
    if(testing == 1)
    {
        N_test1();
        N_test2();
        N_test3();

        Net net1;
        Net net;
        Net_Save(&net1,"test.txt");
        Net_Open(&net,"test.txt");

        Net_compare(&net1,&net,0,1);
    }

    printf("Setup\n");
    int mnist = 1;
    int outputsize = 62;
    int number_of_images;
    int number_of_images2;
    uchar** train_images;
    uchar* train_labels;
    uchar** train_images2;
    uchar* train_labels2;
    int row_size = 28;
    int imagesize = row_size*row_size;

    std::vector<float> image;
    std::vector<float> label;

    std::vector<Layer*> layers;
    Layer solution = L_new(0,outputsize,0,0,0,0,0);

    printf("Setup done\n");

    if(mnist == 1 && testing == 0)
    {
        printf("Mnist\n");
        train_images = read_mnist_images(path_image_train,number_of_images,imagesize);
        train_labels = read_mnist_labels(path_labels_train,number_of_images);
        train_images2 = read_mnist_images(path_image_test,number_of_images2,imagesize);
        train_labels2 = read_mnist_labels(path_labels_test,number_of_images2);
        printf("Mnist done\n");
    }





    printf("Creating net\n");
    Net net;
    Net_Create(&net,0,STRAFE,32*32,62,normal);
    Net_Save(&net,"net_basic.txt");
    Region region;
    Region_Open_Net(&region,"net_basic.txt",NETINPUT,1,1,-1);
    Region_Open_Net(&region,"net_basic.txt",NETINPUT,1,1,-1);
    //L_connect_fully(&region.nets[1].layers[0],&region.nets[0].layers.back());

    Region_Save(&region,"region.txt");
    region.nets.clear();
    region.subnets.clear();
    region.inputnets.clear();
    region.outputsnets.clear();
    region.calculationorder.clear();
    region.root = 0;
    region.id = 0;
    Region_Open(&region,"region.txt");
    region.nets[0].inputtype = hilbert;
    region.root = 32;
    region.training = 0;

    std::vector<std::vector<int>> neurons;
    Region_get_neurons(&region,&neurons);
    std::vector<std::vector<int>> connections;
    Region_get_neurons(&region,&connections);


    printf("Creating net done\n");


    printf("Setting up GUI\n");

    char* filepath = "hello.txt";


    for(int i=0; i<net.layers.size(); i++)
    {
        layers.push_back(&net.layers[i]);
    }


    clock_t start, stop;

    SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();

	window = SDL_CreateWindow("Arys",50,50,screenx,screeny,0);
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);


    int mx = 0;
    int my = 0;

    int px = 0;
    int py = 0;
    float scale = 0.6f;

    int learnings = 10000;

    int randimage = 0;

    int running = 1;

    int correct = 0;
    int cc = 0;
    float accuracy = 0.0f;
    int temp = 0;
    int temp2 = 0;

    srand((int)clock());

    printf("Setting up GUI done\n");


    std::thread mnist_train_thread(mnist_train_region_subnets,&region,"C:\\Users\\Thomas\\Downloads\\emnist-byclass-train-images-idx3-ubyte"
                                                              ,"C:\\Users\\Thomas\\Downloads\\emnist-byclass-train-labels-idx1-ubyte");


	while(running==1 && testing == 0)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					SDL_GetMouseState(&mx,&my);
					break;
				case SDL_MOUSEWHEEL:
				    if(event.wheel.y > 0) // scroll up
                    {
                         scale += 0.1f;
                    }
                    else if(event.wheel.y < 0 && scale > 0.1f) // scroll down
                    {
                         scale -= 0.1f;
                    }
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_RIGHT:
							break;
                        case SDLK_w:
                            py += (int)(50.0f*scale);
                            break;
                        case SDLK_s:
                            py -= (int)(50.0f*scale);
                            break;
                        case SDLK_a:
                            px += (int)(50.0f*scale);
                            break;
                        case SDLK_d:
                            px -= (int)(50.0f*scale);
                            break;
						case SDLK_t:
                            region.training = (region.training + 1)%2;
						    break;
                        case SDLK_g:
                            region.training = 2;
						    break;
						case SDLK_LEFT:
							break;
						case SDLK_r:
						    randimage = rand()%number_of_images;
						    image.clear();
						    image = mnist_image_to_paddedvector(0,0,region.root,train_images[randimage]);
                            label.clear();
                            label = mnist_label_to_vector(train_labels[randimage]);
                            Region_Set_Inputs(&region,image);
                            Region_Calculate(&region);
							break;
						case SDLK_UP:
							break;
                        case SDLK_o:
                            draw_text(renderer,20,20,250,250,250,"Saving Region");
                            SDL_RenderPresent(renderer);
                            Region_Save(&region,filepath);
							break;
                        case SDLK_i:
                            draw_text(renderer,20,20,250,250,250,"Opening Region");
                            SDL_RenderPresent(renderer);
                            Region_Open(&region,filepath);
							break;
						case SDLK_DOWN:
							break;
						case SDLK_y:
						    printf("Testing\n");
						    draw_text(renderer,20,20,250,250,250,"Testing");
                            SDL_RenderPresent(renderer);
                            correct = 0;
                            cc = 0;
						    for(int i=0; i< number_of_images2; i++)
                            {
                                if(i%(number_of_images2/10)==0)
                                {
                                    printf("Testing %d % Done\n",cc*10);
                                    cc += 1;
                                }
                                image.clear();
                                image = mnist_image_to_vector(train_images2[i]);
                                Net_Input(&net, image);
                                Net_Calculate(&net,0);
                                temp = (int)train_labels2[i];
                                temp2 = L_max(&net.layers.back());

                                if(temp==temp2)
                                {
                                    correct+=1;
                                }

                            }
                            accuracy = ((float)(correct))/((float)(number_of_images2));
						    printf("Testing Done\n");
							break;
						case SDLK_j:
							break;
                        case SDLK_k:
							break;

					}
			}

		}

		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

        draw_neurons(renderer,&region,100,100,mx,my,px,py,scale,32);

		SDL_RenderPresent(renderer);
        SDL_Delay(10);

	}
    printf("Seraph terminating\n");

    SDL_Quit();
    TTF_Quit();
    region.training = 2;
    mnist_train_thread.join();


    printf("Seraph done\n");

    return 0;
}
