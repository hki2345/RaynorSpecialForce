#pragma once
#include "BaseStream.h"
class ReadStream : public BaseStream
{
public:
	template<typename T>
	void Read(T& _RBuf)
	{
		Read(&_RBuf, sizeof(T), sizeof(T));
	}
	void Read(void* _RBuf, int _BufByteSize);
	void Read(void* _RBuf, int _BufByteSize, int _ReadByteSize);
	void Seek(long _Size, int _Mode = SEEK_SET);

public:
	ReadStream(const wchar_t* _Path);
	~ReadStream();
};

