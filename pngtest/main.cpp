
#include "PngProc.h"
#include <iostream>
#include <string>


void FloydSteinberg(NPngProc::SImage* pIn
			 , NPngProc::SImage* pOut);

#define _BPP 8
#define _BWTHRESH 0xA0

int main()
{
	

	std::string strInputFile;
	std::cout << "Input image file: ";	//запрос открываемого файла
	std::getline(std::cin, strInputFile);

	size_t nReqSize = NPngProc::readPngFile(strInputFile.c_str(), 0, 0, 0, 0);
	if (nReqSize == NPngProc::PNG_ERROR)
	{
		std::cout<<std::endl<<"Error ocured while png file was read."<<std::endl;
		std::getchar();
		return -1;
	}
	
		//получение имени нового файла

	std::string strOutputFile(strInputFile);
	int dotPos = strOutputFile.find_last_of('.');
	strOutputFile.insert(dotPos, "_proc");
	std::cout << "Name of output file is " << strOutputFile << std::endl;


	unsigned char* pInputBits = new unsigned char[nReqSize];
	if (!pInputBits)
	{
		std::cout << "Can't allocate memory for image, required size is "<< nReqSize<<std::endl;
		std::getchar();
		return -1;
	}
	CBitsPtrGuard InputBitsPtrGuard(&pInputBits);

	unsigned char* pOutputBits = new unsigned char[nReqSize];
	if (!pOutputBits)
	{
		std::cout << "Can't allocate memory for image, required size is " << nReqSize<<std::endl;
		std::getchar();
		return -1;
	}


	CBitsPtrGuard OutputBitsPtrGuard(&pOutputBits);
	size_t nWidth, nHeight;
	unsigned int nBPP;

	size_t nRetSize = NPngProc::readPngFileGray(strInputFile.c_str(), 
							pInputBits, &nWidth, &nHeight/*, &nBPP*/);
	nBPP = 8;


	NPngProc::SImage imageIn(pInputBits, nWidth, nHeight, _BPP);
	NPngProc::SImage imageOut(pOutputBits, nWidth, nHeight, _BPP);

	// ASSERT(nRetSize == nReqSize);

	FloydSteinberg(&imageIn, &imageOut); 
	if (NPngProc::writePngFile(strOutputFile.c_str(),
	imageOut.pBits, imageOut.nWidth, imageOut.nHeight, nBPP) == NPngProc::PNG_ERROR)
	{
		std::cout << "Error occured during png file was written."<<std::endl;
		std::getchar();
		return -1;
	}

std::getchar();
return 0;
}

void FloydSteinberg(NPngProc::SImage* pIn
			 , NPngProc::SImage* pOut)
{
	//ASSERT(pOut != NULL && pIn != NULL && nWidth > 0 && nHeight > 0);

	//NPngProc::SImage imageIn(pIn, nWidth, nHeight, _BPP);
	//NPngProc::SImage imageOut(pOut, nWidth, nHeight, _BPP);
	
	unsigned char NewPix, error;

	for (unsigned int i = 2; i <= pIn->nHeight - 1; i++)		//перебор всех пикселов
	{															//сверху вниз, слева направо
		for (unsigned int j = 2; j <= pIn->nWidth - 1; j++)		//кроме крайних
		{
			if (*pIn->getElementAddr( i, j ) <= _BWTHRESH)	//определение какой из двух цветов
				NewPix = 0;								//присвоить пикселу
			else
				NewPix = 0xFF;

			error = abs(*pIn->getElementAddr( i, j ) - NewPix);						//определение ошибки
			*pOut->getElementAddr( i, j ) = NewPix;									//присвоение нового значения
			
			//рассеивание ошибки
			*pIn->getElementAddr(i, j + 1) +=		static_cast<int>(7.0 / 16)*error;
			*pIn->getElementAddr(i + 1, j - 1) +=	static_cast<int>(3.0 / 16)*error;
			*pIn->getElementAddr(i + 1, j) +=		static_cast<int>(5.0 / 16)*error;
			*pIn->getElementAddr(i + 1, j + 1) +=	static_cast<int>(1.0 / 16)*error;

		}

		
	}
	
	return;
}
