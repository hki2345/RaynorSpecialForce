#include "ReadStream.h"


ReadStream::ReadStream(const wchar_t* _Path) : BaseStream(_Path, L"rb")
{
}


ReadStream::~ReadStream()
{
}

void ReadStream::Read(void* _RBuf, int _BufByteSize) {
	Read(_RBuf, _BufByteSize, _BufByteSize);
}

void ReadStream::Read(void* _RBuf, int _BufByteSize, int _ReadByteSize)
{
	BaseStream::Read(_RBuf, _BufByteSize, _ReadByteSize);
}

void ReadStream::Seek(long _Size, int _Mode ) {
	BaseStream::Seek(_Size, _Mode);
}