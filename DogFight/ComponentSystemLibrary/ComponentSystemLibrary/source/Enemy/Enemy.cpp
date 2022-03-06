#include "Enemy.h"

using namespace Component;

Enemy::Enemy(GameObject *_owner):ComponentBase(_owner)
{
	owner->SetName("enemy");
	owner->SetTag("Enemy");

	// 位置
	tra = owner->transform;
	tra->scale = 2;

	// モデルの設定
	model = owner->AddComponent<Component::ModelRender>(
		"assets/f1/f1.x",			// モデルデータ			
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/f1/");				// テクスチャの存在フォルダ

	// 当たり判定と重力
	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();
	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(10);

	// 状態
	status = Status::LIVE;

	// 移動方向
	RandMove();

	// 自分のフレーム追加
	GameObject* _frame = new GameObject();
	frame=_frame->AddComponent<Component::EnemyPosFrame>();  //コンポーネントでこのエネミー渡す
	_frame->GetComponent< Component::EnemyPosFrame>()->SetParent(owner);
	posFrameCom=_frame->GetComponent< Component::EnemyPosFrame>();
	SceneManager::Instance()->AddObject(_frame, false);

	auto bombobj = new GameObject();
	bomb=bombobj->AddComponent<Component::BombAnim>();  //コンポーネントでこのエネミー渡す
	SceneManager::Instance()->AddObject(bombobj, false);
	bombobj->SetExistState(false);

	lockui=owner->AddComponent<Component::LockOnUI>();
	lockStr = owner->AddComponent<Component::LockOnString>();
}

Enemy::~Enemy()
{
}

void Enemy::Start()
{
	smogmana= SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();
}

void Enemy::Update()
{
	if (status == Status::LIVE) {

		// 移動の範囲外の判定
		OutSide();

		// 移動
		MoveUpdate();

		// 回転
		SpinMove();

		//保存された		
		frame->CheckScreen();

		// 画面位置
		m_ScreenPos = posFrameCom->GetScreenPos();
		lockStr->SetLockOnPos(m_ScreenPos);
	}

	StatusUpdate();
}

void Enemy::DrawObject()
{
	if (status == Status::LIVE) {
		//GameObject::Draw();
	}
}

// 移動方向の更新
void Enemy::RandMove()
{
	switch (RandomClass::GetRandom(0, 7))
	{
	case 0:
		movedir = MoveDir::UP;
		break;
	case 1:
		movedir = MoveDir::DOWN;
		break;
	case 2:
		movedir = MoveDir::UP;
		break;
	case 3:
		movedir = MoveDir::DOWN;
		break;

	case 7:
		if (!turnFlg) {
			movedir = MoveDir::TURUN;
		}
		else {
			turnFlg = true;

			// 一回外出た後は普通の行動の阻止
			switch (RandomClass::GetRandom(0, 2))
			{
			case 0:
				movedir = MoveDir::UP;
				break;
			case 1:
				movedir = MoveDir::DOWN;
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}

}

void Enemy::SpinMove()
{
	if (movedir != MoveDir::TURUN)
	{
		if (m_spincnt < m_nowspincnt) {

			m_nowspincnt = 0;
			m_spincnt = RandomClass::GetRandom(30, 60);
			//回転の設定
			switch (RandomClass::GetRandom(0, 2))
			{
			case 0:
				spin = Spin::Right;
				break;
			case 1:
				spin = Spin::Right;
				break;
			case 2:
				spin = Spin::None;
				break;
			default:
				break;
			}
		}
		else {
			m_nowspincnt++;

			switch (spin)
			{
			case Component::Enemy::Spin::Right:
				tra->RotateLocalZaxis(1.0f);
				break;
			case Component::Enemy::Spin::Left:
				tra->RotateLocalZaxis(-1.0f);
				break;
			case Component::Enemy::Spin::None:
				break;
			default:
				break;
			}
		}
	}
}

//移動方向に対する更新
void Enemy::MoveUpdate()
{
	switch (movedir)
	{
	case Enemy::MoveDir::UP:
		tra->RotateLocalXaxis(-0.5f);
		break;
	case Enemy::MoveDir::DOWN:
		tra->RotateLocalXaxis(0.5f);
		break;
	case Enemy::MoveDir::RIGHT:
		//tra->RotateYaxis(1.0f);
		break;
	case Enemy::MoveDir::LEFT:
		//tra->RotateLocalYaxis(-1.0f);
		break;
	case Enemy::MoveDir::STRREAT:
		straightSpeed += speed;
		break;
	case Enemy::MoveDir::SLOW:
		straightSpeed -= speed;
		break;

	case MoveDir::TURUN:
		tra->RotateLocalXaxis(-outsidePlusCnt);
		turnCnt += outsidePlusCnt;
		if (turnCnt > turnMaxCnt)
		{
			// 各フラグの初期化
			turnCnt = 0;
		}
		break;

	case MoveDir::OUTSIDE:

		if (outsideFlg)
		{
			float t = (float)outsideCnt / 360.0f / 4.0f;
			outsideCnt++;
			tra->LookSlerp(m_centerPos, Vector3(0,1,0), t);
			if (t == 1)
			{
				outsideCnt = 0;
				outsideFlg = false;
				RandMove();
			}
		}

		break;

	default:
		break;
	}

	// 範囲外じゃない時
	if (movedir != MoveDir::OUTSIDE)
	{
		// 移動の更新
		if (m_movecnt < m_nowmovecnt)
		{
			RandMove();
			m_nowmovecnt = 0;
		}
		else {
			m_nowmovecnt++;
		}
	}

	// 自動移動
	AutoGo(tra->forward());
}

// 当たり判定
void Enemy::Collision()
{
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		if (_rigidlist.obj->GetTag()== "Bullet")
		{
			// アクティブ時のみ
			if (_rigidlist.obj->GetExistState()) {
				_rigidlist.obj->GetComponent<Component::BulletBase>()->SetStatus(BulletBase::Status::Dead);
				_rigidlist.obj->SetExistState(false);
					
				// 弾のダメージ
				if (_rigidlist.obj->GetName() == "Bullet")
				{
					LowerLife(2);

					// エフェクト
					BulletManager::Instance()->InstaneEffect(_rigidlist.obj->transform->position, Vector3(40, 40, 0));
					BulletManager::Instance()->SetHitFlg(true);
					PlaySound(SOUND_LABEL_SE000);
				}
				else {
					LowerLife(5);

					// エフェクト
					BulletManager::Instance()->InstaneEffect(_rigidlist.obj->transform->position, Vector3(50, 50, 0));
					BulletManager::Instance()->SetHitFlg(true);
				}
			}
		}
	}
}

// 状態の更新
void Enemy::StatusUpdate()
{
	switch (status)
	{
	case Enemy::LIVE:
		// 当たり判定
		Collision();
		
		// 煙
		SmogFly();

		// ライフ
		LifeCheck();
		break;

	case Enemy::DEAD:
		// 死亡時の更新
		DeadUpdate();
		break;
	default:
		break;
	}
}

// 死亡の処理
void Enemy::DeadUpdate()
{
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		if (m_nowdeadcnt == 0) {
			// フレーム削除　爆発発生　
			bomb->GetOwner()->SetExistState(true);
			frame->GetOwner()->SetExistState(false);
			lockui->SetAlpha(0);
		}
		m_nowdeadcnt++;
		DeadFly(tra->forward());
	}
	else {
		owner->SetExistState(false);
	}
	// 爆発の場所の更新
	bomb->GetOwner()->transform->position = owner->transform->position - owner->transform->forward() * 2;
}

// 自動移動
void Enemy::AutoGo(XMFLOAT4 _axisZ)
{
	// 移動の制限
	if (straightSpeed < m_minSpeed)
	{
		straightSpeed = m_minSpeed;
	}
	else if (straightSpeed > m_maxSpeed) {
		straightSpeed = m_maxSpeed;
	}

	// 移動
	if (0 < straightSpeed)
	{
		tra->position.x += _axisZ.x * straightSpeed;
		tra->position.y += _axisZ.y * straightSpeed;
		tra->position.z += _axisZ.z * straightSpeed;
	}
	else
	{
		tra->position.x += _axisZ.x * straightSpeed / 5;
		tra->position.y += _axisZ.y * straightSpeed / 5;
		tra->position.z += _axisZ.z * straightSpeed / 5;
	}
}

// 前との体力比較と死亡処理
void Enemy::LifeCheck()
{
	if (life != oldlife) {
		oldlife = life;
	}

	if (life <= 0)
	{
		status = Status::DEAD;
	}
}

// 死亡処理
void Enemy::DeadFly(XMFLOAT4 _axisZ)
{
	// 向いてる向きに落ちる
	straightSpeed = 1;		// 速度は現在のスピード
	tra->position.x += _axisZ.x * straightSpeed;
	tra->position.y += _axisZ.y * straightSpeed;
	tra->position.z += _axisZ.z * straightSpeed;

	tra->RotateXaxis(0.5f);

	const int hoge = 10;
	for (int i = 1; i < hoge; i++)
	{// 大きさ5で煙ぽかった
		smogmana->SetEnemyDeadSmog(tra->position - tra->forward() * (straightSpeed) * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
		smogmana->SetEnemyDeadSmog(tra->position - tra->forward() * (straightSpeed) * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
	}
}

// 飛んでる時の煙
void Enemy::SmogFly()
{
	if (m_maxsmogcnt < m_nowsmogcnt)
	{
		//一定カウントごとに生成
		// 大きさ5で煙ぽかった
		m_nowsmogcnt = 0;

		smogmana->SetEnemySmog(tra->position + tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
		smogmana->SetEnemySmog(tra->position - tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
	}
	else {
		m_nowsmogcnt++;
	}
}

// 画面外の処理
void Enemy::OutSide()
{
	// 範囲外
	if (tra->position.Distance(m_centerPos) > outsideDistance)
	{
		movedir = MoveDir::OUTSIDE;
		outsideFlg = true;
	}
	else {
		outsideCnt = 0;
		outsideFlg = false;
		RandMove();
	}
}

void Enemy::Atack()
{
	BulletManager::Instance()->Fire(tra->qt, tra->position, tra->up(), tra->right(), tra->forward(), &owner->GetTag());
}
