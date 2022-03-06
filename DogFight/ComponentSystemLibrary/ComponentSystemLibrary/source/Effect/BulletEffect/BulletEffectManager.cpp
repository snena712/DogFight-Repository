#include "BulletEffectManager.h"

using namespace Component;

BulletEffectManager::BulletEffectManager(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("BulletEffectManager");

	std::vector<GameObject*> _list;

	// 弾初期化
	for (int i = 0; i < m_effectnum; i++)
	{
		GameObject* _effect = new GameObject();
		_effect->transform->position.x = 10000;
		auto _efe=_effect->AddComponent<Component::BulletEffect>();
		m_effectList.emplace_back(_efe);
		_list.emplace_back(_effect);
		_effect->SetExistState(false);
	}
	SceneManager::Instance()->AddList(&_list);
}

BulletEffectManager::~BulletEffectManager()
{
}

void BulletEffectManager::Start()
{

}

void BulletEffectManager::Update()
{

}


// エフェクト生成
void BulletEffectManager::Fire(Vector3 _pos, Vector3 _size)
{
	// エフェクトのない物の探索
	for (auto list : m_effectList) {

		if (!list->GetOwner()->GetExistState())
		{
			list->GetOwner()->SetExistState(true);
			list->SetPosSize(_pos, _size);
			
			break;
		}
	}

}


