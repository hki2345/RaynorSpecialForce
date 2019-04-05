// 상수버퍼는 b
// 텍스처는 t
// 샘플러는 s
#include "Light.fx"

cbuffer LightBuffer : register(b12)
{
	LIGHTDATA ArrLight[10];
	int       LightCount;
	int       LightCount1;
    int LightCount2;
    int LightCount3;
}
