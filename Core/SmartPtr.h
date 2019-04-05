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
	// sizeof 트릭 -> sizeof는 연산자이기 때문에 이것을
	// 계산하기 위해서 Check라는 함수가 먼저 계산되어야함 -> 크기 계산
	// 인자값으로 판단하기 때문에(인자는 상속이 맞는지 아닌지)
	// 아니면 char를 반환하므로 거기에 -1을 하여 true false를 판단함
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
	// 템플릿 프랜드.
	template<typename T>
	friend class KPtr;

private:
	int m_Count;

private:
	void Add_Cnt(); // 늘어날때
	void Dec_Cnt(); // 줄어들때
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

	// 형변환
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

	// static 일반 변수 캐스팅
	// reinterpret 강제 캐스팅
	// dynamic 상속관계 캐스팅 - C스타일이 아님 - virtual 상속간에만 가능
	// 런타임 때 연산 -> 문자열 비교
	// const const or volatile 해제 시

	template<typename InType>
	operator KPtr<InType>()
	{
		if (nullptr == m_Ptr)
		{
			return nullptr;
		}

		// 일반적으로 상속구조일때 사용하는 캐스팅입니다.
		// 상속이 아니면 nullptr 반환
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

		// 대입 연산이나 * 에대한 대입 연산이 들어올 경우
		// 내가 가지고 있던 Cnt는 하나 준다
		// 그리고 내가 가지고 있던 것이 대체 되므로
		// 그 새로운 것은 하나가 증가한다.
		// Smart 포인터는 내가 어디서 몇개의 포인터를 썻던 간에 
		// 자동으로 어디서 몇 개를 썻는 지 세어서 0 이하가 되면 삭제 한다. -> 아무도 쓰는 곳이 없으므로
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
