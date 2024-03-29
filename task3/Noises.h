#ifndef __NOISES_H_INCLUDED__
#define __NOISES_H_INLCUDED__

#define _USE_MATH_DEFINES

#include <cmath>
#include <random>
#include "PngProc.h"



void AddNoise(NPngProc::SImage& in, double z0, double sigma );
void PulseNoise(NPngProc::SImage& in, double Ps);

#endif