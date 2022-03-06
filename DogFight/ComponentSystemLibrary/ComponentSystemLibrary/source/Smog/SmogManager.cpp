#include "SmogManager.h"

using namespace Component;

SmogManager::SmogManager(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SmogManager");

	for (auto i = 0; i < smognum; i++)
	{
		// ‰_¶¬
		GameObject* smog = new GameObject();
		smog->AddComponent<Component::SmogPlayer>();
		smog->transform->position = Vector3(10000, 0, 0);
		smog->SetExistState(false);
		// ƒŠƒXƒg‚É’Ç‰Á
		smoglist.emplace_back(smog->GetComponent<Component::SmogPlayer>());
		SceneManager::Instance()->AddObject(smog, false);
	}

	for (auto i = 0; i < wingnum; i++)
	{
		// ‰H‰_¶¬
		GameObject* wing = new GameObject();
		wing->AddComponent<Component::WingSmog>();
		wing->transform->position = Vector3(10000, 0, 0);
		wing->SetExistState(false);
		// ƒŠƒXƒg‚É’Ç‰Á
		winglist.emplace_back(wing->GetComponent<Component::WingSmog>());
		SceneManager::Instance()->AddObject(wing, false);
	}

	for (auto i = 0; i < enenum; i++)
	{
		// “G‰H‰_¶¬
		GameObject* ewing = new GameObject();
		ewing->AddComponent<Component::EnemyWingSmog>();
		ewing->transform->position = Vector3(10000, 0, 0);
		ewing->SetExistState(false);

		// ƒŠƒXƒg‚É’Ç‰Á
		ewinglist.emplace_back(ewing->GetComponent<Component::EnemyWingSmog>());
		SceneManager::Instance()->AddObject(ewing, false);
	}

	for (auto i = 0; i < enenum; i++)
	{
		// “G‰H‰_¶¬
		GameObject* airsmog = new GameObject();
		airsmog->AddComponent<Component::AirSmog>();
		airsmog->transform->position = Vector3(10000, 0, 0);
		airsmog->SetExistState(false);

		// ƒŠƒXƒg‚É’Ç‰Á
		airsmoglist.emplace_back(airsmog->GetComponent<Component::AirSmog>());
		SceneManager::Instance()->AddObject(airsmog, false);
	}

	// ‰_‚Ì‰Šú‰»
	for (auto i = 0; i < cloudnum; i++)
	{
		// “G‰H‰_¶¬
		GameObject* cloud = new GameObject();
		cloud->AddComponent<Component::AirCloud>();
		cloud->transform->position = Vector3(10000, 0, 0);
		cloud->SetExistState(false);

		// ƒŠƒXƒg‚É’Ç‰Á
		aircloudlist.emplace_back(cloud->GetComponent<Component::AirCloud>());
		SceneManager::Instance()->AddObject(cloud, false);
	}

	// ‰_‚Ì¶¬
	InstanceCloud(Vector3(cloudPosLimit, cloudPosLimit, cloudPosLimit));
	
}

// ˆø”‚ğêŠ‚É‚·‚é
void  SmogManager::SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	auto a= smoglist;
	for (auto list : smoglist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == SmogPlayer::Status::DEAD)
		{
			list->SetStatus(SmogPlayer::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			list->SetType(SmogPlayer::ObjType::Player);
			break;
		}
	}
}


void  SmogManager::SetWingSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce)
{
	for (auto list : winglist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == WingSmog::Status::DEAD)
		{
			list->SetStatus(WingSmog::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			break;
		}
	}
}

// “G€–S—p‚Ì‰Œ
void  SmogManager::SetEnemyDeadSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	// €‚ñ‚¾‚Æ‚«‚Éo‚·‚à‚Ì‚É‚·‚é
	for (auto list : smoglist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == SmogPlayer::Status::DEAD)
		{
			list->SetStatus(SmogPlayer::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			list->SetType(SmogPlayer::ObjType::Enemy);

			break;
		}
	}
}

// ƒvƒŒƒCƒ„[”òs‚Ì‰Œ
void  SmogManager::SetEnemySmog(Vector3 _vec, XMFLOAT4 _qt, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	// ’Êí‚Éo‚·‚à‚Ì‚É‚·‚é

	for (auto list : ewinglist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == EnemyWingSmog::Status::DEAD)
		{

			list->SetStatus(EnemyWingSmog::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->GetOwner()->transform->qt = _qt;			
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			break;
		}
	}

}


void  SmogManager::SetAirSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce)
{
	// ’Êí‚Éo‚·‚à‚Ì‚É‚·‚é

	for (auto list : airsmoglist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == AirSmog::Status::DEAD)
		{
			list->SetStatus(AirSmog::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			break;
		}
	}
}


void  SmogManager::InstanceCloud(Vector3 _vec)
{
	// ’Êí‚Éo‚·‚à‚Ì‚É‚·‚é

	for (auto list : aircloudlist)
	{
		// €‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == AirCloud::Status::DEAD)
		{
			auto  _cloudPos = 
				Vector3(RandomClass::GetRandom(-_vec.x, _vec.x), RandomClass::GetRandom(0.0f, _vec.y)+500, RandomClass::GetRandom(-_vec.z, _vec.z));
			list->SetStatus(AirCloud::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _cloudPos;
			list->SetAlpha(DirectX::XMFLOAT4(1,1,1,0.3f));
			list->SetSize(500, 200);
			list->SetReduce(0);	
		}
	}
}