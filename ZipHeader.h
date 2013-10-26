
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#pragma once
class ZipHeader
{
private:
	int signature, crc32, compressedSize, uncompressedSize, dataOffset;
	short versionNeeded, bitFlag, compressionMethod, lastModifiedTime, lastModifiedDate, nameLength, extraLength;
	string fileName, extraField;
	void reportError();
public:
	ZipHeader(FILE* &file);
	ZipHeader(char* &buffer, int maxSize, int startOffset = 0);
	void print();

	int getCompressedSize();
	int getUncompressedSize();
	int getHeaderFullSize();
	string getFileName();
	int getOffset();
	bool hasError();
	static vector<ZipHeader> getAllHeaders(FILE* &file); // Reads headers from the actual file
	static vector<ZipHeader> getAllHeaders(char* &buffer, int bufferSize); // Reads headers from allocated file
};

int readInt(FILE* &file);
short readShort(FILE* &file);
string readString(FILE* &file, int length);

int readInt(char* &buffer);
short readShort(char* &buffer);
string readString(char* &buffer, int length);