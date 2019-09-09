// zxqFileUtils.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

void readBMP(const char* fileName);

int main()
{
	readBMP("123");
}

void readBMP(const char* fileName) {
	std::ifstream file("C:/Users/xueqianz/Desktop/workspace/CppProjects/bmp_files/24_bit_color.bmp", std::ios::binary);
	typedef struct zBMPHEADER {
		unsigned short bfType;
		unsigned short bfSizeLow;
		unsigned short bfSizeHigh;
		unsigned short bfReserved1;
		unsigned short bfReserved2;
		unsigned short bfOffsetBitsLow;
		unsigned short bfOffsetBitsHigh;
	}BMPHEADER;
	typedef struct zBMPINFO {
		unsigned int biSize;
		unsigned int biWidth;
		int biHeight;
		unsigned int biBitInfo;
		unsigned int biCompression;
		unsigned int biSizeImage;
		int biXPixels;
		int biYPixels;
		unsigned int biClrUsed;
		unsigned int biClrImportant;
	} BMPINFO;
	BMPHEADER header;
	BMPINFO info;
	file.read((char*)&header, sizeof(BMPHEADER));
	file.read((char*)&info, sizeof(BMPINFO));
	unsigned int fileSize = 0;
	unsigned int offsetBits = 0;
	unsigned short planes = info.biBitInfo & 0x00ff;
	unsigned short bitCount = info.biBitInfo >> 16;
	fileSize |= header.bfSizeHigh;
	fileSize = (fileSize << 16) | header.bfSizeLow;
	offsetBits |= header.bfOffsetBitsHigh;
	offsetBits = (offsetBits << 16) | header.bfOffsetBitsLow;
	std::cout  << info.biSize << " " << info.biClrImportant;
}

const char* readFile(const char* fileName) {
	std::ifstream file(fileName);
	char* content;
	content = (char*)malloc(sizeof(char) * 40960);
	if (content)
	{
		char buff[1024];
		int pt = 0;
		while (file.getline(buff, 1024))
		{
			for (int i = 0; i < 1024; i++) {
				char tmp = buff[i];
				if (tmp == '\0') {
					content[pt] = '\n';
					pt++;
					break;
				}
				content[pt] = tmp;
				pt++;
			}
		}
		content[pt] = '\0';
		char* result = (char*)malloc(sizeof(char) * (++pt));
		if (!result)
			return NULL;
		for (int i = 0; i < pt; i++) {
			result[i] = content[i];
		}
		return result;
	}
	return NULL;
}
