#include <stdlib.h>
#include <windows.h>
#include <io.h>
#include "Common/RandomAccessFile.h"
#include "Common/Defines.h"

//default constructor
RandomAccessFile::RandomAccessFile() 
: m_pFileObj(NULL)
, filePointer(0)
{
}

RandomAccessFile::~RandomAccessFile()
{
	close();
}

bool RandomAccessFile::openForRead(const char *fileName)
{
	int fileStatus = _access(fileName, 4);
	if(fileStatus == -1)
	{
		printf("RandomAccessFile:: %s File/Directory does not exist!!!", fileName);
		return false;
	}

	printf("%s - opened for Read!!!\n", fileName);
	m_sFileName = fileName;

	fopen_s(&m_pFileObj, m_sFileName.c_str(), "rb");//Read As BINARY
	filePointer = 0;

	return true;
}

bool RandomAccessFile::openForWrite(const char *fileName)
{
	if(_access(fileName, 0) == -1)
	{
		touch(fileName);
	}

	int fileStatus = _access(fileName, 2);
	if(fileStatus == -1)
	{
		printf("RandomAccessFile:: %s File/Directory does not exist!!!", fileName);
		return false;
	}

	m_sFileName = fileName;

	fopen_s(&m_pFileObj, m_sFileName.c_str(), "wb+");//Write as BINARY
	filePointer = 0;
	
	return true;
}

const char* RandomAccessFile::openForAppend(const char *fileName)
{
	int fileStatus = _access(fileName, 6);
	if(fileStatus == -1)
	{
		printf("RandomAccessFile:: %s File/Directory does not exist!!!", fileName);
		return "-1";
	}

	m_sFileName = fileName;

	fopen_s(&m_pFileObj, m_sFileName.c_str(), "a+");
	filePointer = getFileLength();

	return "1";
}

bool RandomAccessFile::openForRW(const char *fileName)
{
	int fileStatus = _access(fileName, 6);
	if(fileStatus == -1)
	{
		printf("RandomAccessFile::open --> File/Directory does not exist!!!\n");
		return false;
	}

	m_sFileName = fileName;

	fopen_s(&m_pFileObj, m_sFileName.c_str(), "rb+");//Read As BINARY
	filePointer = 0;

	return true;
}

int RandomAccessFile::touch(const char* fileName)
{
	int fileHandle = -1;
	fileHandle = _creat(fileName, _S_IREAD | _S_IWRITE);
	if(fileHandle == -1)
	{
		return -1;
	}

	printf("File created. %s\n", fileName);
	_close(fileHandle);
	return 1;
}

void RandomAccessFile::close()
{
	if(m_pFileObj != NULL) 
	{
		if(fclose(m_pFileObj) == 0) 
		{
			m_pFileObj = 0;
			printf("File close succesfull.\n");
		}
		else
			printf("Unable to close File.\n");
	}
}

const char* RandomAccessFile::readCharS() 
{
	if(m_pFileObj != NULL) 
	{
		unsigned char byte0;
		size_t iSize = sizeof(char) + 1;
		char* pBuffer = new char[iSize];
		memset(pBuffer, 0, iSize);
		pBuffer[sizeof(char)] = '\0';

		fseek(m_pFileObj, filePointer, SEEK_SET);

		byte0 = fgetc(m_pFileObj);
		filePointer += 1;

		sprintf_s(pBuffer, iSize, "%c", byte0);
		pBuffer[sizeof(char)] = '\0';

		return pBuffer;
	}
	
	return NULL;
}

char RandomAccessFile::readChar() 
{
	if(m_pFileObj != NULL) 
	{
		unsigned char byte0;
		
		fseek(m_pFileObj, filePointer, SEEK_SET);
		byte0 = fgetc(m_pFileObj);
		filePointer += 1;

		return byte0;
	}

	return NULL;
}

bool RandomAccessFile::readBoolean() 
{
	return (readByte()>0)?true:false;
}

const char* RandomAccessFile::readBooleanS() 
{
	return (readByte()>0)?"true":"false";
}

int RandomAccessFile::readByte() 
{
	char byte0 = -1;

	fseek(m_pFileObj, filePointer, SEEK_SET);

	if(m_pFileObj != NULL && !isEOF()) 
	{
		byte0 = getc(m_pFileObj);
		filePointer += 1;
	}

	return byte0&0xff;
}

int RandomAccessFile::readShort() 
{
	if(m_pFileObj != NULL) 
	{
		int int_16;

		fseek(m_pFileObj, filePointer, SEEK_SET);

		int_16 = readByte();
		int_16 |= (readByte()<<8);

		return int_16;
	}

	return NULL;
}

unsigned long RandomAccessFile::readInt() 
{
	if(m_pFileObj != NULL) 
	{
		unsigned long int_32;

		fseek(m_pFileObj, filePointer, SEEK_SET);

		int_32 = readShort();
		int_32 |= readShort()<<16;

		return int_32;
	}

	return NULL;
}

unsigned long long RandomAccessFile::readLong() 
{
	if(m_pFileObj != NULL) 
	{
		unsigned long long LSB, MSB, int_64;

		fseek(m_pFileObj, filePointer, SEEK_SET);

		LSB  = readInt();
		MSB  = readInt();
		int_64 = MSB<<32;
		int_64 |= LSB;

		return int_64;
	}

	return -1;
}

const char* RandomAccessFile::readByteS() 
{
	char* pBuffer = new char[sizeof(char)+1];
	memset(pBuffer, 0, sizeof(char)+1);
	pBuffer[sizeof(char)] = '\0';

	sprintf(pBuffer, "%d", readByte());
	pBuffer[sizeof(char)] = '\0';

	return pBuffer;
}

const char* RandomAccessFile::readShortS() 
{
	if(m_pFileObj != NULL) 
	{
		short LSB;
		char* pBuffer = new char[sizeof(short)+1];
		memset(pBuffer, 0, sizeof(short)+1);
		pBuffer[sizeof(short)] = '\0';

		fseek(m_pFileObj, filePointer, SEEK_SET);

		sprintf(pBuffer, "%d", readShort());
		pBuffer[sizeof(short)] = '\0';

		return pBuffer;
	}

	return NULL;

}

const char* RandomAccessFile::readIntS() 
{
	if(m_pFileObj != NULL) {
		char* pBuffer = new char[sizeof(int)+1];
		memset(pBuffer, 0, sizeof(int)+1);
		pBuffer[sizeof(int)] = '\0';

		fseek(m_pFileObj, filePointer, SEEK_SET);

		sprintf(pBuffer, "%lu", readInt());
		pBuffer[sizeof(int)] = '\0';

		return pBuffer;
	}

	return NULL;
}

const char* RandomAccessFile::readLongS() 
{
	if(m_pFileObj != NULL) 
	{
		char* pBuffer = new char[sizeof(long long)+1];
		memset(pBuffer, 0, sizeof(long long)+1);
		pBuffer[sizeof(long long)] = '\0';

		sprintf(pBuffer, "%llu", readLong());
		pBuffer[sizeof(long long)] = '\0';

		return pBuffer;
	}

	return NULL;
}

void RandomAccessFile::writeByte(const char* byte0) 
{
	if(m_pFileObj != NULL) 
	{
		writeByte(atoi(byte0));
	}
}

void RandomAccessFile::writeByte(int byte0) {
	if(m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, filePointer, SEEK_SET);
		fputc(byte0, m_pFileObj);
		
		filePointer += 1;
	}
}

void RandomAccessFile::writeShort(const char* shortValue) 
{
	if(m_pFileObj != NULL) 
	{
		writeShort(atoi(shortValue));
	}
}

void RandomAccessFile::writeShort(int shortValue) 
{
	if(m_pFileObj != NULL) 
	{
		writeByte(shortValue&0xff);
		writeByte((shortValue>>8)&0xff);
	}
}

void RandomAccessFile::writeBoolean(const char* isTrue) 
{
	if(m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, filePointer, SEEK_SET);

		int numValue = 0;
		if(_strcmpi(isTrue, "1") == 0) {
			numValue = 1;
		}
		
		fputc(numValue, m_pFileObj);
		filePointer += 1;
	}
}

void RandomAccessFile::writeBoolean(int isTrue) 
{
	writeByte(isTrue>0?1:0);
}

void RandomAccessFile::writeChar(const char* charValue) 
{
	if(m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, filePointer, SEEK_SET);
		
		char char0 = (char)charValue[0];
		
		fputc(char0, m_pFileObj);
		filePointer += 1;
	}
}

const char *RandomAccessFile::isEOFS() 
{
	if(m_pFileObj != NULL) 
	{
		if(!feof(m_pFileObj)) 
		{
			return "false";
		}
		else 
		{
			return "true";
		}	
	}

	return "true";
}

int RandomAccessFile::isEOF() 
{
	if(m_pFileObj != NULL) 
	{
		return feof(m_pFileObj)?1:0;
	}

	return 1;
}
/*
const char *RandomAccessFile::readLine() {
	if(isEOF())
		return "";

	int REALLOC = 1;
	int MAX_READ = 255;

	char* line = (char*)::malloc(MAX_READ);//GlobalAlloc(0, MAX_READ);
	::memset(line, 1, MAX_READ);

	int prevFilePointer = filePointer;

	int bytesRead = read(line);
	int cursor = 0, count = 0;
		
	//MessageBox(NULL, getStringValue(bytesRead), "OOOOOOOOOOOOOOOOOOOOOOOOO", MB_OK);

	while(true) {
		if(line[cursor] == '\r') {
			line[cursor++] = 0;
			
			filePointer = prevFilePointer + cursor;
			if(line[cursor] == '\n') {
				filePointer++;
			}
			
			fseek(m_pFileObj, filePointer, SEEK_SET);

			break;
		}
		else {
			if(	count >= bytesRead
				&&
				bytesRead != MAX_READ
			) {
				line[cursor++] = 0;
				filePointer = prevFilePointer + cursor;
				break;
			}
		}
		

		cursor++;
		count++;

		if(cursor%255 == 0) {
			REALLOC++;
//MessageBox(NULL, getStringValue(REALLOC*255), MyStrCat("REALLOC ", (char*)getStringValue(cursor)), MB_OK);
			line = new char[REALLOC*MAX_READ+1];//reallocMemory(line, REALLOC*MAX_READ, cursor, 'A');
			line[REALLOC*MAX_READ] = '\0';
			
			bytesRead = read(line, cursor, MAX_READ);
			count = 0;

			//if(isEOF())
			//	MB("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE");

//MessageBox(NULL, getStringValue(filePointer), "filePointer...................", MB_OK);
//MessageBox(NULL, getStringValue(bytesRead), "bytesRead...................", MB_OK);
//MessageBox(NULL, line, MyStrCat("linelinelinelineline ", (char*)getStringValue(bytesRead)), MB_OK);
		}
	}
//MessageBox(NULL, getStringValue(filePointer), "filePointer...................", MB_OK);
	return line;
}
//*/
const char *RandomAccessFile::readLine() 
{
	if (isEOF())
	{
		return "";
	}
	
	int prevFilePointer = filePointer;

	unsigned char c = readChar();
	while(!(c == '\r' || c == '\n') && !isEOF()) 
	{
		c = readChar();
	}
	
	int iLineLength = filePointer - prevFilePointer - 1;
	if(c == '\r') 
	{
		filePointer++;
	}

	if(iLineLength > 0) 
	{
		char* line = new char[sizeof(char) * iLineLength+1];
		::memset(line, 0, sizeof(char) * iLineLength+1);
		line[sizeof(char) * iLineLength] = '\0';

		fseek(m_pFileObj, prevFilePointer, SEEK_SET);
		
		int iRead = fread(line, sizeof(char), iLineLength, m_pFileObj);
		fseek(m_pFileObj, filePointer, SEEK_SET);

		line[sizeof(char) * iLineLength] = '\0';

		m_sLine = line;
		SAFE_DELETE_ARRAY(line);

		return m_sLine.c_str();
	}
	
	return "";
}

void RandomAccessFile::writeLine(const char *line) 
{
	if(m_pFileObj != NULL) 
	{
		writeChars(line);
		writeChars("\r");
		writeChars("\n");
	}
}

void RandomAccessFile::write(char* buf) 
{
	writeChars(buf);
}

void RandomAccessFile::write(char* buf, int offset, int len) 
{
	char* writeFrom = buf+offset;

	fseek(m_pFileObj, filePointer, SEEK_SET);
	
	fwrite(writeFrom, sizeof(char), len, m_pFileObj);
	filePointer += len;
}

void RandomAccessFile::writeChars(const char* buf) 
{
	if(m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, filePointer, SEEK_SET);
		
		fwrite(buf, sizeof(char), strlen(buf), m_pFileObj);
		filePointer += strlen(buf);
	}
}

void RandomAccessFile::writeBytes(const char* buf) 
{
	writeChars(buf);
}

void RandomAccessFile::writeIntS(const char* intValue) 
{
	if(m_pFileObj != NULL) 
	{
		int num = atol(intValue);

		writeInt(num);
	}
}

void RandomAccessFile::writeInt(unsigned long intValue) 
{
	if(m_pFileObj != NULL) 
	{
		writeShort(intValue&0xffff);
		writeShort((intValue>>16)&0xffff);
	}
}

void RandomAccessFile::writeLong(unsigned long long longValue) 
{
	if(m_pFileObj != NULL) 
	{
		writeInt(longValue&0xffffffff);
		writeInt((longValue>>32)&0xffffffff);
	}
}

void RandomAccessFile::writeLong(const char* longValue) 
{
	if(m_pFileObj != NULL) 
	{
		fseek(m_pFileObj, filePointer, SEEK_SET);
		unsigned long long num0 = atol(longValue);

		fputc((num0>>0)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num0>>8)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num0>>16)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num0>>24)&0xff, m_pFileObj);
		filePointer += 1;

		unsigned long long num1 = num0>>24;
		num1 >>= 8;

		fputc((num1>>0)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num1>>8)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num1>>16)&0xff, m_pFileObj);
		filePointer += 1;
		fputc((num1>>24)&0xff, m_pFileObj);
		filePointer += 1;
	}
}

int RandomAccessFile::read(char* buf) 
{
	return read(buf, 0, strlen(buf));
}

int RandomAccessFile::read(char* buf, int offset, int len)
{
	int count = 0;
	int bufLen = strlen(buf);
	
	int bytesToRead = len;
	
	if(offset+len > bufLen)
		bytesToRead = bufLen - offset;
	
	fseek(m_pFileObj, filePointer, SEEK_SET);

	while(!isEOF()) 
	{
		if(	len > 0 
			&&
			offset < bufLen
		) {
			int bytesRead = fread(buf + offset, sizeof(char), bytesToRead, m_pFileObj);
			filePointer += bytesRead;

			fseek(m_pFileObj, filePointer, SEEK_SET);
			return bytesRead;
		}
		else {
			return count;
		}
	}

	return count;
}

const char *RandomAccessFile::getFilePointer() 
{
	char* pBuffer = new char[sizeof(long long)+1];
	memset(pBuffer, 0, sizeof(long long)+1);
	pBuffer[sizeof(long long)] = '\0';

	_itoa(filePointer, pBuffer, 10);
	pBuffer[sizeof(long long)] = '\0';

	return pBuffer;
}

unsigned long long RandomAccessFile::getFilePointerLong() 
{
	return filePointer;
}

void RandomAccessFile::skipBytes(unsigned long long numOfBytesToSkip) 
{
	if(m_pFileObj != NULL) 
	{
		filePointer += numOfBytesToSkip;
		fseek(m_pFileObj, numOfBytesToSkip, SEEK_SET);
	}
}

void RandomAccessFile::seek(unsigned long long pos) 
{
	if(m_pFileObj != NULL) 
	{
		filePointer = pos;
		fseek(m_pFileObj, filePointer, SEEK_SET);
	}
}

long RandomAccessFile::getFileLength() 
{
	if(m_pFileObj!= NULL) 
	{
		int errNo = fseek(m_pFileObj, 0l, SEEK_END);
		if(errNo == 0) 
		{
			fpos_t pos_t;
			int err = fgetpos(m_pFileObj, &pos_t);
			
			return (long)pos_t;
		}
	}

	return -1;
}

unsigned long long	RandomAccessFile::length() 
{
	long int fileLength = 0;
	if(m_pFileObj != NULL) 
	{
		long lastPos = ftell(m_pFileObj);//save last position.

		fseek(m_pFileObj, 0l, SEEK_END);
		fileLength = ftell(m_pFileObj);

		fseek(m_pFileObj, lastPos, SEEK_SET);
	}
	
	return fileLength;
}

const char *RandomAccessFile::lengthS() 
{
	long int fileLength = length();

	char* pBuffer = new char[sizeof(fileLength)+1];
	memset(pBuffer, 0, sizeof(fileLength)+1);
	pBuffer[sizeof(fileLength)] = '\0';

	sprintf(pBuffer, "%ld", fileLength);
	pBuffer[sizeof(fileLength)] = '\0';

	return pBuffer;
}

int RandomAccessFile::getFD() 
{
	int fd = -1;
	if(m_pFileObj != NULL) 
	{
		fd = _fileno(m_pFileObj);
	}

	return fd;
}

const char *RandomAccessFile::getFileDescriptor() 
{
	char* pBuffer = new char[sizeof(int)+1];
	memset(pBuffer, 0, sizeof(int)+1);
	pBuffer[sizeof(int)] = '\0';
	
	if(m_pFileObj != NULL) 
	{
		sprintf(pBuffer, "%d", getFD());
		pBuffer[sizeof(int)] = '\0';
	}

	return pBuffer;
}

void RandomAccessFile::setLength(const char *newLength) 
{
	if(m_pFileObj != NULL) 
	{
		long newLen = atol(newLength);
		if(_chsize(getFD(), newLen) == 0) 
		{
			printf("File length successfully altered to %ld\n", newLen);
			
			filePointer = newLen;
			fseek(m_pFileObj, filePointer, SEEK_SET);
		}
	}
}

void RandomAccessFile::renameFile(const char *newFileName) 
{
	if(m_pFileObj != NULL) 
	{
	}
}

static void deleteFile(const char *fileToDelete) 
{
	int errVal = remove(fileToDelete); 
	if(errVal != 0) 
	{
		printf("Error deleting FILE %d\n", errVal);
	}
	else
		printf("FILE \"%s\" deleted sucessfully!!!\n", fileToDelete);
}