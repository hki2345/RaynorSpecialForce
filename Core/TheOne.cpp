#include "TheOne.h"
#include "Component.h"
#include "KMacro.h"
#include "State.h"
#include "Core_Class.h"
#include "RenderManager.h"
#include "CollisionManager.h"


TheOne::TheOne()
{
}

TheOne::~TheOne()
{
}

KPtr<Component> TheOne::Set_Component(KPtr<Component> _TCom)
{

	_TCom->Set_Type();
	_TCom->one(this);
	_TCom->kwindow(kwindow());
	_TCom->state(state());
	_TCom->ComInit();

	
	m_ComList.push_back(_TCom);
	return _TCom;
}


void TheOne::Set_Abs() 
{
	m_AbsType = AT_ALLOVER;

	if (true == kwindow()->statemanager.Find_Abs(this))
	{
		return;
	}

	kwindow()->statemanager.m_ABSOneList.push_back(this);

	std::unordered_map<std::wstring, KPtr<State>>::iterator StartMapIter = kwindow()->statemanager.m_StateMap.begin();
	std::unordered_map<std::wstring, KPtr<State>>::iterator EndMapIter = kwindow()->statemanager.m_StateMap.end();

	for (; StartMapIter != EndMapIter; ++StartMapIter)
	{
		if (StartMapIter->second == state())
		{
			continue;
		}

		StartMapIter->second->OverPushActor(this);
	}
}

void TheOne::PrevUpdate() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->PrevUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->PrevUpdate();
	}

}

void TheOne::Update() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->Update();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Update();
	}
}

void TheOne::NextUpdate() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->NextUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->NextUpdate();
	}
}

void TheOne::DebugUpdate() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->DebugUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DebugUpdate();
	}
}

void TheOne::FinalUpdate() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->FinalUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->FinalUpdate();
	}
}

void TheOne::DebugRender() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->DebugRender();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->DebugRender();
	}
}



void TheOne::UIRender()
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->UIRender();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->UIRender();
	}
}

void TheOne::EndUpdate() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->Is_Active())
		{
			continue;
		}

		(*m_ComSI)->EndUpdate();
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->EndUpdate();
	}
}

void TheOne::Release() 
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; )
	{
		if (true == (*m_ComSI)->Is_Death())
		{
			m_ComSI = m_ComList.erase(m_ComSI);
		}
		else {
			++m_ComSI;
		}
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Release();
	}
}

void TheOne::Set_Death() 
{
	Begin_Updater::Set_Death();

	std::list<KPtr<Component>>::iterator ComStartIter = m_ComList.begin();
	std::list<KPtr<Component>>::iterator ComEndIter = m_ComList.end();

	for (; ComStartIter != ComEndIter; ++ComStartIter)
	{
		(*ComStartIter)->Set_Death();
	}
}

void TheOne::Add_Child(KPtr<TheOne> _Actor, bool _TransWorld /*= true*/)
{
	if (AT_NONE != _Actor->abs_type())
	{
		return;
	}

	if (false == state()->EraseActor(_Actor))
	{
		return;
	}

	_Actor->m_pParent = this;
	m_pChildList.push_back(_Actor);

	if (true == _TransWorld && nullptr != Trans() && nullptr != _Actor->Trans())
	{
		// 100, 100, 100
		// 100, 100, 100
		// 크기
		KVector4 CScale = _Actor->Trans()->scale_local();
		KVector4 Scale = Trans()->scale_local();

		// KVector4 R = CScale / Scale;

		_Actor->Trans()->scale_local(CScale / Scale);

		// 100, 100 // 자식
		// 50, 50 // 부모
		KVector4 CPos = _Actor->Trans()->pos_local();
		KVector4 Pos = Trans()->pos_local();
		_Actor->Trans()->pos_local((CPos - Pos) / Trans()->scale_local());

		// 90 
		// 90
		// 회전
		KVector4 CRot = _Actor->Trans()->rotate_local();
		KVector4 Rot = Trans()->rotate_local();
		_Actor->Trans()->rotate_local(CRot - Rot);
	}
	else if (false == _TransWorld && nullptr != Trans() && nullptr != _Actor->Trans())
	{
		_Actor->Trans()->Reset();
	}

} 



void TheOne::Detach() 
{
	std::list<KPtr<TheOne>>::iterator StartIter = m_pParent->m_pChildList.begin();
	std::list<KPtr<TheOne>>::iterator EndIter = m_pParent->m_pChildList.end();

	bool ChildCheck = false;
	for (; StartIter != EndIter; ++StartIter)
	{
		if ((*StartIter) == this)
		{
			m_pParent->m_pChildList.erase(StartIter);
			ChildCheck = true;
			break;
		}
	}

	if (false == ChildCheck)
	{
		KASSERT(false == ChildCheck);
		return;
	}
	// m_pParent->state()->add
}

#include "Renderer.h"

void TheOne::Insert_ABSRender(RenderManager* _pRenMgr)
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->IsParent<Renderer>())
		{
			continue;
		}
		_pRenMgr->Insert_AbsRenderer((*m_ComSI));
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Insert_ABSRender(_pRenMgr);
	}
}

void TheOne::Insert_ABSCol(CollisionManager* _pCol2DMgr, CollisionManager* _pCol3DMgr)
{
	m_ComSI = m_ComList.begin();
	m_ComEI = m_ComList.end();

	for (; m_ComSI != m_ComEI; ++m_ComSI)
	{
		if (false == (*m_ComSI)->IsParent<KCollider2D_DE>())
		{
			_pCol2DMgr->PushCol((*m_ComSI));
		}

		// else if (true == (*m_ComSI)->IsParent<KCollider3D_DE>())
		// {
		// 	_pCol3DMgr->PushCol((*m_ComSI));
		// }
	}

	ChildStartIter = m_pChildList.begin();
	ChildEndIter = m_pChildList.end();

	for (; ChildStartIter != ChildEndIter; ++ChildStartIter)
	{
		(*ChildStartIter)->Insert_ABSCol(_pCol2DMgr, _pCol3DMgr);
	}
}