#pragma once
#include "Bullet.h"
#include "HommigBullet.h"
#include "BulletFlash.h"
#include "../Smog/SmogManager.h"
#include "../Enemy\Enemy.h"
#include "../Enemy\EnemyManager.h"
#include "../../Component\SceneManager.h"
#include "../Effect/BulletEffect/BulletEffectManager.h"
#include "../../DX11System/Controller.h"
#include "../../DX11System/XAudio2.h"

constexpr int  LOCKONMAX = 12;	// ロックオン最大の状態

class BulletManager
{
public:
	// 弾の種類
	enum class BulletType
	{
		Bullet,
		Homing
	};
	// タグの種類
	enum class TagType
	{
		Player,
		Enemy
	};

private:
	GameObject* _player;
	GameObject* window;
	GameObject* m_effectmana;


	bool initFlg = false;						// 初期化
	float pspeed = 0;							// プレイヤーのスピード

	BulletType bullettype = BulletType::Bullet;	// バレットの種類

	GameObject* stackEnemy = nullptr;			// 一時保存変数
	GameObject* m_oldStackEnemy = nullptr;		// 前フレームの敵を保持
	std::vector<GameObject*> m_bulletlist;		// 弾配列
	std::vector<GameObject*> m_hominglist;		// ホーミング配列
	GameObject* flash;							// 発射光

	int m_bulletnum = 20;						// 弾のカウント
	float m_nowbulletcnt = m_bulletcnt;			// 弾の発射カウント
	const float m_bulletcnt = 3;				// 弾の発射最大カウント
	int m_bulletAllCnt;							// 通常団の仕分け変数

	int m_homingnum = 5;						// ホーミングミサイルの数
	const float m_maxhomingtcnt = LOCKONMAX;	// ホーミングミサイル弾の発射カウント
	float m_nowhomingcnt = m_maxhomingtcnt;		// ホーミングミサイル弾の発射最大カウント

	//---ロックオン関連---
	int m_lockoncnt = 0;						// ロックオンの
	bool m_lockonOk = false;					// ロックオンの発射可能
	bool m_hitflg = false;						// 当たったかのフラグ

	void HitCheck();									// 弾が当たったかの確認
	void ClearLockOn();									// ロックオンの対象をクリア
	Vector3 GetDegree(GameObject* _a,GameObject* _b);	// aとbの角度の取得
public:
	static BulletManager* Instance() {
		static BulletManager singleton;
		return &singleton;
	}

	BulletManager() {};
	~BulletManager();

	void Init();
	void Start();
	void Update();
	void Reset();

	// 発射関連
	void Fire(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str);
	void FireBullet(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str);
	void FireHoming(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward);
	
	GameObject* GetTargetObj() { return stackEnemy; }
	BulletType GetBulletType() { return bullettype; }		// 状態取得
	void BulletChange();									// 弾種の変更
	void BulletCntUpdate();									// 発射カウントの更新
	bool GetHitFlg() { return m_hitflg; }					// ヒット状態の取得
	void SetHitFlg(bool _flg) { m_hitflg = _flg; }			// ヒット状態のセット
	void InstaneEffect(Vector3 _pos, Vector3 _size);		// エフェクト生成
	bool GetLockOK() { return m_lockonOk; }					// ロックオンOK
	void SetPlayerObj(GameObject* _obj) { _player = _obj; }	// プレイヤーのセット
	void SetPlayerSpeed(float _num) { pspeed = _num; }		// プレイヤーの速度

	int GetHomigNum() {	return m_homingnum;}				// ホーミングの弾取得
	void HomingNumUP() { m_homingnum++; }					// ホーミングの弾増やす

	// 右の時true
	bool GetNormalBulletPos() {
		// 右
		if (m_bulletAllCnt % 2 == 1)
		{
			return true;
		}
		return false;
	}

	bool GetFire()
	{
		if (m_nowbulletcnt == 0)
		{
			return true;
		}
		return false;
	}

};
