#pragma once


// 해당 int에 몇 번쨰 int의 값 확인
bool Confirm_Bit(const int& _Target, const int& _Num)
{
	return _Target && (1 << _Num);
}

// 해당 int에 몇 번쨰 int의 값 0 
int On_Bit(int& _Target, const int& _Num)
{
	return _Target |= (1 << _Num);
}

// 해당 int에 몇 번쨰 int의 값 0 
int Off_Bit(int& _Target, const int& _Num)
{
	return _Target &= (0 << _Num);
}