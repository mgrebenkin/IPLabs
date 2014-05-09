#include "PngProc.h"
#include "Noises.h"
#include <iostream>
#include <string>

void main()
{
	using namespace std;
	//получение файла

	string strInputFile;
	cout << "File: ";
	getline(cin, strInputFile);


	//получение имени нового файла

	std::string strOutputFile(strInputFile);
	int dotPos = strOutputFile.find_last_of('.');
	strOutputFile.insert(dotPos, "_proc");

	unsigned char* pInBytes;

	size_t nSizeIF;

	nSizeIF = NPngProc::readPngFile(strInputFile.c_str(), 0, 0, 0, 0);

	if ((nSizeIF == NPngProc::PNG_ERROR))
	{
		cout << "Error reading file. Abort." << endl;
		getchar();
		return;
	}

	pInBytes = new unsigned char[nSizeIF];

	

	if (!pInBytes)
	{
		cout << "Can not allocate memory. " << nSizeIF << " bytes required. Abort." << endl;
		getchar();
		return;
	}

	//CBitsPtrGuard PtrGrd(&pInBytes);
	size_t nWidth, nHeight;

	NPngProc::readPngFileGray(strInputFile.c_str(), pInBytes, &nWidth, &nHeight);


	NPngProc::SImage in(pInBytes, nWidth, nHeight, 8);

	int ch;

	cout << "1 for additive noise " << endl << "2 for pulse noise" << endl;
	cout << "Smth else to quit" << endl;
	cin >> ch;

	if (ch == 1)
	{
		double sigma, z0;
		cout << "Sigma: ";
		cin >> sigma;
		cout << "z0: ";
		cin >> z0;
		//добавление аддитивного шума
		AddNoise(in, z0, sigma);
	}
	else if (ch == 2)
	{
		double Ps;
		cout << "Ps: ";
		cin >> Ps;
		//добавление импульсного шума
		PulseNoise(in, Ps);
	}
	else return;


	//сохранение в файле
	if (NPngProc::writePngFile(strOutputFile.c_str(),
		in.pBits, in.nWidth, in.nHeight, 8)
		== NPngProc::PNG_ERROR)
	{
		std::cout << "Unable to write .png file." << std::endl;
		std::getchar();
		return;
	}

	getchar();
}