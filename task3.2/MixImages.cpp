#include "MixImages.h"


void MixImages(SImage &in1, SImage &in2, SImage &alpha, SImage &out)
{
	double buf;
	for (size_t y = 1; y <= in1.nHeight; y++)
		for (size_t x = 1; x <= in1.nWidth; x++)
		{
			buf = *in1(x, y)*(*alpha(x, y) / 255.0)
				+
				*in2(x, y)*(0xFF - *alpha(x, y)) / 255.0;
			
			if (buf > 0xFF) *out(x, y) = 0xFF;
			else *out(x, y) = (unsigned char)buf;
		}
}