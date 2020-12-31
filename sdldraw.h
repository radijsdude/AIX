#ifndef SDLDRAW_H_INCLUDED
#define SDLDRAW_H_INCLUDED


#include<SDL2/SDL.h>
#include<TTF/SDL_ttf.h>
#include<string>
#include"region.h"

#define screeny 800
#define screenx 1200
#define m_pi 3.14152692

void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r,int g,int b,int a, int filled);

void draw_rect(SDL_Renderer *renderer, int x, int y, int b, int l, int cr,int cg,int cb,int ca, int filled);


void draw_circle(SDL_Renderer *renderer, int x, int y, int r, int color_r, int color_g, int color_b, int color_a, int filled);

void draw_text(SDL_Renderer* renderer,int x, int y, int r,int g,int b, const char* txt);

void draw_float(SDL_Renderer* renderer,int x, int y, int r,int g,int b, float f);

void draw_int(SDL_Renderer* renderer,int x, int y, int r,int g,int b, int f);

void draw_neurons(SDL_Renderer* renderer, Region* region, int bx,int by, int mx, int my, int px, int py, float scale, int roott);

void draw_neuron(SDL_Renderer* renderer, Neuron selected, int ix1, int iy, int px, int py, float scale);


#endif // SDLDRAW_H_INCLUDED
