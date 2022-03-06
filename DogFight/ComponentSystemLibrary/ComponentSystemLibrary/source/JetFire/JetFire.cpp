#include "JetFire.h"
#include "../../Component/SceneManager.h"

Vector3 g_pos(0, 3, 0);

using namespace Component;

JetFire::JetFire(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("JetFire");

	tra = owner->transform;

	for (int i = 0; i < conenum; i++) {
		Cone* cone = new Cone();
		cone->Init(XMFLOAT4(1, 0, 0, 0.5f), (i+1) * 0.25f);
		conelist.emplace_back(cone);
	}
}

JetFire::~JetFire()
{
	RenderingType type = RenderingType::Transparent;
	SceneManager::DeleteDrawFunction(type, owner);
}

void JetFire::Start()
{
	RenderingType type = RenderingType::Transparent;
	SceneManager::AddDrawFunction(type, owner);
	player = SceneManager::Instance()->GameObjectFindName("player")->GetComponent<Component::PlayerObj>();
}

void JetFire::Update()
{
	if (player->GetStatus() == Status::LIVE) {
		auto _pos = player->GetOwner()->transform->position
			- player->GetOwner()->transform->up() * 0.35f - player->GetOwner()->transform->forward() * 10;

		FlickerFire();		
	}
	else {
		owner->SetExistState(false);
	}

}

void JetFire::DrawObject()
{
	int forward = 10;
	XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 0, 0.5f);
	// ƒŠƒXƒg•`‰æ

	float _speed = player->GetSpeed();
	float _maxspeed = player->GetMaxSpeed();

	float _rate = _speed / _maxspeed;
	float _colorRate = (1.0 - _rate);
	//----------‚P Â-----------//
	col.x = _colorRate; if (col.x < 0.1)col.x = 0.1f;
	col.y = 0.0f;
	col.z = 0.2f;
	col.w = 0.3f;
	g_pos.y = (1 + m_firesize) * _rate;
	conelist[0]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position 
		- player->GetOwner()->transform->up() * 0.35f - player->GetOwner()->transform->forward() * forward * 1.25f
		, player->GetOwner()->transform->qt, 1, 1);
	
	conelist[0]->Draw();

	//------------2 Â----------//
	col.x = _colorRate; if (col.x < 0.2)col.x = 0.2f;
	col.y = 0.0f;
	col.z = _rate / 2;
	col.w = 0.2f;
	g_pos.y = (1.5f + m_firesize) * _rate;
	conelist[1]->BufferUpdate(col, g_pos);
	conelist[1]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position
		- player->GetOwner()->transform->up() * 0.35f - player->GetOwner()->transform->forward() * forward * 1.25f
		, player->GetOwner()->transform->qt, 1, 1);
	
	conelist[1]->Draw();

	//----------3 Â‡-----------//
	col.x =  _colorRate; if (col.x < 0.1)col.x = 0.3f;
	col.y = 0.0f;
	col.z = _rate;
	col.w = 0.1f;

	g_pos.y = (2 + m_firesize) * _rate;
	conelist[2]->BufferUpdate(col, g_pos);
	conelist[2]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position
		- player->GetOwner()->transform->up() * 0.35f - player->GetOwner()->transform->forward() * forward * 1.25f
		,player->GetOwner()->transform->qt, 1, 1);
	
	conelist[2]->Draw();

	g_pos.z -= 1;


}

// ‰Î‚Ì‚¿‚ç‚Â‚«
void JetFire::FlickerFire()
{

	if (m_nowFlickcnt > m_maxFlickcnt)
	{
		m_nowFlickcnt = 0;

		m_firesize = RandomClass::GetRandom(0.0f, 1.0f);

		m_maxFlickcnt = RandomClass::GetRandom(0.0f, 3.0f);

	}
	else {
		m_nowFlickcnt++;
	}
}