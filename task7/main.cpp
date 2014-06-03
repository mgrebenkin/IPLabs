#include <iostream>
#include <string>
#include "conio.h"
#include "PngProc.h"
#include "stuff.h"


void main()
{
	std::string strInputFile;
	std::cout << "Input image file: ";	//запрос открываемого файла
	std::getline(std::cin, strInputFile);

	size_t nReqSize = NPngProc::readPngFile(strInputFile.c_str(), 0, 0, 0, 0);
	if (nReqSize == NPngProc::PNG_ERROR)
	{
		std::cout << std::endl << "Error ocured while png file was read." << std::endl;
		std::getchar();
		return;
	}

	

	unsigned char* pInputBits = new unsigned char[nReqSize];
	if (!pInputBits)
	{
		std::cout << "Can't allocate memory for image, required size is " << nReqSize << std::endl;
		std::getchar();
		return;
	}

	size_t nWidth, nHeight;
	unsigned int nBPP;

	size_t nRetSize = NPngProc::readPngFileGray(strInputFile.c_str(),
		pInputBits, &nWidth, &nHeight/*, &nBPP*/);
	nBPP = 8;


	NPngProc::SImage in(pInputBits, nWidth, nHeight, 8);

	int thresh;

	std::cout << "Threshold: ";
	std::cin >> thresh;

	ToBin(in, thresh);

	int amount = FindAreas(in);

	std::cout << "Areas: " << amount <<std::endl;

	//получение имени нового файла

	std::string strOutputFile(strInputFile);
	int dotPos = strOutputFile.find_last_of('.');
	strOutputFile.insert(dotPos, "_proc");



	if (NPngProc::writePngFile(strOutputFile.c_str(),
		in.pBits, in.nWidth, in.nHeight, nBPP) == NPngProc::PNG_ERROR)
	{
		std::cout << "Error occured during png file was written." << std::endl;
		std::getchar();
		return;
	}

	std::cout << "Name of output file is " << strOutputFile << std::endl;
	std:_getch();
	return;

}