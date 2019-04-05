#pragma once
#include "SmartPtr.h"
#include <string>
#include <typeinfo.h>
#include "KMacro.h"

class Begin_Type : public SmartPtr 
{
protected:
	Begin_Type(const Begin_Type& _Other) : SmartPtr(_Other),  m_pTypeInfo(_Other.m_pTypeInfo), m_bDebug(_Other.m_bDebug) {}
	Begin_Type() : m_bDebug(false) {}
	~Begin_Type() {}

private:

	// 타입의 형을 비교함 -> 여기에 해쉬코드, 이름등이 저장되고
	// 특히 해쉬코드는 컴퓨터가 달라져도 값이 같음 -> 씬을 저장하고
	// 불러오는데에 중요한 역할
	const type_info* m_pTypeInfo;
	bool m_bDebug;

public:
	bool IsDebug()  { return m_bDebug; }
	void DebugOn() { m_bDebug = true; }
	void DebugOff() { m_bDebug = false; }

public:
	const type_info* TypeInfo()
	{
		return m_pTypeInfo;
	}

	void Set_Type()
	{
		// 타입 인포를 저장하는 단계 -> 인자값으로는 값형식을 넣어준다.
		// 포인터를 그대로 넣어주면 값 비교시 포인터를 비교하는 꼴이 되어버림
		m_pTypeInfo = &typeid(*this);
	}

	// 여러 타입을 비교할 수 있게끔 탬플릿으로 받게 한다.
	template <typename T>
	bool IsEqual()
	{
		return (*m_pTypeInfo) == typeid(T);
	}

	// 다이나믹 캐스트 -> virtual로 된 클래스 간 상속의 여부를 알수 있음
	// 정확히 말하면 상속이 이루어져 있을 때 그 상속하는 객체의 주소를 가져옴
	// 다운 캐스팅은 안되며 virtual 연결이 되어야만 함
	template <typename T>
	bool IsParent()
	{
		// return true;

		return dynamic_cast<T*>(this) != nullptr;
	}
};

// type_info
class Begin : public Begin_Type
{
public:
	Begin();
	Begin(const Begin& _Other);
	Begin(const wchar_t* _Name);
	~Begin();

private:
	std::wstring m_Name;

public:
	KUINT size_name()
	{
		return (KUINT)m_Name.size() + 1;
	}

	const wchar_t* name() 
	{
		return m_Name.c_str();
	}

	void name(const wchar_t* _Name)
	{
		m_Name = _Name;
	}

	const std::wstring ws_name()
	{
		return m_Name;
	}

};

