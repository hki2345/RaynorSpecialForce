#include "Component.h"
#include "WriteStream.h"
#include "ReadStream.h"
#include "KMacro.h"


bool Component::IsMulti(TheOne* _pActor) 
{
	return true;
}

Component::Component()
{
}


Component::~Component()
{
}

void Component::ComInit() {

}

// 엔진에서 업데이트 하는 녀석들의 처리는 여기서 끝난다는 것을 기억하자.
// 최종 적용이란 솟리
void Component::FinalUpdate() 
{

}
void Component::EndUpdate()
{

}