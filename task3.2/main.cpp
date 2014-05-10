#include "MixImages.h"
#include "PngProc.h"
#include <iostream>
#include <string>

void main()
{
	using namespace std;
	//получение трех файлов

	string strInputFile1, strInputFile2, strInputFileAlpa;
	string strOutputFile = "task3_proc.png";
	
	cout << "File A: ";
	getline(cin, strInputFile1);
	cout << "File B: ";
	getline(cin, strInputFile2);
	cout << "File Alpha: ";
	getline(cin, strInputFileAlpa);


	unsigned char* pInBytes1, * pInBytes2, * pInBytesAlpha;

	size_t nSizeIF1, nSizeIF2, nSizeIFA;

	nSizeIF1 = NPngProc::readPngFile(strInputFile1.c_str(), 0, 0, 0, 0);
	nSizeIF2 = NPngProc::readPngFile(strInputFile2.c_str(), 0, 0, 0, 0);
	nSizeIFA = NPngProc::readPngFile(strInputFileAlpa.c_str(), 0, 0, 0, 0);

	if ((nSizeIF1 == NPngProc::PNG_ERROR) ||
		(nSizeIF2 == NPngProc::PNG_ERROR) ||
		(nSizeIFA == NPngProc::PNG_ERROR))
	{
		cout << "Error reading file. Abort." << endl;
		getchar();
		return;
	}

	pInBytes1 = new unsigned char[nSizeIF1];
	pInBytes2 = new unsigned char[nSizeIF2];
	pInBytesAlpha = new unsigned char[nSizeIFA];

	if (!(pInBytes1 && pInBytes2 &&pInBytesAlpha))
	{
		cout << "Can not allocate memory. " << nSizeIF1 << " bytes required. Abort." << endl;
		getchar();
		return;
	}
	
	CBitsPtrGuard PtrGrd1(&pInBytes1), PtrGrd2(&pInBytes2), PtrGrd(&pInBytesAlpha);

	size_t nWidth1, nHeight1, nWidth2, nHeight2, nWidthA, nHeightA;
	size_t nWidth, nHeight;

	NPngProc::readPngFileGray(strInputFile1.c_str(), pInBytes1, &nWidth1, &nHeight1);
	NPngProc::readPngFileGray(strInputFile2.c_str(), pInBytes2, &nWidth2, &nHeight2);
	NPngProc::readPngFileGray(strInputFileAlpa.c_str(), pInBytesAlpha, &nWidthA, &nHeightA);

	if (!((nWidth1 == nWidth2) && (nWidth2 == nWidthA) &&
		(nHeight1 == nHeight2) && (nHeight2 == nHeightA)))
	{
		cout << "Sizes of images must match. Abort." << endl;
		getchar();
		return;
	}

	nWidth = nWidth1;
	nHeight = nHeight1;

	NPngProc::SImage in1(pInBytes1, nWidth, nHeight, 8);
	NPngProc::SImage in2(pInBytes2, nWidth, nHeight, 8);
	NPngProc::SImage inAlpha(pInBytesAlpha, nWidth, nHeight, 8);

	unsigned char* pOutBytes = new unsigned char[nSizeIF1];

	if (!pOutBytes)
	{
		cout << "Can not allocate memory. " << nSizeIF1 << " bytes required. Abort." << endl;
		getchar();
		return;
	}

	CBitsPtrGuard PtrGrdOut(&pOutBytes);

	NPngProc::SImage out(pOutBytes, nWidth, nHeight, 8);

	//наложение изображений
	MixImages(in1, in2, inAlpha, out);

	//сохранение в файле
	if (NPngProc::writePngFile(strOutputFile.c_str(),
		out.pBits, out.nWidth, out.nHeight, 8)
		== NPngProc::PNG_ERROR)
	{
		std::cout << "Unable to write .png file." << std::endl;
		std::getchar();
		return;
	}

	cout << "Done. Name of the output file is " << strOutputFile << endl;

	getchar();
}