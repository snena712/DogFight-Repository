#include "HommigBullet.h"
#include "../Enemy/Enemy.h"

using namespace Component;

HommingBullet::HommingBullet(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName( "HomingBullet");
	owner->SetTag("Bullet");
	tra = owner->transform;

	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2/f2a_misairu.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",				// 頂点シェーダー
		"shader/ps.hlsl",				// ピクセルシェーダー
		"assets/F2/f2a_texture/");		// テクスチャの存在フォルダ

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	Component::SphereCollider* col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(5);
	col->SetSize(5);

	bul= owner->AddComponent < Component::BulletBase >();

}

HommingBullet::~HommingBullet()
{
}

void HommingBullet::Start()
{

}

void HommingBullet::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		switch (bul->GetStatus())
		{
		case Component::BulletBase::Status::Live:

			// 移動
			MoveUpdate();

			// 煙生成
			SmogUpdate();

			// 当たり判定
			Collision();

			break;

		case Component::BulletBase::Status::Dead:

			// アニメーションが終わるまで削除待つ
			bul->SetStatus(Component::BulletBase::Status::Dead);
			bul->SetSpeed(0);
			m_targetobj = nullptr;
			m_targetPos = Vector3(0, 0, 0);
			break;
		default:
			break;
		}
	}
}

// 描画
void HommingBullet::Draw()
{
	// bulletと同じにする
}

// 移動の更新
void HommingBullet::MoveUpdate()
{
	Vector3 f = tra->forward();
	Vector3 d = m_targetPos - owner->transform->position;

	if (m_targetobj != nullptr) {
		m_targetPos = m_targetobj->transform->position;
		f = tra->forward();
		d = m_targetPos - owner->transform->position;
	}

	if (f.Dot(d) < 0)
	{
		// ターゲット消す
	}
	else {
		if (m_targetobj != nullptr) {
			// ポジションの更新がない
			owner->transform->LookSlerp(m_targetPos, owner->transform->up(), movrRotMax);
		}
	}

	bul->SetSpeed(bul->GetSpeed() + 0.1f);	// スピードの代入
	owner->transform->position += tra->forward() * bul->GetSpeed();

	// 弾のライフの状態遷移
	if (bul->GetLife() > 0)
	{
		bul->SetLife(bul->GetLife() - 1);
	}
	else {
		
		bul->SetStatus(Component::BulletBase::Status::Dead);
		owner->SetExistState(false);
	}
}

// 煙の更新
void HommingBullet::SmogUpdate()
{
	smog->SetActive(owner->transform->position - owner->transform->forward() * 6- owner->transform->up()/2, Vector3(2, 2, 1), XMFLOAT4(1, 1, 1, 0.7f), Vector3(0.05f, 0.05f, 0.05f), 0.01f);
}

void HommingBullet::Collision()
{
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		// 球が当たった時
		//if (_rigidlist.obj->GetName() == "enemy")
		//{
		//	// 存在しているとき
		//	if (_rigidlist.obj->GetExistState() &&
		//		_rigidlist.obj->GetComponent<Component::Enemy>()->GetStatus())
		//	{
		//		_rigidlist.obj->GetComponent<Component::Enemy>()->LowerLife(100);
		//		m_hitflg = true;
		//	}

		//}
	}
}