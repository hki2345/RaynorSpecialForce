#pragma once
#include <vector>
#include <unordered_map>
#include "KMacro.h"
#include "DXContainer.h"

#include "Stl_AID.h"
#include "SmartPtr.h"


#define IS_KEY(ACTION) InputManager::IsKey(ACTION)

#define KEY_PRESS(ACTION) InputManager::Press(ACTION)
#define KEY_UP(ACTION) InputManager::Up(ACTION)
#define KEY_DOWN(ACTION) InputManager::Down(ACTION)
#define KEY_UNPRESS(ACTION) InputManager::UnPress(ACTION)
#define KEY_OVER(ACTION) InputManager::Over(ACTION)
#define KEY_OVER_RESET(ACTION) InputManager::Over_Reset(ACTION)


// ���� ���̺귯������ ������ �̷��� ������ �̸� �����س��� �ҷ����� ���̴�.
enum INPMGR_ERR
{
	IERR_OK,
	IERR_CREATE_ERROR,
	IERR_CREATE_FIND_ERROR,
	IERR_MAX,
};

class KCore;
class KWindow;
class InputManager
{
private:
	InputManager();
	~InputManager();
	
public:
	friend KCore;
	friend KWindow;

public:
	static const char KM_ERRORMSG[IERR_MAX][256];

private:
	// ��Ʈ���� ����
	// �̹����� Ű�� char ���� �ϳ��� ��� �����ϵ��� �Ѵ�.
	// �������� ���������� �޸𸮸� �Ƴ� �� �ִ�. -> �Ʋ����� ������ ���̴�.
	// �̷����� �޸� �Ƴ��� �������ƶ� -> �ϴ��� ������ �����غ��� ��
	class Input_Command : public SmartPtr
	{
	private:
		static const char g_bUp;
		static const char g_bUpStay;
		static const char g_bDown;
		static const char g_bDownStay;
		static const char g_brUp;
		static const char g_brUpStay;
		static const char g_brDown;
		static const char g_brDownStay;

	public:
		friend InputManager;

	private:
		std::vector<int> m_KeyDataList;
		char m_Data;


		// �߰��� �� ->> ������ �ִ� �ð��� �����Ͽ� Over�� ������ �� ���� -> 
		// �����ð� ������ �ִ°�
		float m_PressTime;

	public:
		template<typename T, typename... Rest>
		void InsertKeyData(const T& _Key, Rest... _Arg)
		{
			m_KeyDataList.push_back(_Key);
			InsertKeyData(_Arg...);
		}

		void InsertKeyData()		{		}


	private:
		bool AllKeyCheck();
		void Update();
		
		inline bool Up();
		inline bool UnPress();
		inline bool Down();
		inline bool Press();
		inline bool Over(float _Time);
		inline bool Over_Reset(float _Time);

	public:
		Input_Command(const size_t _RSize);
		~Input_Command();
	};

	/******************* MOUSE *******************/

private:
	static POINT m_iPoint;
	static KVector2 m_MPos;
	static KVector2 m_PMPos;
	static KVector2 m_MDir;

	static bool m_bMove;

	// WHEEL
	static int m_WheelMove;
	static bool m_bLock;



public:
	// �޽������� �޴� �� -> �ƽ��Ե� MFC �ʿ��� �޽��� �޴� �� ���� ������ ���� ����.
	// ��, �ھ�� �����͸� �˾ƾ� �� �Լ��� private�� ���� �� �ִٴ� ��
	// ����� ��𼭳� ���������� MFC�� �ƴ� ���� �����Ѵٸ�
	// �� �Լ��� private�� �Ǵ� �� �´�. -> �޽����� �޾ƾ� �ϴϱ�
	// ����� ������ ���� ������ ������ �޴¤���.
	static void Receive_Wheel(const int& _Value)
	{
		m_WheelMove += _Value;
	}

	static KVector2 MousePos() {
		return m_MPos;
	}

	static KVector2 MouseDir() {
		return m_MDir;
	}

	static bool Check_InScr();

public:
	static const char* ErrorMsg(INPMGR_ERR _Msg);

private:
	static std::unordered_map<std::wstring, KPtr<Input_Command>>::iterator m_KeyStartIter;
	static std::unordered_map<std::wstring, KPtr<Input_Command>>::iterator m_KeyEndIter;
	static std::unordered_map<std::wstring, KPtr<Input_Command>> m_KeyMap;

public:
	template<typename... Rest>
	static INPMGR_ERR Create_Command(const wchar_t* _Name, Rest... _Arg) 
	{
		KPtr<Input_Command> pKEY = Map_Find<KPtr<Input_Command>>(m_KeyMap, _Name);

		if (nullptr != pKEY)
		{
			return IERR_CREATE_ERROR;
		}

		// ���ø� ��������.
		// ���� -> �̰� ����?
		// sizeof...(_Arg) ���������� ������ �������ݴϴ�.
		Input_Command* NewKeyData = new Input_Command(sizeof...(_Arg));
		NewKeyData->InsertKeyData(_Arg...);
		m_KeyMap.insert(std::unordered_map<std::wstring, KPtr<Input_Command>>::value_type(_Name, NewKeyData));
		return IERR_OK;
	}

public:
	static bool		IsKey(const wchar_t* _Name);
	static bool&	Is_MouseMove();

	static bool		Up(const wchar_t* _Name);
	static bool		UnPress(const wchar_t* _Name);
	static bool		Down(const wchar_t* _Name);
	static bool		Press(const wchar_t* _Name);
	static bool		Over(const wchar_t* _Name, float _Time);
	static bool		Over_Reset(const wchar_t* _Name, float _Time);
	
	static int&		WheelValue(int* _Ref);
	static void		Set_MLock();
	static void		Set_MUnLock();

private:
	static	void	Update_CheckMove();
	static	void	Update_Mouse();
	static	void	Update_Key();

	static  void	Update();
	static	void	EndUpdate();

};

