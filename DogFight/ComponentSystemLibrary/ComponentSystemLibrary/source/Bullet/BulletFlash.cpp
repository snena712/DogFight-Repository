#include "BulletFlash.h"

using namespace Component;

BulletFlash::BulletFlash(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName( "BulletFlash");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/smoke_white1.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetVertexColor(XMFLOAT4(1, 1, 0, 0.5f));

	ren->SetHeight(2.5f);
	ren->SetWidth(2.5f);

}

BulletFlash::~BulletFlash()
{
}

void BulletFlash::Start()
{
}

void BulletFlash ::Update()
{
	if (m_life <= 0)
		owner->SetExistState(false);
	if(m_life>0){
		LifeReduce();
	}
}


