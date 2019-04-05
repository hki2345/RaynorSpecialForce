#include "WriteStream.h"


WriteStream::WriteStream(const wchar_t* _Path) : BaseStream(_Path, L"wb")
{
}


WriteStream::~WriteStream()
{
}

void WriteStream::Write(void* _WBuf, int _ByteSize)
{
	BaseStream::Write(_WBuf, _ByteSize);
}

void WriteStream::Write(const void* _WBuf, int _ByteSize)
{
	BaseStream::Write(_WBuf, _ByteSize);
}