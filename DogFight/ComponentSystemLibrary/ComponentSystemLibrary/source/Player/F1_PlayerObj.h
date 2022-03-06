#pragma once

#include "../../Component\RigidbodyComponent.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../source/ObjectShape/Cone.h"
#include "../../DX11System/RandomClass.h"
#include "../../DX11System\quad.h"
#include "../../DX11System\CDirectxGraphics.h"
#include "../../DX11System\Shader.h"
#include "../../DX11System/CDirectInput.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SphereColliderComponent.h"
#include "../../Component/BoxColliderComponent.h"
#include "XAudio2.h"
#include "../../DX11System/XAudio2.h"
#include "../../Component\ImGuiComponent.h"
#include "../../Component/SceneManager.h"
#include "../drawaxis.h"
#include "../../DX11System/Controller.h"
#include "../../DX11System/mystring.h"
#include "../../DX11System/XAudio2.h"
#include "../MainGameManager.h"
#include "../Bullet/BulletManager.h"
#include "../Smog/SmogManager.h"
#include "../Effect/Bomb/BombAnim.h"
#include "../Player/PlayerWingParent.h"
#include "../Player/PlayerJatPartsParent.h"
#include "../Move/FallObjectComponent.h"
#include "../Move/OutSideControlComponent.h"
#include "../UI/Window/EnemyRoot/EnemyRoot.h"
#include "../Bullet/FireBulletPlace.h"
#include "../Stage/StageCollider.h"
#include "../../Config.h"
#include "../Effect/Splashes/SplashesEfect.h"

enum  Status
{
	LIVE,
	DEAD
};

namespace Component {
	class PlayerObj : public ComponentBase
	{

	private:
		//----コンポーネント------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		GameObject* bomb;
		Component::FallObjectComponent* fall ;
		Component::OutSideControlComponent* out;
		GameObject* root;
		GameObject* splashes;
		Component::ComputeComponent* efemana;
		Component::SmogManager* smog;
		RandomClass* randam;
		Component::PlayerWingParent*pwing;
		Component::PlayerJatPartsParent* pjet;
		Component::FireBulletPlace* bulletRPlace;
		Component::FireBulletPlace* bulletLPlace;
		GameObject* bulletRPlaceObj;
		GameObject* bulletLPlaceObj;

		int ControllerNum = -1;						// コントローラー

		Status m_status = Status::LIVE;				// 状態
		int m_nowdeadcnt = 0;						// 死亡時のカウント
		const int m_maxdeadcnt = 60;				// 死亡時の最大カウント

		//----角度----//
		DirectX::XMFLOAT4X4 g_mtxframe;				// 今回のフレームでの回転位置を表す行列
		DirectX::XMFLOAT3   g_angle = { 0,0,0 };	// 回転角度

		// 移動
		const float speed = 0.016;					// 移動速度
		const float Lside = 2;						// 左スティックのXの移動制限
		const float m_maxstraightStackSpeed = 0.5f;	// 移動最大値
		const float m_maxSpeed = 5.0f;				// 移動最大値
		//const float m_minSpeed = 1.0f;			// 移動最小値
		const float m_minSpeed = 0;					// 移動最小値
		float straightSpeed = m_minSpeed+2;			// 現在の速度
		float straightStackSpeed;					// 現在の速度
		float m_oldspeed = 0;						// 前の速度
		const float m_compareSpeed = 0.005f;		// 振動用の加速地の固定値

		// 回転
		Vector3 m_stackRot;							// 回転時の移動量保存
		const float deadZone = 20;					// 入力の入らない範囲
		const float m_maxRot = 55;					// 移動最大値
		const float m_minRot = 0;					// 移動最小値
		const float m_decay = 1;					// 減衰量()

		// 位置
		DirectX::XMFLOAT3 g_startpos;				// キー入力された際の開始位置
		
		// 速度の変数
		bool m_streatflg = false;					// 前に加速しているか
		bool m_downflg = false;						// 減速したか
		float m_speedShakeNowCnt;					// 速度の現在カウント
		float m_speedShakeMaxCnt=10;				// 速度の最大カウント

		const float winglInstValue = 0.5f;				// 煙を生成する
		float m_perimetrySmogCnt = 0;					// 加速しないときのカウント
		const float m_perimetrySmogMaxCnt = m_maxSpeed;	// 加速しないときの最大カウント


		//-------関数--------//
		void Collision();						// 当たり判定 
		void Keybord();							// 回転
		void Controller();						// コントローラー
		void DrawSpeed();						// スピードの取得
		void AutoGo(XMFLOAT4 _axisZ);			// 移動
		void RotUpdate();						// 回転の計算
		void SmogInstanse();					// 主な煙生成
		void InstancePerimetrySmog(bool _flg);	// 飛行時の周りの雲
		void SpeedControllerShake();			// 速度に応じた振動
		void DeadUpdate();						// 死亡時の更新
		void DeadFly(XMFLOAT4 _axisZ);			// 死亡時の自動移動

		// フロートの値が一定いないであるかを判断
		bool floatcheck(float _checkvalue, float _value, float _minimum);	

		void BulletEffect();					// 発射エフェクト
	public:
		PlayerObj(GameObject* _obj);
		~PlayerObj();

		//----基本関数----//
		void Start();
		void Update();
		void DrawObject();

		float GetSpeed() { return straightSpeed; }				// 速度取得
		float GetMaxSpeed() { return m_maxSpeed; }				// 最大速度取得

		bool GetStreatFlg() { return m_streatflg; }				// 加速したかの判定
		bool GetDownFlg() { return m_downflg; }					// 減速したかの判定

		Status GetStatus() { return m_status; }					// 状態取得
		void SetStatus(Status _status) { m_status = _status; }	// 状態のセット

	};
}