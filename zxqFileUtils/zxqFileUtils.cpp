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
	unsigned char JPEG_STAGE = 0x00;

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
		unsigned short point;
		unsigned char* app[15];
	} JPEGAPPnINFO;

	typedef struct zJPEGDQT {
		unsigned char dqtFlag;
		unsigned char* dqtData;
	} JPEGDQT;

	typedef struct zJPEGSOF0{
		unsigned char accuracy;
		unsigned char heightHighByte;
		unsigned char heightLowByte;
		unsigned char widthHighByte;
		unsigned char widthLowByte;
		unsigned char colorWeight;
		unsigned char* colorWeightInfo;
	} JPEGSOF0;

	typedef struct zJPEGDHT {
		unsigned char dhtFlag;
		unsigned char dqtDataCount[16];
		unsigned char* dqtData;
	} JPEGDHT;

	typedef struct zJPEGSOS
	{
		unsigned char colorWeight;
		unsigned char* colorWeightInfo;
		unsigned char compressedData[3];
	} JPEGSOS;

	typedef struct zJPEGDATA
	{
		unsigned short height;
		unsigned short width;
		unsigned char* jpegData;
	} JPEGDATA;

	typedef struct zJPEGMCUINFO {
		unsigned char vMax = 0;
		unsigned char hMax = 0;
		unsigned char mcuWidth = 0;
		unsigned char mcuHeight = 0;
	} JPEGMCUINFO;
	
	JPEGDQT dqt[4];
	JPEGAPP0INFO app0;
	JPEGSOF0 sof0;
	JPEGDHT dht[4];
	JPEGSOS sos;
	JPEGDATA jpeg;
	JPEGMCUINFO mcu;

	bool isFlag = false;
	unsigned char item;
	while (!file.eof()) {
		file.read((char*)&item, sizeof(unsigned char));
		if (item == 0xff) {
			isFlag = true;
			continue;
		}
		if (isFlag) {
			JPEG_STAGE = item;
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
				dqtSize = u_shortRevert(dqtSize) - 2;
				while (dqtSize > 0) {
					unsigned char flag;
					file.read((char*)&flag, sizeof(char));
					unsigned char _id = flag & 0xf;
					unsigned char accuracy = flag >> 4 & 0xf;
					unsigned char length = (accuracy + 1) << 6;
					dqt[_id].dqtData = (unsigned char*)malloc(sizeof(char)*(length));
					dqt[_id].dqtFlag = flag;
					file.read((char*)dqt[_id].dqtData, sizeof(char)*(length));
					for(int j=0; j< length; j++)
						std::cout << std::hex << int(dqt[_id].dqtData[j]) << " ";
					std::cout << std::endl;
					dqtSize -= (length+1);
				}
				break;
			case JPEG_SOF0:
				std::cout << "SOF0" << std::endl;
				unsigned short sofSize;
				file.read((char*)&sofSize, sizeof(unsigned short));
				sofSize = u_shortRevert(sofSize) - 2;
				sof0.colorWeightInfo = (unsigned char*)malloc(sizeof(char)*(sofSize - 6));
				file.read((char*)&sof0, sizeof(char) * 6);
				file.read((char*)sof0.colorWeightInfo, sizeof(char)*(sofSize - 6));
				unsigned short l_height;
				l_height = ((sof0.widthHighByte << 8) & 0xff00) | (sof0.widthLowByte & 0xff);
				jpeg.width = ((sof0.widthHighByte << 8) & 0xff00) | (sof0.widthLowByte & 0xff);
				jpeg.height = ((sof0.heightHighByte << 8) & 0xff00) | (sof0.heightLowByte & 0xff);
				for (int j = 0; j < sofSize - 6; j++)
					std::cout << std::hex << int(sof0.colorWeightInfo[j]) << " ";
				if (sof0.colorWeight * 3 != (sofSize - 6)) {
					std::cerr << "Wrong SOF header of jpeg file." << std::endl;
				}
				for (int j = 0; j < sof0.colorWeight * 3; j += 3) {
					unsigned char colorId, factor, tableID;
					unsigned char h_factor, v_factor;
					colorId = sof0.colorWeightInfo[j];
					factor = sof0.colorWeightInfo[j + 1];
					v_factor = factor & 0xf;
					h_factor = (factor >> 4) & 0xf;
					tableID = sof0.colorWeightInfo[j + 2];
					if (h_factor > mcu.hMax) {
						mcu.hMax = h_factor;
					}
					if (v_factor > mcu.vMax) {
						mcu.vMax = v_factor;
					}
				}
				mcu.mcuHeight = mcu.hMax << 2;
				mcu.mcuWidth = mcu.vMax << 2;
				std::cout << std::endl;
				break;
			case JPEG_DHT:
				std::cout << "DHT" << std::endl;
				unsigned short dhtSize;
				file.read((char*)&dhtSize, sizeof(unsigned short));
				dhtSize = u_shortRevert(dhtSize) - 2;
				while (dhtSize > 0)
				{
					unsigned char flag;
					file.read((char*)&flag, sizeof(char));
					unsigned char _id = flag & 0xf;
					unsigned char type = flag >> 4 & 0xf;
					file.read((char*)dht[_id].dqtDataCount, sizeof(char) * 16);
					dqt[_id].dqtFlag = flag;
					unsigned short length = 0;
					for (int j = 0; j < 16; j++) {
						length += dht[_id].dqtDataCount[j];
					}
					dqt[_id].dqtData = (unsigned char*)malloc(sizeof(char)*length);
					file.read((char*)dqt[_id].dqtData, sizeof(char)*length);
					dhtSize -= (length + 16 + 1);
					for (int j = 0; j < length; j++) {
						std::cout << std::hex << int(dqt[_id].dqtData[j]) << " ";
					}
					std::cout << std::endl;
				}
				break;
			case JPEG_DRI:
				std::cout << "DRI" << std::endl;
				break;
			case JPEG_SOS:
				std::cout << "SOS" << std::endl;
				unsigned short sosSize;
				file.read((char*)&sosSize, sizeof(unsigned short));
				sosSize = u_shortRevert(sosSize) - 2;
				unsigned char l_colorWeight;
				file.read((char*)&l_colorWeight, sizeof(char));
				sos.colorWeight = l_colorWeight;
				sos.colorWeightInfo = (unsigned char*)malloc(sizeof(char)*(l_colorWeight * 2));
				file.read((char*)sos.colorWeightInfo, sizeof(char)*(l_colorWeight * 2));
				file.read((char*)sos.compressedData, sizeof(char) * 3);
				for (int j = 0; j < l_colorWeight * 2; j++) {
					std::cout << std::hex << int(sos.colorWeightInfo[j]) << " ";
				}
				std::cout << std::endl;
				break;
			case JPEG_EOI:
				std::cout << "EOI" << std::endl;
				std::cout << jpeg.height << " " << jpeg.width << std::endl;
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
		else {
			switch (JPEG_STAGE)
			{
			case JPEG_SOS:

				break;
			default:
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
