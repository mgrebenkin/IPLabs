#include "ContrastTransf.h"


void ContrastTransf(NPngProc::SImage& in)
{
	for (size_t y = 1; y <= in.nHeight; y++)
	{
		for (size_t x = 1; x <= in.nWidth; x++)
		{
			if (*in(x, y) <= __S1) *in(x, y) = __D1;
			else if (*in(x, y) >= __S2) *in(x, y) = __D2;
			else
			{
				*in(x, y) = pow(((double)*in(x, y) - __S1)/(__S2 - __S1), 1/__gamma)*
					(__D2 - __D1) + __D1;
			}
		}
	}
}