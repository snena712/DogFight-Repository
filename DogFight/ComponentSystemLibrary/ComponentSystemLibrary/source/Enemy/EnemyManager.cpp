#include "EnemyManager.h"


EnemyManager::~EnemyManager()
{
	m_initFlg = false;
}

void EnemyManager::Start()
{
	if (!m_initFlg) 
	{
		player = SceneManager::Instance()->GameObjectFindName("player"); if (player == NULL)return;
		EnemyDegreeUpdate();
		EnemyCameraLock();

		m_initFlg = true;
	}
}

void EnemyManager::Reset()
{
	// 各種変数初期化
	m_initFlg = false; 
	m_objlist.clear();
	m_lengthlist.clear();
	m_degreelist.clear();
}

void EnemyManager::Update()
{
	// 初期化
	Start();

	if (m_initFlg) {
		// 角度取得
		EnemyDegreeUpdate();

		// 生存数の比較
		CompareLiveCnt();

		// 死んだ敵はいるか
		DeadEnemyCheck();
	}

	m_deadTarget = m_objlist[0];
}

// 位置のリストにデータ入れる
void EnemyManager::SetList(std::vector<Vector3*> _pos)
{
	// 各種初期化
	m_objlist.clear();
	m_lengthlist.clear();
	m_degreelist.clear();

	for (auto hoge : _pos)
	{

		// リストに追加
		GameObject* ene = new GameObject();
		ene->AddComponent<Component::Enemy>();
		m_objlist.emplace_back(ene);

		// 状態
		ene->transform->position = *hoge;

		// 角度のリストの初期化
		m_degreelist.emplace_back(0);
		m_lengthlist.emplace_back(0, nullptr);

	}
	SceneManager::Instance()->AddList(&m_objlist);

	// サイズ取得
	m_enelistsize = m_objlist.size();
	m_startlivrcnt = m_enelistsize;
}

// 近い敵の更新
void EnemyManager::UpdateNearEnemy()
{
	if (m_nearcnt < m_liveCount - 1) {
		m_nearcnt++;
	}
	else {
		m_nearcnt = 0;
	}

	// 入力があったときのみ実行
	EnemyCameraLock();
}

// 全敵の角度更新
void EnemyManager::EnemyDegreeUpdate()
{
	_num = 0;
	m_liveCount = 0;

	// プレイヤーとエネミーを結ぶベクトルを出します
	for (auto hoge : m_objlist)
	{
		SerchDegree(hoge);
	}
}


// 角度取得
void EnemyManager::SerchDegree(GameObject* _obj)
{

	if (_obj->GetComponent<Component::Enemy>()->GetStatus() == true)
	{
		// 生きているとき
		m_liveCount++;

		// ２点間の距離
		Vector3 pos2 = _obj->transform->position - player->transform->position;

		// プレイヤーの向いている角度
		double degreep = atan2(player->transform->forward().x, player->transform->forward().z);

		//ベクトルを正規化します
		pos2.y = 0;
		pos2.Normalize();
		//プレイヤーとエネミーを結ぶベクトルの角度を計算します
		double degree;

		degree = atan2(pos2.x, pos2.z);

		degree = degree - degreep;

		m_degreelist[_num] = degree * 180 / 3.141592653589793f + 180;

		// に転換の距離
		float pos = player->transform->position.Distance(_obj->transform->position);

		if (pos < m_lengthmax) {
			// 距離
			m_lengthlist[_num] = std::make_pair(pos, _obj);
		}
		else {
			m_lengthlist[_num] = std::make_pair(m_lengthmax, _obj);
		}

	}
	else {
		// 死んでいる
		m_lengthlist[_num] = std::make_pair(20000, _obj);

	}

	// リストに保存するための変数
	_num++;
}

// カメラのターゲット取得関数
void EnemyManager::EnemyCameraLock()
{
	std::sort(m_lengthlist.begin(), m_lengthlist.end());//昇順ソート

	for (int i = m_nearcnt; i >= 0; i--)
	{
		if (m_lengthlist[m_nearcnt].first < m_maxdis
			&& m_lengthlist[m_nearcnt].second->GetComponent<Component::Enemy>()->GetStatus())
		{
			m_target = m_lengthlist[m_nearcnt].second;
			break;
		}
	}
}


// 死んだ敵はいるか？
void EnemyManager::DeadEnemyCheck()
{
	for (auto list : m_objlist)
	{
		if (list->GetExistState())
		{
			if (list->GetComponent<Component::Enemy>()->GetStatus()==false)
			{
				m_deadFlg = true;
				m_deadTarget = list;
				EnemyCameraLock();
				return;
			}
		}
	}	
}

// 生存数の比較
void EnemyManager::CompareLiveCnt()
{
	// 倒した数の取得
	m_deadcount = m_startlivrcnt - m_liveCount;

	// 倒した敵と初期値が同じとき
	if(m_startlivrcnt==m_deadcount)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::CLEAR);
	}
}
