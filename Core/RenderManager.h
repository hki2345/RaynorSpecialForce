#pragma once
#include <map>
#include <set>
#include <list>
#include "SmartPtr.h"
#include "Camera.h"

class State;
class Renderer;
class Camera;
class TheOne;
class Light;
class RenderManager
{
public:
	RenderManager();
	~RenderManager();

public:
	friend TheOne;
	friend State;
	friend Camera;
	friend Renderer;
	friend class DebugManager;

private:
	static bool Sort_Z(KPtr<Renderer> _Left, KPtr<Renderer> _Right);

private:
	// Camera
	std::map<int, KPtr<Camera>>::iterator m_CSI;
	std::map<int, KPtr<Camera>>::iterator m_CEI;
	std::map<int, KPtr<Camera>> m_CamMap;


	// List Int
	// Renderer
	// 이 레이어 이터는 카메라가 들고 있음 ㅇㅇ - ㅇ 벡터로 바꿈
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_ALLFI;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_ALLRSI;
	std::map<int, std::list<KPtr<Renderer>>>::iterator m_ALLREI;

	std::list<KPtr<Renderer>>::iterator m_RSI;
	std::list<KPtr<Renderer>>::iterator m_REI;
	std::map<int, std::list<KPtr<Renderer>>> m_RenderMap;


private:
	void Insert_Cam(Camera* _Camera);
	void Insert_Renderer(Renderer* _Renderer);
	void Insert_AbsRenderer(KPtr<Renderer> _Renderer);

public:
	// Light
	std::set<KPtr<Light>> m_LightSet;
	std::set<KPtr<Light>>::iterator m_LSI;
	std::set<KPtr<Light>>::iterator m_LEI;

	void Insert_Light(Light* _Camera);
	void Check_Light(KPtr<Camera> _Camera, int _Group);
	void Render_DefLight(KPtr<Camera> _Camera, int _Group);

private:
	void Reset_SList();

public:
	void Render();
	void Render_Defferd(KPtr<Camera> _Camera, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index);
	void Render_Forward(KPtr<Camera> _Camera, std::map<int, std::list<KPtr<Renderer>>>::iterator _Iter, size_t _Index);
	void Merge_Screen();
	void Release();

};

