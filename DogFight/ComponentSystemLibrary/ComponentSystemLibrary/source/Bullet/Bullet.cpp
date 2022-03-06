#include "Bullet.h"

using namespace Component;

Bullet::Bullet(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "Bullet");
	owner->SetTag("Bullet");

	tra = owner->transform;  
	
	// 爆破アニメーション
	model = owner->AddComponent<Component::ModelRender>(
		"assets/Bullet/bullet.fbx",
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps_yellow.hlsl",			// ピクセルシェーダー
		"");			// テクスチャの存在フォルダ);

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(5);
	col->SetSize(1);

	bul = owner->AddComponent<Component::BulletBase>();
}

Bullet::~Bullet()
{
}

void Bullet::Start() 
{
}

void Bullet::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		switch (bul->GetStatus())
		{
		case Component::BulletBase::Status::Live:

			// 移動更新
			MoveUpdate();

			// 体力の確認
			LifeUpdate();

			// 当たり判定
			Collision();

			break;

		case Component::BulletBase::Status::Dead:
			break;
		default:
			break;
		}
	}

}

// 移動更新
void Bullet::MoveUpdate()
{
	tra->position.x += m_speed * tra->forward().x;
	tra->position.y += m_speed * tra->forward().y;
	tra->position.z += m_speed * tra->forward().z;
}

// 体力の更新
void Bullet::LifeUpdate()
{
	// 寿命
	if (bul->GetLife() > 0)
	{
		bul->SetLife(bul->GetLife() - 1);
	}
	else {

		bul->SetStatus(Component::BulletBase::Status::Dead);
		owner->SetExistState(false);
	}
}

// 当たり判定
void Bullet::Collision()
{
	//for (auto& _rigidlist : *rb->GetCollisionObject())
	//{
	//	//// 球が当たった時
	//	//if (_rigidlist.obj->GetName() == "enemy")
	//	//{
	//	//	// 存在しているとき
	//	//	if (_rigidlist.obj->GetExistState() &&
	//	//		_rigidlist.obj->GetComponent<Component::Enemy>()->GetStatus()) 
	//	//	{
	//	//		_rigidlist.obj->GetComponent<Component::Enemy>()->LowerLife(1);
	//	//		m_hitflg = true;
	//	//	}
	//	//	
	//	//}
	//}
}