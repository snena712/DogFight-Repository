#include "BulletManager.h"
#include "../InOutCheck/SimpleMath.h"
#include "../InOutCheck/insightcheck.h"
using namespace Component;

// 初期化はシーンで行う
void BulletManager::Init()
{   
	m_bulletlist.clear();
	m_hominglist.clear();

    std::vector<GameObject*> _list;

	// 弾初期化
    for (int i = 0; i < m_bulletnum; i++)
    {
        GameObject* _bullet = new GameObject();
        _bullet->transform->position.x = 10000;
		_bullet->AddComponent<Component::Bullet>();
        m_bulletlist.emplace_back(_bullet);
        _list.emplace_back(_bullet);
        _bullet->SetExistState(false);
    }


	// ホーミング初期化
    for (int i = 0; i < m_homingnum; i++)
    {
        GameObject* _bullet = new GameObject();
        _bullet->transform->position.x = 10000;
        _bullet->AddComponent<Component::HommingBullet>();
        m_hominglist.emplace_back(_bullet);
        _list.emplace_back(_bullet);
        _bullet->SetExistState(false);
    }

    SceneManager::Instance()->AddList(&_list);

	// 発射光初期化
	flash = new GameObject();
    flash->AddComponent<Component::BulletFlash>();
	SceneManager::Instance()->AddObject(flash, false);
	flash->SetExistState(false);


	// エフェクト用
	m_effectmana = new GameObject();
	m_effectmana->AddComponent<Component::BulletEffectManager>();
	SceneManager::Instance()->AddObject(m_effectmana, false);
}

// 削除作成
BulletManager::~BulletManager()
{
}

void BulletManager::Reset()
{
	// 各種変数の初期化
	initFlg = false;
	m_bulletlist.clear();
	m_hominglist.clear();
}


void BulletManager::Start()
{
	if (!initFlg)
	{
		auto hoge = SceneManager::Instance()->GameObjectFindName("SmogManager");
		if (hoge == NULL)return;

		// スモッグのマネージャー設定
		SmogManager* smog = SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();
		for (auto list : m_hominglist)
		{
			list->GetComponent<Component::HommingBullet>()->SetSmogMana(smog);
		}

		window = SceneManager::Instance()->GameObjectFindName("WindowUI");
		initFlg = true;
	}
}

void BulletManager::Update()
{
	Start();

	if (initFlg) {
		BulletCntUpdate();
		GetDegree(_player,NULL);
		HitCheck();
	}
}

// 通常弾
void BulletManager::Fire(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str)
{
	switch (bullettype)
	{
	case BulletManager::BulletType::Bullet:
		FireBullet(_qt, _pos, _right, _up, _forward, _str);
		break;
	case BulletManager::BulletType::Homing:
		FireHoming(_qt, _pos, _right, _up, _forward);
		break;
	default:
		break;
	}
}

// 通常の弾
void BulletManager::FireBullet(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str)
{
	if (bullettype == BulletType::Bullet&&m_nowbulletcnt > m_bulletcnt) {
		// カウントリセット
		m_nowbulletcnt = 0;

		// 振動
		Controller::Instance()->SetShakeController(0.6f, 1, 0.6f, 1);

		// 探索
		for (auto list : m_bulletlist) {

			if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead
				&& !list->GetExistState())
			{
				list->SetExistState(true);

				list->GetComponent<Component::BulletBase>()->SetStatus(Component::BulletBase::Status::Live);


				// 右
				if (m_bulletAllCnt % 2 == 0) {
					list->transform->position = _pos + _right * 10 - _up * 3 + _forward * 8;

					// 発射光
					flash->GetComponent<Component::BulletFlash>()->SetInitialPosition(_pos + _right * 8 + _forward * 3 - _up);
					flash->GetComponent<Component::BulletFlash>()->SetLife(1);
				}// 左
				else {

					list->transform->position = _pos - _right * 10 - _up * 3 + _forward * 8;

					// 発射光
					flash->GetComponent<Component::BulletFlash>()->SetInitialPosition(_pos - _right * 8 + _forward * 3 - _up);
					flash->GetComponent<Component::BulletFlash>()->SetLife(1);
				}
				m_bulletAllCnt++;

				// 初期化関数作るべき
				list->transform->qt = _qt;
				list->GetComponent<Component::BulletBase>()->SetLife(60);
				PlaySound(SOUND_LABEL_SE000);

				break;
			}
		}
	}
}

// 追尾弾
void BulletManager::FireHoming(XMFLOAT4 _qt, Vector3 _position, Vector3 _right, Vector3 _up, Vector3 _forward)
{
    // シーンマネージャーリストに追加
    // 初期化
    if (bullettype == BulletType::Homing) {
        if (m_nowhomingcnt > m_maxhomingtcnt) {

			// カウントリセット
			m_nowhomingcnt = 0;
            
			XMFLOAT4 _qtcopy = _qt;
            Vector3 _pos = _position;
         
			// 振動
			Controller::Instance()->SetShakeController(1, 2, 1, 2);

            for (auto list : m_hominglist)
            {
				if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead
					&& !list->GetExistState())
				{
					list->SetExistState(true);
					// 発射の設定
					list->GetComponent<Component::BulletBase>()->SetStatus(Component::BulletBase::Status::Live);

					if (m_bulletAllCnt % 2 == 0) {
						list->transform->position = _pos + _right * 10 - _up * 3 + _forward * 8;
						// list->transform->position = _pos - _tra->up() * 10;
					}
					else {

						list->transform->position = _pos - _right * 10 - _up * 3 + _forward * 8;
					}
					m_bulletAllCnt++;

					list->transform->qt = _qtcopy;

					// 追跡する敵
					Vector3 _tpos;
					GameObject* _stack = nullptr;

					// ホーミング可能
					if (stackEnemy != nullptr && m_lockonOk) {
						_tpos = stackEnemy->transform->position;
						_stack = stackEnemy;
					}

					PlaySound(SOUND_BULLET_SE002);
					list->GetComponent<Component::HommingBullet>()->SetTargetObj(_stack);
					list->GetComponent<Component::BulletBase>()->SetLife(150);
					//list->GetComponent<Component::BulletBase>()->SetSpeed(_player->GetSpeed() / 2);

					// 初速の設定
					list->GetComponent<Component::BulletBase>()->SetSpeed(2);

					break;
				}
            }                                   

            m_homingnum = 0;
            for (auto list : m_hominglist)
            {
                if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead)
                {
                    m_homingnum++;
                }
            }
            // 更新掛けているときにオブジェクトのリスと増やすとバグった
        }   
    }
}

// 弾のカウント更新
void BulletManager::BulletCntUpdate()
{
	// 通常
	if (m_nowbulletcnt <= m_maxhomingtcnt) {
		m_nowbulletcnt++;
	}

	// ホーミング
	if (m_nowhomingcnt <= m_maxhomingtcnt) {
		m_nowhomingcnt++;
	}
}

// XZ軸のプレイヤーと敵の角度取得
Vector3 BulletManager::GetDegree(GameObject* _a, GameObject* _b)
{
    const float M_PI = 3.14f;

    Vector3 m_position = _a->transform->position;	//プレイヤーの座標

    //計算して出た暫定的に一番小さい角度を記憶する変数
    float degreemum = 3.14f * 2;
    double degreeHei;
    double degreeVer;
	float _stackdis = 1000;

    auto m_enemyList = EnemyManager::Instance()->GetObjectList();
	auto _camera = Camera::GetMainCamera()->GetOwner();

	bool lock = false;
	bool _lockClear = true;
	static bool _sound = false;
    for (auto* _enemy : m_enemyList)
    {
		if (_enemy->GetComponent<Component::Enemy>()->GetStatus())
		{


			SimpleMath::Vector3 _campos;
			_campos.x = _camera->transform->position.x;
			_campos.y = _camera->transform->position.y;
			_campos.z = _camera->transform->position.z;

			//SimpleMath::Vector3 _lookat;
			//auto _forward = _camera->transform->forward();
			//_lookat.x = _forward.x;
			//_lookat.y = _forward.y;
			//_lookat.z = _forward.z;

			SimpleMath::Vector3 _enemyPos;
			_enemyPos.x = _enemy->transform->position.x;
			_enemyPos.y = _enemy->transform->position.y;
			_enemyPos.z = _enemy->transform->position.z;

			Vector3 _lookat = _a->transform->position + _a->transform->forward() * 100;
			SimpleMath::Vector3 rotlookat;
			rotlookat.x = _lookat.x;
			rotlookat.y = _lookat.y;
			rotlookat.z = _lookat.z;

			static int hogent = 0;
			// ２点間の距離
			if (InsightCheckXZ(
				_campos,
				rotlookat,
				Camera::GetMainCamera()->GetFov(),                          // FOV
				_enemyPos,                     // check point
				_stackdis))                                       // view length
			{
				printf("はい%d\n", hogent);
				hogent++;

				// 距離の更新
				_stackdis = _a->transform->position.Distance(_enemy->transform->position);
				stackEnemy = _enemy;
				_lockClear = false;
			}
		}
    }

	if (_lockClear)
	{
		stackEnemy = nullptr;
		if (!_sound)
		{
			_sound = true;
			StopSound(SOUND_BULLET_SE004);
			StopSound(SOUND_BULLET_SE005);
		}
	}

	if (BulletManager::Instance()->bullettype == BulletManager::BulletType::Homing)
	{	
		// ロックオンの状態保持
		if (stackEnemy != nullptr) {
			
			
			// ターゲットが同じとき
			if (m_oldStackEnemy == stackEnemy) {

				stackEnemy->GetComponent<Component::LockOnUI>()->SetLockOnCnt(m_lockoncnt);
				stackEnemy->GetComponent < Component::LockOnUI >()->SetLoclOnFlg(true);
				// カウントの更新
				if (LOCKONMAX > m_lockoncnt)
				{
					m_lockoncnt++;

					// ロックオン準備音
					if (!CheckPlaying(SOUND_BULLET_SE004))
					{
						PlaySound(SOUND_BULLET_SE004);
					}
				}
				else
				{
					// 追尾可能
					m_lockonOk = true;
					stackEnemy->GetComponent < Component::LockOnString >()->SetlockOnFlg(true);

					// ロックオン準備音の停止とロックオン音
					StopSound(SOUND_BULLET_SE004);
					if (!CheckPlaying(SOUND_BULLET_SE005))
					{
						PlaySound(SOUND_BULLET_SE005);
						_sound = false;
					}
				}
			}
			else {

				ClearLockOn();

				// ターゲットが異なるとき
				m_oldStackEnemy = stackEnemy;
				m_lockoncnt = 0;
			}
		}// ターゲットがないとき
		else
		{
			ClearLockOn();
		}
	}
	else {
		ClearLockOn();
	}
	

    // 要修正
    return Vector3(0, 0, 0);
}

// 弾変更
void BulletManager::ClearLockOn()
{
	// 初期化
	if (m_oldStackEnemy != nullptr)
	{
		m_lockoncnt = 0;
		// uiのコンポーネントの表示消す
		m_oldStackEnemy->GetComponent < Component::LockOnUI >()->SetLoclOnFlg(false);
		m_oldStackEnemy->GetComponent < Component::LockOnString >()->SetlockOnFlg(false);
		m_oldStackEnemy = nullptr;
		m_lockonOk = false;
	}
}


void BulletManager::BulletChange()
{
    switch (bullettype)
    {
    case BulletManager::BulletType::Bullet:
        bullettype = BulletType::Homing;
		break;
    case BulletManager::BulletType::Homing:
        bullettype = BulletType::Bullet;
        break;

    default:
        break;
    }
}

// 当たったかの判定
void BulletManager::HitCheck()
{
	m_hitflg = false;
}

// エフェクト生成
void BulletManager::InstaneEffect(Vector3 _pos, Vector3 _size)
{
	// エフェクト発生
	m_effectmana->GetComponent<Component::BulletEffectManager>()->Fire(_pos, _size);
}
