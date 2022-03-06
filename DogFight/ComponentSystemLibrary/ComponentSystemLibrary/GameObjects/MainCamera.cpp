#include "MainCamera.h"
#include "../Component/SceneManager.h"
#include "../Component/Scene.h"
#include "../DX11System/CDirectInput.h"
#include <array>

using namespace Component;

MainCamera::MainCamera(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("MainCamera");

	tra = owner->transform;
	tra->position = Vector3{ 0.0f,0.0f,0.0f };
	tra->scale = Vector3{ 0.0f,0.0f,0.0f };

	cam = owner->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	shake = owner->AddComponent<Component::CameraShakeComponent>();

	camtype = CameraType::DELAY;	// カメラの種類

	cam->SetFar(50000);

	m_mainPos = Vector3(0,5.5f,15);
}

MainCamera::~MainCamera() {

}

void MainCamera::Update() {

	CameraUpdate();
}

void MainCamera::CameraUpdate()
{

	// プレイヤーが存在するか？
	if (player != nullptr)
	{
		// プレイヤー更新
		playertra = player->transform;
	}

	// 状態変化
	Input();

	// 位置の更新
	CameraPos();

}

// 初期化
void MainCamera::Start()
{
	// カメラの位置をプレイヤーから取得
	player = SceneManager::Instance()->GameObjectFindName("player");
	playertra = player->transform;
	playercompo = player->GetComponent<Component::PlayerObj>();
	out= player->GetComponent<Component::OutSideControlComponent>();

}

// 位置の更新
void MainCamera::CameraPos()
{
	auto scene = SceneManager::Instance()->GetActiveScene()->GetSceneName();

	if (scene == "TitleScene") {
		FPSCamera();
	}
	else {
		switch (camtype)
		{
		case MainCamera::CameraType::FPS:
			if (scene == "TitleScene") {
				FPSCamera();
			}
			break;

		case MainCamera::CameraType::DELAY:

			DelayCameraUpdate();
			break;

		default:
			break;
		}
	}

	cam->CreateProjectionMatrix();
	cam->CreateCameraMatrix();
}
void MainCamera::FPSCamera()
{}

// 遅れて追尾するカメラ
void MainCamera::DelayCameraUpdate()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::START)
	{
		StartCamera(112, 160, 1);
		return;
	}
	
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::CLEAR)
	{
		ClearCamera();
		return;
	}

	if (player->GetComponent<Component::PlayerObj>()->GetStatus() == Status::DEAD)
	{
		// 死亡時のカメラ
		DeadCamera();
		return;
	}
	
	// 前の意図情報を保存
	SetOldPos();

	// 注視
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_L) ||
		ControllerNum != -1 && Controller::GAME_PAD[ControllerNum].state.Gamepad.bLeftTrigger)
	{
		if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
		{
			LockOn();
		}
	}
	else {
		if (!outflg) {
			DelayCamera();
		}
		OutSideCamera();
	}
}

void MainCamera::OutSideCamera()
{
	if (out->GetOutFlg()) {
		// 画面外にいった判定
		cam->GetOwner()->transform->LookAt(player->transform->position, Vector3(0, 1, 0));
		tra->position.y = player->transform->position.y;
		outflg = true;
		stackOutPos = tra->position;

		return;
	}

	// 画面内で出た判定があり、一定カウント以内
	if (!outflg)
	{
		return;
	}

	if (m_outCnt < outMaxCnt)
	{
		m_outCnt++;
		float _cnt = m_outCnt / outMaxCnt;
		Vector3 _pos;
		_pos.x = (1.0f - _cnt) * stackOutPos.x + _cnt * player->transform->position.x;
		_pos.y = (1.0f - _cnt) * stackOutPos.y + _cnt * player->transform->position.y;
		_pos.z = (1.0f - _cnt) * stackOutPos.z + _cnt * player->transform->position.z;

		tra->position = _pos
			- forward[useidx] * m_mainPos.z + up[useidx] * (m_mainPos.y);

		cam->GetOwner()->transform->LookAt(player->transform->position + forward[useidx] * m_mainPos.z + up[useidx] * (m_mainPos.y), tra->up());
	}
	else {
		outflg = false;
		m_outCnt = 0;
	}
}

// 入力
void MainCamera::Input()
{
	if (ControllerNum == -1 && Controller::Instance()->CheckController())
	{
		ControllerNum = Controller::Instance()->GetControllerNum();
		Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	}	
}

// 角度をラジアンに変更
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}

// 球面束縛カメラ(Ｚ軸回転なしバージョン)
void MainCamera::UpdateSphereCamera(
	float radius,						// 半径(入力)
	float elevation,					// 仰角（入力）
	const Vector3 lookat,
	const int _pluscnt)	// 注視点（入力）
{
	Vector3 eye;							// 視点
	Vector3 _pos = Vector3(0, 5, 18);
	float _azimuth;							// 方位角（入力）

	m_startcnt += _pluscnt;
	azimuth -= (float)_pluscnt;

	// 仰角、方位角をラジアンに変換
	elevation = ToRad(elevation);
	_azimuth = ToRad(azimuth);

	// 仰角に応じた半径を計算
	float r = radius * sinf(elevation);

	// 視点座標を計算
	eye.x = r * cosf(_azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(_azimuth);

	// 視点ベクトルを注視点分平行移動
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	tra->position = eye + player->transform->up() * _pos;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());

	XMFLOAT4 _qt = tra->qt;

	tra->qt.x = 0;
}

// 遅れて追従するカメラ
void MainCamera::DelayCamera()
{

	// 基本の場所
	float length = 30;


	Vector3 inputVector;
	Vector3 _pos;

	// 
	inputVector.x = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX) / 362.0f;
	inputVector.y = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 362.0f;
	float yinputVector = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 182.0f;

	//inputVector.xをインとに直して掛けたり割ったりしてする
	float backx = inputVector.x;
	if (inputVector.x < 5 && -5 < inputVector.x) { inputVector.x = 0; }
	int xxx = inputVector.x;
	double radi = xxx * 3.141592653589793f / 360; _pos.x = length * sin(radi);

	if (inputVector.y < 5 && -5 < inputVector.y) { inputVector.y = 0; }
	radi = inputVector.y * 3.141592653589793f / 360;

	_pos.y = length * sin(radi);

	float firstleng = 1;			// 初期長さ
	const float confirstleng = 18;	// 固定の初期長さ

	if (inputVector.x == 0 && inputVector.y == 0)
	{
		firstleng = confirstleng;
	}

	float z = 0;
	float w = 0;

	// クオータニオンを入れる　
	cam->GetOwner()->transform->qt = qt[useidx];

	if (ControllerNum != -1 && (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// 左右入力に対するｚ座標の補正　
		if (_pos.x < 0)
		{
			z = _pos.x + 30;
		}
		else {
			z = 30 - _pos.x;
		}

		// 入力がないとき計算のバグをなくす
		if (_pos.x == 0)
		{
			_pos.x = 0.01f;
		}
		if (_pos.y == 0)
		{
			_pos.y = 0.01f;
		}

		//======== カメラの回転=========== //
		cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

		// 入力なしの初期値の時
		if (firstleng != confirstleng) {
			tra->position = stackpos[useidx]
				- forward[useidx] * _pos.y
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
		else {
			tra->position = stackpos[useidx]
				- forward[useidx] * firstleng
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
	}
	else {
		// 振動
		if (playercompo->GetStreatFlg())
		{
			//shake->ShakeCamera(0.05f);
		}
		if (playercompo->GetDownFlg())
		{
			//shake->ShakeCamera(0.05f);
		}

		auto _shakePos = shake->GetRandPos();

		// 速度に応じた倍率
		auto s = playercompo->GetSpeed();
		auto d = playercompo->GetMaxSpeed();
		float rate = s / d;

		// カメラのの振動を加えたカメラ位置
		Vector3 _pos = m_mainPos + shake->GetRandPos();

		_pos.z = _pos.z * (1.0f - rate);

		tra->position = stackpos[useidx]
			- forward[useidxQt] * _pos.z - forward[useidxQt] * 7
			+ up[useidxQt] * (_pos.y) + right[useidxQt] * _pos.x;

		// 視野角
		auto _fov = DirectX::XM_PI * 0.3f + DirectX::XM_PI  * (rate) * 0.2f;
		cam->SetFov(_fov);

		// 注視点
		cam->GetOwner()->transform->LookAt(player->transform->position + forward[useidx] * m_mainPos.z + up[useidx] * (m_mainPos.y), tra->up());
	}
}

// 敵を注視する
void MainCamera::LockOn()
{
	if (EnemyManager::Instance()->GetLookObj() == nullptr) {
		return;
	}

	Vector3 lookat = EnemyManager::Instance()->GetLookObj()->transform->position;	// 注視点（入力）

	// 視点ベクトルを注視点分平行移動
	tra->position = player->transform->position + player->transform->up() * 3;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());
}

// スタートの演出用
void MainCamera::StartCamera(int posecnt, int _cnt, int _pluscnt)
{
	Vector3 lookat = player->transform->position;	// 注視点（入力）
	static Vector3 _ppos = lookat;


	if (_cnt <= m_startcnt)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);

		for (int i = 0; i < arraySiza; i++)
		{
			SetOldPos();
		}
		return;
	}


	// 待機
	if (posecnt > m_posecnt)
	{
		StartLookCamera(Vector3(11, 5, _ppos.z + 210), m_posecnt);
		m_posecnt += _pluscnt;
	}
	// 回転
	else {
		UpdateSphereCamera(18, 90, lookat, _pluscnt);
	}


	// 開始演出スキップ
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) ||
		ControllerNum != -1 && Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
	}
}

// 開始時に機体を見る演出用
void MainCamera::StartLookCamera(Vector3 _pos, int time)
{
	auto hoge = player->transform->position;
	owner->transform->position = Vector3(_pos.x, hoge.y + _pos.y, _pos.z);
	cam->GetOwner()->transform->LookAt(player->transform->position, player->transform->up());
}

// 位置情報とカウントの更新
void MainCamera::SetOldPos()
{
	qt[svidxQt] = player->transform->qt;
	forward[svidxQt] = player->transform->forward();
	up[svidxQt] = player->transform->up();
	right[svidxQt] = player->transform->right();
	stackpos[svidx] = player->transform->position;
	svidx++;
	svidxQt++;

	// 初期化
	if (svidx > arraySiza - 1)
	{
		ResetSivCnt();
		firstflag = true;
	}
	// 30フレーム前の情報を使用してカメラを作成
	useidx = svidx - _delay;
	if (useidx < 0)
	{
		// 初期化
		if (!firstflag)
		{
			useidx = 0;
		}// 通常
		else
		{
			useidx = svidx + arraySiza - _delay;
		}
	}

	// 初期化
	if (svidxQt > arraySiza - 1)
	{
		ResetSivQtCnt();
		firstflag = true;
	}
	// 30フレーム前の情報を使用してカメラを作成
	useidxQt = svidxQt - _delayQt;
	if (useidxQt < 0)
	{
		// 初期化
		if (!firstflag)
		{
			useidxQt = 0;
		}// 通常
		else
		{
			useidxQt = svidxQt + arraySiza - _delayQt;
		}
	}
}

// 死亡時用
void  MainCamera::DeadCamera()
{
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());
}

// クリア時のカメラ
void  MainCamera::ClearCamera()
{
	auto _ene = EnemyManager::Instance()->GetDeadObj();
	if (_ene == nullptr) {
		return;
	}

	// カメラの位置と注視点の設定
	m_clearEnemyPos = _ene->transform->position;
	owner->transform->position = m_clearEnemyPos + Vector3(100, 0, 0);
	cam->GetOwner()->transform->LookAt(m_clearEnemyPos, owner->transform->up());
}
