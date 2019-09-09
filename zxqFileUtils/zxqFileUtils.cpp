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

void readJPEG() {
	std::ifstream file("C:/Users/xueqianz/Desktop/workspace/CppProjects/bmp_files/24_bit_color.jpg", std::ios::binary);
	const char JPEG_SOI = 0xd8;
	const char JPEG_APP0 = 0xe0;
	const char JPEG_DQT = 0xdb;
	const char JPEG_SOF0 = 0xc0;
	const char JPEG_DHT = 0xc4;
	const char JPEG_DRI = 0xdd;
	const char JPEG_SOS = 0xda;
	const char JPEG_EOI = 0xd9;
	typedef struct zJPEGAPP0INFO {
		unsigned short jaSize;
		unsigned short flagBits1;
		unsigned short flagBits2;
		unsigned short restFlagBits_versionLowBits;
		unsigned short versionHighBits_density;
		unsigned short xDensity;
		unsigned short yDensity;
		unsigned short thumbDensity;
	} JPEGAPP0INFO;
	typedef struct zJPEGAPPnINFO {
		unsigned short jaSize;
	} JPEGAPPnINFO;

	bool isFlag = false;
	char item;
	while (!file.eof()) {
		file.read(&item, sizeof(char));
		if (item == 0xff) {
			isFlag = true;
			continue;
		}
		if (isFlag) {
			switch (item)
			{
			case JPEG_SOI:
				break;
			case JPEG_DQT:
				break;
			case JPEG_SOF0:
				break;
			case JPEG_DHT:
				break;
			case JPEG_DRI:
				break;
			case JPEG_SOS:
				break;
			case JPEG_EOI:
				break;
			default:
				if (item > 0xe0 && item <= 0xef) {

				}
				else {

				}
				break;
			}
		}
	}
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
