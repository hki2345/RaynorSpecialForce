#pragma once
#include "BaseStream.h"
class WriteStream : public BaseStream
{
public:
	template<typename T>
	void Write(T& _WBuf) 
	{
		BaseStream::Write((void*)&_WBuf, sizeof(T));
	}

	void Write(void* _WBuf, int _ByteSize);
	void Write(const void* _WBuf, int _ByteSize);

public:
	WriteStream(const wchar_t* _Path);
	~WriteStream();
};

