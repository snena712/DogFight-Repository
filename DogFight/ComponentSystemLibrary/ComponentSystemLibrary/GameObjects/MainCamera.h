#pragma once
#include "../Component/GameObject.h"
#include "../Component\CameraComponent.h"
#include "../DX11System/RandomClass.h"
#include "../source/Player/F1_PlayerObj.h"
#include "../source/MainGameManager.h"
#include "../source/Camera/CameraShakeComponent.h"

#define arraySiza (200)

namespace Component {

	class MainCamera : public ComponentBase
	{
	public:
		enum class CameraType {
			FPS,
			TPS,
			DELAY,
			STATIC
		};

	private:
		Transform* tra;
		Component::Camera* cam;
		Component::PlayerObj* playercompo;
		Component::OutSideControlComponent* out;
		Component::CameraShakeComponent* shake;
		CameraType camtype = CameraType::TPS;	// カメラの種類

		GameObject* player;				// プレイヤーオブジェクト
		Transform*  playertra;			// プレイヤーのトランスフォーム

		int ControllerNum = -1;			// コントローラー番号
		WORD Buttons = -1;				// ボタンの詳細
		Vector3 m_mainPos;				// 場所のメイン

		//---------スタート用---------//
		int m_startcnt = 0;				// 開始用カウント
		int m_posecnt = 0;				// 開始時注視カウント
		float azimuth = 70;				// 開始角度
		bool firstflag = false;			// 初期化

		//-----------範囲外のカメラ-------------//
		float m_outCnt = 0;				// 範囲外時の管理カウント
		const float outMaxCnt = 60;		// 範囲外時の管理最大カウント
		Vector3 stackOutPos;			// 範囲外に行った時の場所保存
		bool outflg = false;
		
		//--------加速時用変数---------//
		int svidx = 0;					// カメラ位置で保存で使用するカウント
		int useidx = 0;					// カメラ位置で関数で使用するカウント
		int svidxQt = 0;				// カメラ回転で保存で使用するカウント
		int useidxQt = 0;				// カメラ回転で関数で使用するカウント

		const int _delay = 4;			// カメラ位置の遅れ
		const int _delayQt = 10;		// カメラの回転の遅れ

		// 加速管理用配列
		XMFLOAT4  qt[arraySiza];
		Vector3 forward[arraySiza];
		Vector3 up[arraySiza];
		Vector3 right[arraySiza];
		Vector3 stackpos[arraySiza];

		//-----------クリアカメラ-------------//
		Vector3 m_clearEnemyPos;		// 敵の位置
		Vector3 m_clearCameraPos;		// 敵の位置

		//-----------ロックオン用-------------//
		XMFLOAT4 m_lookRoot;			// ロックオン

		void CameraPos();				// 位置の更新
		void Input();					// 入力

		void UpdateSphereCamera(
			float radius,						// 半径(入力)
			float elevation,					// 仰角（入力）
			const Vector3 lookat,				// 注視点（入力）
			const int _pluscnt);				// カウント				

		void FPSCamera();
		void DelayCamera();											// 遅れて追従するカメラ
		void DelayCameraUpdate();									// 追従するカメラの更新
		void LockOn();												// ロックオン
		void StartCamera(int posecnt, int _cnt, int _pluscnt);		// 開始時演出用
		void StartLookCamera(Vector3 _pos, int time);				// 開始時演出用
		void DeadCamera();											// 死亡時用
		void SetOldPos();											// 前の位置の保存
		void ClearCamera();											// クリア時のカメラ
		void OutSideCamera();										// 画面外の処理
		
		void ResetSivCnt() { if (svidx > arraySiza - 1)svidx = 0; }
		void ResetSivQtCnt() { if (svidxQt > arraySiza - 1)svidxQt = 0; }

	public:
		MainCamera(GameObject* _obj);
		~MainCamera();

		void CameraUpdate();

		void Update();
		void Start();

		XMFLOAT4 GetLookRoot() { return m_lookRoot; }				// 注視の位置

	};
}
