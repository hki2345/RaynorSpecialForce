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

// �������� ������Ʈ �ϴ� �༮���� ó���� ���⼭ �����ٴ� ���� �������.
// ���� �����̶� �ڸ�
void Component::FinalUpdate() 
{

}
void Component::EndUpdate()
{

}