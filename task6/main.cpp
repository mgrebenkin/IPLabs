#include <iostream>
#include <string>
#include "PngProc.h"
#include "Median.h"

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

	unsigned char* pOutputBits = new unsigned char[nReqSize];
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
	NPngProc::SImage out(pOutputBits, nWidth, nHeight, 8);

	NPngProc::SImage apt(0, 0, 0, 8);

	MakeApt(apt);

	Median(in, out, apt);

	//получение имени нового файла

	std::string strOutputFile(strInputFile);
	int dotPos = strOutputFile.find_last_of('.');
	strOutputFile.insert(dotPos, "_proc");
	std::cout << "Name of output file is " << strOutputFile << std::endl;

	if (NPngProc::writePngFile(strOutputFile.c_str(),
		out.pBits, out.nWidth, out.nHeight, nBPP) == NPngProc::PNG_ERROR)
	{
		std::cout << "Error occured during png file was written." << std::endl;
		std::getchar();
		return;
	}

	std::getchar();
	return;
}
