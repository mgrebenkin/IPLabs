#pragma once

#include <list>
#include "PngProc.h"

struct SPoint
{
	SPoint(){};
	SPoint(size_t _x, size_t _y){ x = _x; y = _y; }
	size_t x;
	size_t y;
};

void ToBin(NPngProc::SImage& in, int thresh);

int FindAreas(NPngProc::SImage& in);

void Fill(NPngProc::SImage& in, NPngProc::SImage& map, SPoint p, size_t nArea);
