#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "Enemy.h"
#include "../../Component/ComponentBaseClass.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/CDirectInput.h"
#include <utility>
#include "../MainGameManager.h"
#include "../../DX11System/Controller.h"


class EnemyManager
{
private:

	// リスト
	std::vector< GameObject*> m_objlist;						// 全敵のリスト
	std::vector< float> m_degreelist;							// 全敵の角度リスト
	std::vector<std::pair<float, GameObject*>> m_lengthlist;	// 全敵の長さリスト

	GameObject* player;

	// 変数
	int _num = 0;							// 敵のカウント
	int m_startlivrcnt = 0;					// 最初にいている数
	int m_liveCount = 0;					// 生存している敵の数
	int m_deadcount = 0;					// 倒した数
	float m_enelistsize;					// リストのサイズ
	const int m_lengthmax = 700;			// 距離最大値
	bool m_deadFlg = false;					// 死亡管理フラグ
	bool m_initFlg = false;					// 初期化検知

	const float m_maxdis = 10000;			// 対象範囲
	GameObject* m_target;					// ターゲット
	GameObject* m_deadTarget;				// ターゲット
	int m_nearcnt;							// 現在の何番目の近くを表すかのカウント
	const int m_maxnearcnt = 3;				// 何番目かの最大値

	// 関数
	void EnemyDegreeUpdate();				// 全敵の角度更新
	void SerchDegree(GameObject* _obj);		// 角度取得関数
	void EnemyCameraLock();					// カメラのターゲット取得関数
	void CompareLiveCnt();					// 生存数の比較
	void DeadEnemyCheck();					// 死亡した敵の確認
	
public:
	static EnemyManager* Instance() {
		static EnemyManager singleton;
		return &singleton;
	}
	EnemyManager() {};
	~EnemyManager();

	void Start();
	void Update();
	void Reset();

	void SetList(std::vector<Vector3*> _pos);						// 位置のリストにデータ入れる
	std::vector< GameObject*> GetObjectList() { return m_objlist; }	// エネミーリスト取得
	GameObject* GetEnemy(int _num) { return  m_objlist[_num]; }		// エネミーの取得
	std::vector< float> GetDegreeList() { return m_degreelist; }	// 角度リスト取得
	float GetLength(int _num) { return m_lengthlist[_num].first; }	// 距離リスト取得

	int GetLiveCount() { return m_liveCount; }						// 生存数取得	
	void LiveCountReduce() { m_liveCount--; };						// 生存数の減少		
	int GetDeadEnemyCnt() { return m_deadcount; }					// 倒した数

	bool GetDeadFlg() { return m_deadFlg; }							// 死んだ敵はいてるか
	void ResetDeadFlg() { m_deadFlg = false; }						// フラグのリセット

	GameObject* GetLookObj() { return m_target; }					// ロックオンするターゲット
	GameObject* GetDeadObj() { return m_deadTarget; }				// 死亡したオブジェクト

	void UpdateNearEnemy();											// 近い敵の更新
};

