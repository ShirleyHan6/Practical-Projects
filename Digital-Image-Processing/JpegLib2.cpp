#include <stdio.h>
#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
using namespace std;
#pragma pack(2)

typedef struct {
	char signature[2];
	unsigned int fileSize;
	unsigned int reserved;
	unsigned int offset;
}BmpHeader;

typedef struct {
	unsigned int headerSiz;
	unsigned int width;
	unsigned int height;
	unsigned short planeCount;
	unsigned short bitDepth;
	unsigned int compression;
	unsigned int compressedImageSize;
	unsigned int horizontalResolution;
	unsigned int verticalResolution;
	unsigned int numColors;
	unsigned int importantColors;
}BmpImageInfo;

typedef struct {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
}Rgb;

int main()
{
	FILE *inFile;
	BmpHeader header;
	BmpImageInfo info;
	Rgb *palette;
	int i = 0;
	char bmpFile[] = "C:\\1.bmp";
	int key;
	
	printf("Opening file for reading.\n");
	errno_t err;
	errno_t err1;
	err = fopen_s(&inFile, bmpFile, "rb");
	if (err!=0) {
		printf("Error opening file %s.\n", bmpFile);
		return -1;
	}

	if (fread(&header, 1, sizeof(BmpHeader), inFile) != sizeof(BmpHeader)) {
		printf("Error reading bmp header.\n");
		return -1;
	}
	if (fread(&info, 1, sizeof(BmpImageInfo), inFile) != sizeof(BmpImageInfo)) {
		printf("Error reading image info.\n");
		return -1;
	}

	cin >> key;
	
	cin >> key;

	printf("Opening file %s for writing .\n", "RGB.txt");
	FILE *outFile;
	err1 = fopen_s(&outFile,"C:\\Users\\Shirley\\Desktop\\JpegLib2\\RGB.txt", "wb");
	if (err1!=0) {
		printf("Error opening outputfile.\n");
		cin >> key;
		return -1;
	}

	Rgb *pixel = (Rgb*)malloc (sizeof(Rgb));
	int read, j;
	
	for (j = 0; j < info.height; j++) {
		printf("------ Row %d\n", j + 1);
		read = 0;
		for (i = 0; i < info.width; i++) {
			if (fread(pixel, 1, sizeof(Rgb), inFile) != sizeof(Rgb)) {
				printf("Error reading pixel!\n");
				return -1;
			}
			read += sizeof(Rgb);
			int buffer[] = { pixel->red, pixel->green, pixel->blue };
			printf("Pixel %d: %3d %3d %3d\n", i + 1, pixel->red, pixel->green, pixel->blue);
			//fwrite(buffer, sizeof(int), sizeof(buffer), outFile);
			fprintf(outFile,"%d  %d  %d \r\n", pixel->red,pixel->green, pixel->blue);
		}
		if (read % 4 != 0) {
			read = 4 - (read % 4);
			printf("Padding: %d bytes\n", read);
			fread(pixel, read, 1, inFile);
		}
	}

	printf("Done.\n");
	fclose(inFile);
	fclose(outFile);

	printf("\nBMP-Info:\n");
	printf("Width x Height: %i x %i\n", info.width, info.height);
	printf("Depth: %i\n", (int)info.bitDepth);
	
	cin >> key;

	return 0;
}