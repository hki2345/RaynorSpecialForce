#pragma once
#include <type_traits>

template<typename BASE, typename CHILD>
class TypeCheck
{
private:
	static short Check(BASE*) {};
	static char Check(...) {};

	static CHILD* p;

public:
	// sizeof Ʈ�� -> sizeof�� �������̱� ������ �̰���
	// ����ϱ� ���ؼ� Check��� �Լ��� ���� ���Ǿ���� -> ũ�� ���
	// ���ڰ����� �Ǵ��ϱ� ������(���ڴ� ����� �´��� �ƴ���)
	// �ƴϸ� char�� ��ȯ�ϹǷ� �ű⿡ -1�� �Ͽ� true false�� �Ǵ���
	static const bool Value = (bool)(sizeof(Check(p)) - 1);

private:
	TypeCheck() {}
	~TypeCheck() {}
};

class SmartPtr
{
public:
	SmartPtr();
	SmartPtr(const SmartPtr& _Other);
	virtual ~SmartPtr() = 0;

public:
	// ���ø� ������.
	template<typename T>
	friend class KPtr;

private:
	int m_Count;

private:
	void Add_Cnt(); // �þ��
	void Dec_Cnt(); // �پ�鶧
};

template<typename T/*, std::enable_if<std::is_base_of<RefBase, T>::value, T>::type*/>
class KPtr
{
private:
	T* m_Ptr;

public:
	KPtr() : m_Ptr(nullptr) {}

	KPtr(T* _PTR) : m_Ptr(_PTR)
	{
		Add_Ptr();
	}
	KPtr(const KPtr& _PTR) : m_Ptr(_PTR.m_Ptr)
	{
		Add_Ptr();
	}
	KPtr(std::nullptr_t _PTR) : m_Ptr(_PTR)
	{
		// nullptr;
	}
	~KPtr()
	{
		Dec_Ptr();
	}

public:
	bool operator==(std::nullptr_t _PTR) const
	{
		return m_Ptr == _PTR;
	}

	bool operator==(T* _PTR) const
	{
		return m_Ptr == _PTR;
	}

	bool operator==(const KPtr& _Other) const
	{
		return m_Ptr == _Other.m_Ptr;
	}

	bool operator<(const KPtr& _Other) const
	{
		return m_Ptr < _Other.m_Ptr;
	}

	bool operator>(const KPtr& _Other) const
	{
		return m_Ptr > _Other.m_Ptr;
	}

	bool operator!=(std::nullptr_t _PTR) const
	{
		return m_Ptr != _PTR;
	}

	// ����ȯ
	operator T*() {
		return m_Ptr;
	}

	const T* operator->() const
	{
		return m_Ptr;
	}

	T* operator->()
	{
		return m_Ptr;
	}

	T& operator*()
	{
		return *m_Ptr;
	}

	// static �Ϲ� ���� ĳ����
	// reinterpret ���� ĳ����
	// dynamic ��Ӱ��� ĳ���� - C��Ÿ���� �ƴ� - virtual ��Ӱ����� ����
	// ��Ÿ�� �� ���� -> ���ڿ� ��
	// const const or volatile ���� ��

	template<typename InType>
	operator KPtr<InType>()
	{
		if (nullptr == m_Ptr)
		{
			return nullptr;
		}

		// �Ϲ������� ��ӱ����϶� ����ϴ� ĳ�����Դϴ�.
		// ����� �ƴϸ� nullptr ��ȯ
		InType* P = dynamic_cast<InType*>(m_Ptr);
		if (true == TypeCheck<T, InType>::Value)
		{
			return (InType*)m_Ptr;
		}

		return nullptr;
	}

	KPtr<T>& operator = (T* _PTR)
	{
		if (m_Ptr == _PTR)
		{
			return *this;
		}

		// ���� �����̳� * ������ ���� ������ ���� ���
		// ���� ������ �ִ� Cnt�� �ϳ� �ش�
		// �׸��� ���� ������ �ִ� ���� ��ü �ǹǷ�
		// �� ���ο� ���� �ϳ��� �����Ѵ�.
		// Smart �����ʹ� ���� ��� ��� �����͸� ���� ���� 
		// �ڵ����� ��� �� ���� ���� �� ��� 0 ���ϰ� �Ǹ� ���� �Ѵ�. -> �ƹ��� ���� ���� �����Ƿ�
		Dec_Ptr();
		m_Ptr = _PTR;
		Add_Ptr();
		return *this;
	}

	KPtr<T>& operator = (const KPtr& _PTR)
	{
		if (m_Ptr == _PTR.m_Ptr)
		{
			return *this;
		}

		Dec_Ptr();
		m_Ptr = _PTR.m_Ptr;
		Add_Ptr();
		return *this;
	}

private:
	void Add_Ptr()
	{
		if (nullptr != m_Ptr)
		{
			m_Ptr->Add_Cnt();
		}
	}

	void Dec_Ptr()
	{
		if (nullptr != m_Ptr)
		{
			m_Ptr->Dec_Cnt();
		}
	}
};

template<typename T>
bool operator==(std::nullptr_t _PTR, const KPtr<T>& _KPtr) { return _KPtr == _PTR; }

template<typename T>
bool operator!=(std::nullptr_t _PTR, const KPtr<T>& _KPtr) { return _KPtr != _PTR; }
