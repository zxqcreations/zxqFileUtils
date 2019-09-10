// zxqFileUtils.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

void readBMP(const char* fileName);
void readJPEG();

int main()
{
	readJPEG();
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

unsigned short u_shortRevert(unsigned short num) {
	return ((num & 0xff) << 8) | ((num & 0xff00) >> 8);
}

void readJPEG() {
	std::ifstream file("C:/Users/xueqianz/Desktop/workspace/CppProjects/bmp_files/24_bit_color.jpg", std::ios::binary);

	const unsigned char JPEG_SOI = 0xd8;
	const unsigned char JPEG_APP0 = 0xe0;
	const unsigned char JPEG_APP1 = 0xe1;
	const unsigned char JPEG_APP15 = 0xef;
	const unsigned char JPEG_DQT = 0xdb;
	const unsigned char JPEG_SOF0 = 0xc0;
	const unsigned char JPEG_DHT = 0xc4;
	const unsigned char JPEG_DRI = 0xdd;
	const unsigned char JPEG_SOS = 0xda;
	const unsigned char JPEG_EOI = 0xd9;

	typedef struct zJPEGAPP0INFO {
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

	JPEGAPP0INFO app0;
	unsigned char* dqt;

	bool isFlag = false;
	unsigned char item;
	while (!file.eof()) {
		file.read((char*)&item, sizeof(unsigned char));
		if (item == 0xff) {
			isFlag = true;
			continue;
		}
		if (isFlag) {
			switch (item)
			{
			case JPEG_SOI:
				std::cout << "SOI" << std::endl;
				break;
			case JPEG_APP0:
				std::cout << "APP0" << std::endl;
				unsigned short app0Size;
				file.read((char*)&app0Size, sizeof(unsigned short));
				app0Size = u_shortRevert(app0Size);
				file.read((char*)&app0, sizeof(JPEGAPP0INFO));
				std::cout << std::hex << app0.flagBits1 << app0.flagBits2 << std::endl;
				break;
			case JPEG_DQT:
				std::cout << "DQT" << std::endl;
				unsigned short dqtSize;
				file.read((char*)&dqtSize, sizeof(unsigned short));
				dqtSize = u_shortRevert(dqtSize);
				dqt = (unsigned char*)malloc(sizeof(char)*(dqtSize - 2));
				file.read((char*)dqt, sizeof(char)*(dqtSize - 2));
				std::cout << std::hex << dqt[0] << std::endl;
				break;
			case JPEG_SOF0:
				std::cout << "SOF0" << std::endl;
				break;
			case JPEG_DHT:
				std::cout << "DHT" << std::endl;
				break;
			case JPEG_DRI:
				std::cout << "DRI" << std::endl;
				break;
			case JPEG_SOS:
				std::cout << "SOS" << std::endl;
				break;
			case JPEG_EOI:
				std::cout << "EOI" << std::endl;
				break;
			default:
				if (item >= JPEG_APP1 && item <= JPEG_APP15) {
					std::cout << "APP" << item << std::endl;
				}
				else if(item == 0x00) {

				}
				else {

				}
				break;
			}
		}

		isFlag = false;
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
