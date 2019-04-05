#pragma once
#include <string>
#include "Begin.h"

class KResource : public Begin
{

public:
	KResource();
	KResource(const KResource& _Other);
	~KResource();

private:
	std::wstring m_AllPath;

	std::wstring m_FileForder;
	std::wstring m_FileNameExi;
	std::wstring m_FileName;
	std::wstring m_FileExt;

public:
	const wchar_t* AllPath() 
	{
		return m_AllPath.c_str();
	}
	const wchar_t* FileName() 
	{
		return m_FileName.c_str(); 
	}
	const wchar_t* FileNameExt() 
	{
		return m_FileNameExi.c_str();
	}
	const wchar_t* FileExt() 
	{
		return m_FileExt.c_str(); 
	}
	const wchar_t* FileForder()
	{
		return m_FileForder.c_str();
	}


	std::wstring& ws_allpath()
	{
		return m_AllPath;
	}
	std::wstring& ws_filename()
	{
		return m_FileName;
	}
	std::wstring& ws_filenameext()
	{
		return m_FileNameExi;
	}
	std::wstring& ws_fileext()
	{
		return m_FileExt;
	}
	std::wstring& ws_filefolder()
	{
		return m_FileForder;
	}

	void FileForder(const wchar_t* _pPathKey) { m_FileForder = _pPathKey; }
	bool Compare_Ext(const wchar_t* _pExt) { return m_FileExt == _pExt; }

	void Split_Path(const wchar_t* _pPath);

private:
	virtual bool Create();
	virtual bool Load();
	virtual bool Save(KPtr<KResource> _Res);

};

