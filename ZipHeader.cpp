#include "ZipHeader.h"

ZipHeader::ZipHeader(FILE* &file)
{
	signature = readInt(file); 
	versionNeeded = readShort(file);
	bitFlag = readShort(file);
	compressionMethod = readShort(file);
	lastModifiedTime = readShort(file);
	lastModifiedDate = readShort(file);
	crc32 = readInt(file);
	compressedSize = readInt(file);
	uncompressedSize = readInt(file);

	nameLength = readShort(file);
	extraLength = readShort(file);

	fileName = readString(file, nameLength);
	extraField = readString(file, extraLength);
	dataOffset = ftell(file);
}

ZipHeader::ZipHeader(char* &buffer, int maxSize, int startOffset)
{
	char* temp = buffer + startOffset;
	maxSize -= startOffset;
	if(maxSize < 30)
	{
		reportError();
		return;
	}
	signature = readInt(temp); 
	versionNeeded = readShort(temp);
	bitFlag = readShort(temp);
	compressionMethod = readShort(temp);
	lastModifiedTime = readShort(temp);
	lastModifiedDate = readShort(temp);
	crc32 = readInt(temp);
	compressedSize = readInt(temp);
	uncompressedSize = readInt(temp);

	nameLength = readShort(temp);
	extraLength = readShort(temp);;
	if(maxSize < 30 + nameLength - extraLength)
	{
		reportError();
		return;
	}
	fileName = readString(temp, nameLength);
	extraField = readString(temp, extraLength);
	dataOffset = (int) (temp-buffer);
}

void ZipHeader::print()
{
	printf("Signature: %d\n", signature);
	printf("version needed: %d\n", versionNeeded);
	printf("bit flag: %d\n", bitFlag);
	printf("compression method: %d\n", compressionMethod);
	printf("last modified time: %d\n", lastModifiedTime);
	printf("last modified date: %d\n", lastModifiedDate);
	printf("CRC32: %d\n", crc32);
	printf("compressed size: %d\n", compressedSize);
	printf("uncompressed size: %d\n", uncompressedSize);
	double ratio = (double)uncompressedSize / compressedSize;
	printf("Ration: %lf\n", ratio);

	printf("name length: %d\n", nameLength);
	printf("extra length: %d\n", extraLength);

	cout << "file name: " << fileName << endl;
	cout << "extra field: " << extraField << endl;
	cout << "data offset: " << dataOffset << endl;
}

int ZipHeader::getCompressedSize()
{
	return compressedSize;
}

int ZipHeader::getUncompressedSize()
{
	return uncompressedSize;
}

int ZipHeader::getOffset()
{
	return dataOffset;
}

int ZipHeader::getHeaderFullSize()
{
	return 30+extraLength+nameLength+compressedSize;
}

string ZipHeader::getFileName()
{
	return fileName;
}

void ZipHeader::reportError()
{
	signature = -1;
	versionNeeded = -1;
	bitFlag = -1;
	compressionMethod = -1;
	lastModifiedTime = -1;
	lastModifiedDate = -1;
	crc32 = -1;
	compressedSize = -1;
	uncompressedSize = -1;

	nameLength = -1;
	extraLength = -1;
}

bool ZipHeader::hasError()
{
	return signature == -1 && versionNeeded == -1 && bitFlag == -1 && compressionMethod == -1 && lastModifiedTime == -1 && lastModifiedDate == -1 
			&& crc32 == -1 && compressedSize == -1 && uncompressedSize == -1 && nameLength == -1 && extraLength == -1;
}

vector<ZipHeader> ZipHeader::getAllHeaders(FILE* &file)
{
	int bytesRead = 0;
	fseek(file, 0L, SEEK_END);
	int totalBytes = ftell(file);
	rewind(file);
	vector<ZipHeader> toRet;
	while(!feof(file))
	{
		ZipHeader header(file);
		bytesRead += sizeof(header) + header.getCompressedSize();
		if(bytesRead > totalBytes)
			break;
		toRet.push_back(header);
		fseek(file, header.getCompressedSize(), SEEK_CUR);
	}
	return toRet;
}

vector<ZipHeader> ZipHeader::getAllHeaders(char* &buffer, int bufferSize)
{
	int offset = 0;
	vector<ZipHeader> toRet;
	while(offset < bufferSize)
	{
		ZipHeader header(buffer, bufferSize, offset);
		offset += header.getHeaderFullSize();
		if(offset < bufferSize)
		{
			toRet.push_back(header);
		}
	}
	return toRet;
}

int readInt(FILE* &file)
{
	char temp[5];
	memset(temp, 0, 5);
	if(fgets(temp, 5, file) == NULL)
		return -1;
	return *(int*)temp;
}

short readShort(FILE* &file)
{
	char temp[3];
	memset(temp, 0, 3);
	if(fgets(temp, 3, file) == NULL)
		return -1;
	return *(short*)temp;
}

string readString(FILE* &file, int length)
{
	if(length <= 0)
		return "";
	char *temp = new char[length+1];
	memset(temp, 0, length+1);
	string toRet;
	if(fgets(temp, length+1, file) == NULL)
	{
		toRet = "";
	}
	else
		toRet = string(temp, length);
	delete[] temp;
	return toRet;
}

int readInt(char* &buffer)
{
	int toRet = *(int*)buffer;
	buffer += 4;
	return toRet;
}

short readShort(char* &buffer)
{
	short toRet = *(short*)buffer;
	buffer += 2;
	return toRet;
}

string readString(char* &buffer, int length)
{
	if(length <= 0)
		return "";
	string toRet(buffer, length);
	buffer += length;
	return toRet;
}