#include "Median.h"

using namespace std;

void MakeApt(NPngProc::SImage& apt)
{
	bool done = false;
	size_t rows, cols;
	int buf;
	while (!done)
	{
		cout << "How mane rows in your aperture? Odd and under 5." << endl;
		cin >> rows;
		if (rows >= 1 && rows <= 5 && rows%2 == 1) done = true;
		else cout << "Invalid amout of rows." << endl;
	}
	done = false;

	while (!done)
	{
		cout << "How mane columns in your aperture? Odd and under 5." << endl;
		cin >> cols;
		if (cols >= 1 && cols <= 5 && cols % 2 == 1) done = true;
		else cout << "Invalid amout of columns." << endl;
	}

	unsigned char* pApt = new unsigned char[rows*cols];
	
	apt.nHeight = rows;
	apt.nWidth = cols;
	apt.pBits = pApt;
	for (size_t y = 1; y <= apt.nHeight; y++)
	{
		for (size_t x = 1; x <= apt.nWidth; x++)
		{
			cout << "Row: " << y << "; Col: " << x << ";" << endl;
			cin >> buf;
			*apt(x, y) = buf%0x0100;
		}
	}
}

void SortShit(unsigned char* vals, size_t len)
{
	bool done = false;
	unsigned char buf;
	while (!done)
	{
		done = true;
		for (size_t i = 0; i < len - 1; i++)
		{
			if (vals[i] > vals[i + 1]) 
			{
				buf = vals[i];
				vals[i] = vals[i + 1];
				vals[i + 1] = buf;
				done = false;
			}
		}
	}
}

unsigned char getMed(unsigned char* vals, size_t len)
{
	if (len % 2 == 1)
	{
		return vals[(len - 1) / 2];
	}
	else
	{
		return ((vals[(len / 2) - 1] + vals[(len / 2)])) / 2;
	}
}

size_t getCap(NPngProc::SImage& apt)
{
	size_t cnt = 0;
	for (size_t y = 1; y <= apt.nHeight; y++)
	{
		for (size_t x = 1; x <= apt.nWidth; x++)
		{
			cnt += *apt(x, y);
		}
	}
	return cnt;
}

void Median(NPngProc::SImage& in, NPngProc::SImage& out, NPngProc::SImage& apt)
{
	int aptH = (apt.nHeight - 1) / 2; //имеется ввиду ширина и высота по одну сторону от середины
	int aptW = (apt.nWidth - 1) / 2;
	size_t pos;
	size_t n = getCap(apt);
	//getCap просто просматривает, сколько значений захватит апертура из рассматриваемого окна
	unsigned char* vals = new unsigned char[n];
	
	for (size_t y = aptH + 1; y <= in.nHeight - aptH; y++)
	{
		for (size_t x = aptW + 1; x <= in.nWidth - aptW; x++)
		{
			pos = 0;
			for (int j = -aptH; j <= aptH; j++)
			{
				for (int i = -aptW; i <= aptW; i++)
				{
						for (int k = 1; k <= *apt(i + aptW + 1, j + aptH + 1); k++)
						vals[pos] = *in(x + i, y + j);
						pos++;
				}
			}
			SortShit(vals, n);
			*out(x, y) = getMed(vals, n); //высчитывается медиана из ряда
		}
	}
	delete[] vals;
}