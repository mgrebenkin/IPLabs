#ifndef PNG_PROC_H
#define PNG_PROC_H

#define _CRT_SECURE_NO_WARNINGS

class CBitsPtrGuard
{
public:
	CBitsPtrGuard(unsigned char** pB) : m_ppBits(pB) { }
	~CBitsPtrGuard() { if (*m_ppBits) delete *m_ppBits, *m_ppBits = 0; }
protected:
	unsigned char** m_ppBits;
};


namespace NPngProc
{



	const size_t PNG_ERROR = (size_t)-1;

	struct SImage
	{
		SImage () : pBits(0), nWidth(0), nHeight(0), nBPP(0) { }
		
		
		SImage(unsigned char* _pBits, 
			size_t _nWidth, size_t _nHeight, 
			unsigned long _nBPP);
		
		
		~SImage() { release(); }

		void	release()
		{
			if (pBits)
				delete [] pBits, pBits = 0;
			nWidth = nHeight = nBPP = 0;
		}

		unsigned char* operator ()(size_t x, size_t y);

	public:
		unsigned char*	pBits;
		size_t		nWidth;
		size_t		nHeight;
		unsigned long	nBPP;

		unsigned char* getElementAddr(size_t x, size_t y);
	};


	size_t readPngFile(const char* szFileName
						, unsigned char* pBuf
						, size_t*	pWidth
						, size_t*	pHeight
						, unsigned int* pBPP
						);

	size_t readPngFileGray(const char* szFileName
		, unsigned char* pBuf
		, size_t*	pWidth
		, size_t*	pHeight
		);

	size_t writePngFile(const char* szFileName
						, unsigned char* pBuf
						, size_t	nWidth
						, size_t	nHeight
						, unsigned int nBPP
						);

};

#endif /* PNG_PROC_H */