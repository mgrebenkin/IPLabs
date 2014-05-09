#include "PngProc.h"
#include <iostream>
#include <string>

#define a -0.5

double K(double sigma);

void Zoom(NPngProc::SImage* in, NPngProc::SImage* out, double N);
	unsigned char Interp(NPngProc::SImage* in, 
	unsigned int sqx, unsigned int sqy,
	double origX, double origY);

void main()
{
	std::string strInputFile;
	double N;
	std::cout << "Input image file: ";	//запрос имени файла и коэффициента увеличения
	//std::getline(std::cin, strInputFile);
	strInputFile = "Goldhill8.png";
	size_t nReqSize = NPngProc::readPngFile(strInputFile.c_str(), 0, 0, 0, 0);

	if (nReqSize == NPngProc::PNG_ERROR)
	{
		std::cout << "Error ocured while png file was read." << std::endl;
		std::getchar();
		return;
	}

	std::cout << "Input zooming rate: ";
	std::cin >> N;
	if (N <= 0) { std::cout << "Incorrect rate"; std::getchar();  return; }

	//получение имени нового файла

	std::string strOutputFile(strInputFile);
	int dotPos = strOutputFile.find_last_of('.');
	strOutputFile.insert(dotPos, "_proc");
	


	//чтение файла и создание структуры изображения
	unsigned char* pInputBits = new unsigned char[nReqSize];
	if (!pInputBits)
	{
		std::cout << "Can't allocate memory for input image, required size is " 
			<< nReqSize <<std::endl;
		std::getchar();
		return;
	}
	//CBitsPtrGuard InputBitsPtrGuard(&pInputBits);

	size_t nInWidth, nInHeight;
	unsigned int nBPP = 8;
	NPngProc::readPngFileGray(strInputFile.c_str(), pInputBits, &nInWidth, &nInHeight );

	NPngProc::SImage imageIn(pInputBits, nInWidth, nInHeight, nBPP);

	size_t nOutWidth = (int)ceil(nInWidth*N);
	size_t nOutHeight = (int)ceil(nInHeight*N);

	size_t nGotSize = nOutWidth*nOutHeight;			//наверное нужен такой размер

	unsigned char* pOutputBits = new unsigned char[nGotSize];
	if (!pInputBits)
	{
		std::cout << "Can't allocate memory for output image, required size is "
			<< nGotSize << std::endl;
	}

	NPngProc::SImage imageOut(pOutputBits, nOutWidth, nOutHeight, nBPP);

	//обработка

	Zoom(&imageIn, &imageOut, N);


	//сохранение в файле
	if (NPngProc::writePngFile(strOutputFile.c_str(),
		imageOut.pBits, imageOut.nWidth, imageOut.nHeight, imageOut.nBPP)
		== NPngProc::PNG_ERROR)
	{
		std::cout << "Unable to write .png file." << std::endl;
		std::getchar();
		return;
	}

	std::cout << "Done. Name of output file is " << strOutputFile << std::endl;
	std::getchar();
}


void Zoom(NPngProc::SImage* in, NPngProc::SImage* out, double N)
{
	unsigned char new_pix;						//значение пиксела

	double origX, origY;						//координаты, в которые отображается пиксел в старом
												//изображении
	
	unsigned int sqx, sqy;						//начальная точка квадрата интерполяции

	/*
	start
	 
		|	 |	  |	   |
	____|____|____|____|____
		|	 |	  |	   |
	____|____|____|____|____
		|	 |	  |	   |
	____|____|____|____|____
	    |	 |	  |	   |
	____|____|____|____|____
		|	 |	  |	   |
		|	 |	  |	   |
	
	*/

	for (unsigned int x = 1; x <= out->nWidth; x++)		//перебор пикселов нового изображения
	{
		for (unsigned int y = 1; y <= out->nHeight; y++)
		{
			origX = x / N;						//определение, какой квадрат будет 
												//задействован при рассчете данного пиксела
			if (origX < 2)	sqx = 1;			 
			else if (origX >= in->nWidth - 1) sqx = in->nWidth - 3;
			else sqx = (int)floor(origX) - 1;

			origY = y / N;
			if (origY < 2) sqy = 1;
			else if (origY >= in->nHeight - 1) sqy = in->nHeight - 3;
			else sqy = (int)floor(origY) - 1;

			//теперь проводится интерполяция в пределах установленного квадрата

			new_pix = Interp(in, sqx, sqy, origX, origY);

			*out->getElementAddr(x, y) = new_pix;
		}
	}
}

unsigned char Interp(NPngProc::SImage* in,
	unsigned int sqx, unsigned int sqy,
	double origX, double origY)
{
	double pt[4];			//четыре полученных значения на одном столбце
	double res;				//полученное значение в нужной точке

	unsigned int i;



	for (i = sqx; i < sqx + 4; i++)
	{
		//проход по каждой строке, нахождение на каждой одного значения
		pt[i - sqx] = *in->getElementAddr(i, sqy) * K(origY - sqy ) + 
			*in->getElementAddr(i, sqy + 1) * K(origY - (sqy + 1)) +
			*in->getElementAddr(i, sqy + 2) * K(origY - (sqy + 2)) +
			*in->getElementAddr(i, sqy + 3) * K(origY - (sqy + 3));
	}

	res = pt[0] * K(origX - sqx) + 
		pt[1] * K(origX - (sqx + 1)) + 
		pt[2] * K(origX - (sqx + 2)) +
		pt[3] * K(origX - (sqx + 3));

	return (int)res;
}

double K(double sigma)
{
	sigma = abs(sigma);					//используется только модуль сигмы

	if (sigma >= 2.0)
		return 0;
	else if (sigma >= 1.0)
		return a*pow(sigma, 3) - 5 * a* pow(sigma, 2) + 8 * a * sigma - 4 * a;
	else
		return (a + 2) * pow(sigma, 3) - (a + 3) * pow(sigma, 2) + 1;
}