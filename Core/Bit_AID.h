#pragma once


// �ش� int�� �� ���� int�� �� Ȯ��
bool Confirm_Bit(const int& _Target, const int& _Num)
{
	return _Target && (1 << _Num);
}

// �ش� int�� �� ���� int�� �� 0 
int On_Bit(int& _Target, const int& _Num)
{
	return _Target |= (1 << _Num);
}

// �ش� int�� �� ���� int�� �� 0 
int Off_Bit(int& _Target, const int& _Num)
{
	return _Target &= (0 << _Num);
}