#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#pragma once
class ZipHeader
{
private:
	int signature, crc32, compressedSize, uncompressedSize;
	short versionNeeded, bitFlag, compressionMethod, lastModifiedTime, lastModifiedDate, nameLength, extraLength;
	string fileName, extraField;
public:
	ZipHeader(FILE* &file);
	void print();
	int getCompressedSize();
	static vector<ZipHeader> getAllHeaders(FILE* &file);
};

int readInt(FILE* &file);
short readShort(FILE* &file);
string readString(FILE* &file, int length);