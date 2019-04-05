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

	// Ÿ���� ���� ���� -> ���⿡ �ؽ��ڵ�, �̸����� ����ǰ�
	// Ư�� �ؽ��ڵ�� ��ǻ�Ͱ� �޶����� ���� ���� -> ���� �����ϰ�
	// �ҷ����µ��� �߿��� ����
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
		// Ÿ�� ������ �����ϴ� �ܰ� -> ���ڰ����δ� �������� �־��ش�.
		// �����͸� �״�� �־��ָ� �� �񱳽� �����͸� ���ϴ� ���� �Ǿ����
		m_pTypeInfo = &typeid(*this);
	}

	// ���� Ÿ���� ���� �� �ְԲ� ���ø����� �ް� �Ѵ�.
	template <typename T>
	bool IsEqual()
	{
		return (*m_pTypeInfo) == typeid(T);
	}

	// ���̳��� ĳ��Ʈ -> virtual�� �� Ŭ���� �� ����� ���θ� �˼� ����
	// ��Ȯ�� ���ϸ� ����� �̷���� ���� �� �� ����ϴ� ��ü�� �ּҸ� ������
	// �ٿ� ĳ������ �ȵǸ� virtual ������ �Ǿ�߸� ��
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

