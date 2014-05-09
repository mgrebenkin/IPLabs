#include "Noises.h"

double N(unsigned char* z)
{
	const double sigma = 25;
	const unsigned char z0 = 0;
	return (1 / sqrt(2 * M_PI * sigma))*exp((*z-z0)/(2*sigma*sigma));
}

void AddNoise(NPngProc::SImage& in)
{
	for (size_t y = 1; y <= in.nHeight; y++)
		for (size_t x = 1; x <= in.nWidth; x++)
		{
			*in(x, y) = *in(x, y) + (char)N(in(x,y));
		}
}

void PulseNoise(NPngProc::SImage& in)
{

}