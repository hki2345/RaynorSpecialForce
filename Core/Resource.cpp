#include "Resource.h"



KResource::KResource()
{
}

KResource::KResource(const KResource& _Other) : Begin(_Other),
	m_FileForder(_Other.m_FileForder),
	m_AllPath(_Other.m_AllPath),
	m_FileNameExi(_Other.m_FileNameExi),
	m_FileName(_Other.m_FileName),
	m_FileExt(_Other.m_FileExt)

{

}

KResource::~KResource()
{
}


// 이것을 상속받지 않고 실행하면 거짓을 반환하게 함 -> 상속 받을 수 있게
bool KResource::Create()
{
	return false;
}
bool KResource::Load() 
{
	return false;
}
bool KResource::Save(KPtr<KResource> _Res)
{
	return false;
}


void KResource::Split_Path(const wchar_t* _Path) 
{
	m_AllPath = _Path;

	wchar_t ArrDrive[256];
	wchar_t ArrFolder[256];
	wchar_t ArrFile[256];
	wchar_t ArrExt[256];
	_wsplitpath_s(_Path, ArrDrive, ArrFolder, ArrFile, ArrExt);

	// m_FileForder = ArrDrive;
	// m_FileForder += ArrFolder;

	m_FileName = ArrFile;
	m_FileExt = ArrExt; 
	m_FileNameExi = m_FileName + m_FileExt;

	// 폴더 별 저장은 - 멀티
	// 폴더가 파일 이름으로 되는 순간 - 솔로
	if (L"" == m_FileForder)
	{
		m_FileForder = m_FileNameExi;
	}
}