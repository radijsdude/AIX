
#include"sdldraw.h"

void draw_neuron(SDL_Renderer* renderer, Neuron selected, int ix1, int iy, int px, int py, float scale)
{
    int ix2 = ix1;
    int y0 = iy;
    char c[50];
    snprintf(c,sizeof c,"id %d",selected.id);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"layer %d",selected.layer);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"net %d",selected.net);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"type %d",selected.type);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"recursive %d",selected.recursive);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    if(selected.recursive == 1)
    {
        snprintf(c,sizeof c,"rec pot %f",selected.recurse_potential);
        draw_text(renderer,ix2,iy,250,250,250,c);

        iy += 20;
        snprintf(c,sizeof c,"rec wei %f",selected.recurse_weight);
        draw_text(renderer,ix2,iy,250,250,250,c);
    }
    iy += 20;
    snprintf(c,sizeof c,"convolution %d",selected.convolution);
    draw_text(renderer,ix2,iy,250,250,250,c);

    iy += 20;
    snprintf(c,sizeof c,"conv pos %d",selected.convolutional_position);
    draw_text(renderer,ix2,iy,250,250,250,c);
    iy += 20;
    snprintf(c,sizeof c,"potential %f",selected.potential);
    draw_text(renderer,ix2,iy,250,250,250,c);
    if(selected.convolution==1)
    {
        iy += 20;
        snprintf(c,sizeof c,"conv weight: %f",selected.weights.at(0));
        draw_text(renderer,ix1+25,iy,250,250,250,c);
    }
    else
    {
        for(int i = 0; i< selected.connections.size();i++)
        {
            iy += 20;
            snprintf(c,sizeof c,"%d",selected.connections.at(i)->id);
            draw_text(renderer,ix1+25,iy,250,250,250,c);
        }
        iy -= 20 * selected.connections.size();
        for(int i = 0; i< selected.weights.size();i++)
        {
            iy += 20;
            snprintf(c,sizeof c,"%f",selected.weights.at(i));
            draw_text(renderer,ix2-25,iy,250,250,250,c);
        }
    }
    iy = y0;
    draw_text(renderer,ix2+100,iy,250,250,250,"errors:");
    for(int i = 0; i< selected.errors.size();i++)
    {
        iy += 20;
        snprintf(c,sizeof c,"%f",selected.errors.at(i));
        draw_text(renderer,ix2+150,iy,250,250,250,c);
    }
}

void draw_neurons(SDL_Renderer* renderer, Region* region,int bx,int by, int mx, int my, int px, int py, float scale, int roott)
{
    int rn = 5;
    int cn = -1;
    for(int i=0; i<region->nets.size();i++)
    {
        int root = roott;
        if(roott<0)
        {
            root = region->nets[i].root;
        }
        for(int j=0; j<region->nets[i].layers.size();j++)
        {
            for(int k=0; k<region->nets[i].layers[j].neurons.size();k++)
            {
                int net1 = region->nets[i].layers[j].neurons[k].net;
                int layer1 = region->nets[i].layers[j].neurons[k].layer;
                int neuron1 = region->nets[i].layers[j].neurons[k].id;
                float pot = region->nets[i].layers[j].neurons[k].potential;
                float pot2 = 0.0f;
                int x1 = bx + px + layer1 * rn*3*root;
                int y1 = by + py + 2*rn * neuron1 + root * rn * 4 * net1;
                x1 += neuron1/root * rn * 2;
                y1 = by + py + (neuron1 % root) * rn * 2 + root * rn * 3 * net1;
                x1 = (int)(scale*(float)(x1));
                y1 = (int)(scale*(float)(y1));
                int rnn = (int)(scale*(float)(rn));
                if(region->nets[i].layers[j].neurons[k].convolution==1)
                {
                    pot = region->nets[i].layers[j].neurons[k].weights[0];
                }
                if (pot < 0.0f)
                {
                    pot2 = -pot;
                    pot = 0.0f;
                }
                if (pot > 1.0f)
                {
                    pot = 1.0f;
                }
                if (pot2 > 1.0f)
                {
                    pot2 = 1.0f;
                }
                draw_circle(renderer,x1,y1,rnn+1,50,50,50,250,1);
                draw_circle(renderer,x1,y1,rnn,(int)(250.0f*pot2),(int)(250.0f*pot),0,250,1);
                if((mx-x1)*(mx-x1)<rnn*rnn && (my-y1)*(my-y1)<rnn*rnn)
                {

                    draw_neuron(renderer,region->nets[i].layers[j].neurons[k],100,screeny-200,px,py,scale);
                    for(int ii=0; ii<region->nets[net1].layers[layer1].neurons[neuron1].connections.size(); ii++)
                    {
                        int net2,layer2,neuron2;
                        net2 = region->nets[net1].layers[layer1].neurons[neuron1].connections[ii]->net;
                        layer2 = region->nets[net1].layers[layer1].neurons[neuron1].connections[ii]->layer;
                        neuron2 = region->nets[net1].layers[layer1].neurons[neuron1].connections[ii]->id;
                        int x2 = bx + px + layer2 * rn*3*root;
                        int y2 = by + py + 2*rn * neuron2 + root * rn * 4 * net2;
                        x2 += neuron2/root * rn * 2;
                        y2 = by + py + (neuron2 % root) * rn * 2 + root * rn * 3 * net2;
                        x2 = (int)(scale*(float)(x2));
                        y2 = (int)(scale*(float)(y2));
                        draw_line(renderer,x1,y1,x2,y2,250,250,250,250,1);
                        if(region->nets[net1].layers[layer1].neurons[neuron1].connections[ii]->convolution==1)
                        {
                            Neuron* n3 = region->nets[net1].layers[layer1].neurons[neuron1].connections[ii]->connections[region->nets[net1].layers[layer1].neurons[neuron1].convolutional_position];
                            int net3,layer3,neuron3;
                            net3 = n3->net;
                            layer3 = n3->layer;
                            neuron3 = n3->id;
                            int x3 = bx + px + layer3 * rn*3*root;
                            int y3 = by + py + 2*rn * neuron3 + root * rn * 4 * net3;
                            x3 += neuron3/root * rn * 2;
                            y3 = by + py + (neuron3 % root) * rn * 2 + root * rn * 3 * net3;
                            x3 = (int)(scale*(float)(x3));
                            y3 = (int)(scale*(float)(y3));
                            draw_line(renderer,x3,y3,x2,y2,250,250,250,250,1);
                        }
                    }
                }
            }
        }
    }
}


void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r,int g,int b,int a, int filled)
{
    SDL_SetRenderDrawColor(renderer,r,g,b,a);
    if(x2-x1==0)
    {
        if(y1<y2)
        {
             for(int i=y1;i < y2; i++)
            {
                SDL_RenderDrawPoint(renderer,x1,i);
            }
        }
        if(y1>y2)
        {
             for(int i=y2;i < y1; i++)
            {
                SDL_RenderDrawPoint(renderer,x1,i);
            }
        }
    }
    float m = (float)(y2 - y1)/(float)(x2 - x1);
    if(x2-x1>0)
    {
        for(int i=0;i < x2-x1; i++)
        {
            SDL_RenderDrawPoint(renderer,x1 + i, y1 + i*m);
        }
    }
    if(x2-x1<0)
    {
        for(int i=0;i < x1-x2; i++)
        {
            SDL_RenderDrawPoint(renderer,x2 + i, y2 + i*m);
        }
    }
}

void draw_rect(SDL_Renderer *renderer, int x, int y, int b, int l, int cr,int cg,int cb,int ca, int filled)
{
    SDL_SetRenderDrawColor(renderer,cr,cg,cb,ca);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = b;
    r.h = l;
    SDL_RenderDrawRect(renderer, &r);
    SDL_RenderFillRect(renderer, &r);
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int r, int color_r, int color_g, int color_b, int color_a, int filled)
{
    SDL_SetRenderDrawColor(renderer,color_r,color_g,color_b,color_a);
    if (r < 2){r = 2;}
    if(filled == 1)
    {
        for (int i = -r; i <= r; i++)
        {
            for (int j = 0; j*j<=r*r-i*i;j++)
            {
                SDL_RenderDrawPoint(renderer,x + i, y + j);
                SDL_RenderDrawPoint(renderer,x + i, y - j);
            }
        }
    }
    else
    {
        for (int i = -r; i <= r; i++)
        {
            SDL_RenderDrawPoint(renderer,x + i, y + (int)sqrt(r*r - (x+i)*(x+i)));
            SDL_RenderDrawPoint(renderer,x + i, y - (int)sqrt(r*r - (x+i)*(x+i)));
        }
    }

}


void draw_text(SDL_Renderer* renderer,int x, int y, int r,int g,int b, const char* txt)
{
    TTF_Font * font = TTF_OpenFont("arial.ttf", 11);
    SDL_Color color = { r, g, b };
    SDL_Surface * surface = TTF_RenderText_Solid(font,txt, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { x, y, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void draw_float(SDL_Renderer* renderer,int x, int y, int r,int g,int b, float f)
{
    char c[10];
    snprintf(c,sizeof c,"%f",f);
    draw_text(renderer,x,y,255,255,255,c);
}

void draw_int(SDL_Renderer* renderer,int x, int y, int r,int g,int b, int f)
{
    char c[10];
    snprintf(c,sizeof c,"%d",f);
    draw_text(renderer,x,y,255,255,255,c);
}
