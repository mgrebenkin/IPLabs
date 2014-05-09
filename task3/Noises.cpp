#include "Noises.h"


void AddNoise(NPngProc::SImage& in, double z0, double sigma)
{

	std::default_random_engine generator;
	std::normal_distribution<double> dist(z0, sigma);
	double rnd;
	for (size_t y = 1; y <= in.nHeight; y++)
		for (size_t x = 1; x <= in.nWidth; x++)
		{
			rnd = dist(generator);
			if (*in(x, y) + (char)rnd < 0)  
				*in(x, y) = 0;
			else if (*in(x, y) + (char)rnd > 255) 
				*in(x, y) = 255;
			else 
				*in(x, y) = *in(x, y) + (char)rnd;
		}
}

void PulseNoise(NPngProc::SImage& in, double Ps)
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	double rnd;
	char N;

	if (Ps < 0) Ps = 0;
	else if (Ps > 1) Ps = 1;

	for (size_t y = 1; y <= in.nHeight; y++)
		for (size_t x = 1; x <= in.nWidth; x++)
		{
			rnd = dist(generator);
			if (rnd < Ps / 2) *in(x, y) = 0;
			else if (rnd < Ps) *in(x, y) = 0xFF;
		}

}