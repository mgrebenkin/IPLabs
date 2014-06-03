#include "stuff.h"

void ToBin(NPngProc::SImage& in, int thresh)
{
	for (size_t y = 1; y <= in.nHeight; y++)
		for (size_t x = 1; x <= in.nWidth; x++)
		{
		if (*in(x, y) < thresh) *in(x, y) = 0;
		else *in(x, y) = 0xFF;
		}
}

int FindAreas(NPngProc::SImage& in)
{
	unsigned char* pMapBits = new unsigned char[in.nWidth*in.nHeight];
	memset(pMapBits, 0, in.nWidth*in.nHeight);
	//это карта связанных областей
	//каждая область получает свое значение пикселов
	NPngProc::SImage map(pMapBits, in.nWidth, in.nWidth, 8); 
	size_t nArea = 0;					//счетчик областей
	//текущий рассматриваемый пиксел, то биш его координаты
	SPoint start_point;

	for (size_t y = 1; y <= in.nHeight; y++)
		for (size_t x = 1; x <= in.nWidth; x++)
		{
			if (*in(x, y) == 0) *map(x, y) = 0;
			else if (*map(x, y) == 0 )
			{
				nArea++;
				start_point.x = x; start_point.y = y;
				Fill(in, map, start_point, nArea); //с этой точки начинается выделение всей связной области
			}
		}
	return nArea;
}

void Fill(NPngProc::SImage& in, NPngProc::SImage& map, SPoint start_point, size_t nArea)
{

	//список пикселов, включенных в область. Их соседей также предстоит проверить на включение
	std::list<SPoint> pix;
	
	
	SPoint p;
	pix.push_back(start_point);

	while (pix.size() != 0)
	{
		p = pix.back();
		pix.pop_back();
		*map(p.x, p.y) = nArea;
		
		if (p.x + 1 <= in.nWidth)
			if (*in(p.x + 1, p.y) != 0 && *map(p.x + 1, p.y) == 0) 
				pix.emplace_back(p.x + 1, p.y);
		if (p.y + 1 <= in.nHeight)
			if (*in(p.x, p.y + 1) != 0 && *map(p.x, p.y + 1) == 0) 
				pix.emplace_back(p.x, p.y + 1);
		if (p.x - 1 >= 1)
			if (*in(p.x - 1, p.y) != 0 && *map(p.x - 1, p.y) == 0) 
				pix.emplace_back(p.x - 1, p.y);
		if (p.y - 1 >= 1)
			if (*in(p.x, p.y - 1) != 0 && *map(p.x, p.y - 1) == 0) 
				pix.emplace_back(p.x, p.y - 1);
	}
}