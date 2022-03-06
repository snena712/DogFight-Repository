#pragma once
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../Compute/EffectManager.h"
#include "../UI/Enemy/EnemyPosFrame.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../Effect/Bomb/BombAnim.h"
#include "../UI/Window/LockOn/LockOnUI.h"
#include "../UI/String/LockOnString.h"
#include "../Bullet/BulletManager.h"
#include <random>

namespace Component {
	class Enemy :public ComponentBase
	{
	public:
		// 移動
		enum class MoveDir
		{
			UP,
			DOWN,
			RIGHT,
			LEFT,
			STRREAT,
			SLOW,
			CHASE,
			TURUN,
			STOP,
			OUTSIDE
		};

		// 回転
		enum class Spin
		{
			Right,
			Left,
			None,
		};
		// 状態
		enum Status
		{
			LIVE,
			DEAD
		};

	private:
		MoveDir movedir;
		Spin spin;
		Status status;
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::SphereCollider* col;
		Component::SmogManager* smogmana;
		Component::EnemyPosFrame* posFrameCom;
		EffectManager* efemana;
		Component::LockOnString* lockStr;
		Component::LockOnUI* lockui;
	
		Component::EnemyPosFrame *frame;		// 枠
		Component::BombAnim *bomb;				// 爆発
	
		float life = 20;						// ライフ
		float oldlife = life;					// 前フレームのライフ

		int m_nowdeadcnt = 0;					// 現在の死亡カウント
		const int m_maxdeadcnt = 60;			// 死亡時の最大カウント
	
		//-----移動----
		const float speed = 0.01;				// 移動速度
		const float Lside = 3;					// 左スティックのXの移動制限
		const float m_maxSpeed = 3;				// 移動最大値
		const float m_minSpeed = 2.5f;			// 移動最小値

		//-----移動----//
		int m_movecnt = 60;						// 移動用のカウント
		int m_nowmovecnt;						// 現在のカウント
		//const float speed = 0.00;				// 移動速度
		//const float Lside = 0;				// 左スティックのXの移動制限
		//const float m_maxSpeed = 0;			// 移動最大値
		//const float m_minSpeed = 0.00f;		// 移動最小値
		int m_spincnt = 60;						// 移動用のカウント
		int m_nowspincnt;						// 現在のカウント
		float straightSpeed = m_maxSpeed / 2;	// 速度

		//-----回転と範囲外----//
		float outsideDistance = 800;			// 行動変化の距離（旋回になる）
		int turnCnt = 0;						// 回転のカウント
		int turnMaxCnt = 180;					// 回転のカウント最大
		int outsideCnt = 0;						// 画面外のカウント
		const int outsidePlusCnt = 1;			// カウントの増加
		bool outsideFlg= false;					// 旋回後の行動制限
		bool turnFlg= false;					// 旋回後の行動制限
		const Vector3 m_centerPos
			= Vector3(0, outsideDistance, 0);	// 移動の中心点

		int m_nowsmogcnt = 0;					// 煙の現在のカウント
		int m_maxsmogcnt = 1;					// 煙の現在の最大カウント

		Vector3 m_ScreenPos;					// スクリーン座標

		void MoveUpdate();						// 移動量の計算
		void AutoGo(XMFLOAT4 _axisZ);			// 移動
		void Collision();						// 当たり判定
		void StatusUpdate();					// 状態変更
		void RandMove();						// 状態をランダムに変更
		void SpinMove();						// 横の回転
		void LifeCheck();						// 体力の確認
		void DeadUpdate();						// 死亡処理
		void DeadFly(XMFLOAT4 _axisZ);			// 死亡時の飛行
		void SmogFly();							// 飛行時の雲
		void OutSide();							// 範囲外の更新
		void Atack();							// 攻撃
	public:
		Enemy(GameObject *_owner);
		~Enemy();

		void Start();
		void Update();
		void DrawObject();

		void LowerLife(int _num) { life -= _num; }	// 体力の減少
		bool GetStatus() { if (status == LIVE)		// 生きているときtrue返す
		{ return true; } else { return false; } }

	
		// ターゲットのスクリーン座標
		void SetScreenPos(Vector3 _pos) { m_ScreenPos = _pos; }
		Vector3 GetScreenPos() { return m_ScreenPos; }
	};

}