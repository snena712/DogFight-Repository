#include "F1_PlayerObj.h"

using namespace Component;

PlayerObj::PlayerObj(GameObject *_owner) :ComponentBase(_owner)
{
	// 
	owner->SetName("player");
	owner->SetTag("Player");

	tra = owner->transform;

	tra->position = Vector3(0,200,-800);
	tra->scale = (Vector3{ 3,3,3 });

	// this->AddComponent<Component::RendererComponent>();

	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_game_v3.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/CookTorranceVS.hlsl",				// 頂点シェーダー
		"shader/CookTorrancePS.hlsl",				// ピクセルシェーダー
		"assets/F2_v2/f2a_texture/");				// テクスチャの存在フォルダ

	//model = this->AddComponent<Component::ModelComponent>(
	//	"assets/f1/f1.x",		// モデルデータ			// カレントファイルからの読み込み
	//	"shader/vs.hlsl",		// 頂点シェーダー
	//	"shader/ps.hlsl",		// ピクセルシェーダー
	//	"assets/f1/");			// テクスチャの存在フォルダ

	splashes = new GameObject();
	splashes->AddComponent<Component::SplashesEfect>();
	SceneManager::Instance()->AddObject(splashes, false);

	// 翼
	GameObject* wingparent = new GameObject();
	wingparent->transform->SetParent(owner->transform);
	pwing = wingparent->AddComponent<Component::PlayerWingParent>();
	wingparent->transform->position.z -= 2.1f;
	SceneManager::Instance()->AddObject(wingparent, false);

	// ジェットのパーツ（現状は別のもの）
	GameObject* jparts = new GameObject();
	jparts->transform->position.z -= 3.93f;// 0のとき
	jparts->transform->position.y -= 0.05f;
	pjet =jparts->AddComponent<Component::PlayerJatPartsParent>();
	SceneManager::Instance()->AddObject(jparts, false);

	// 落下用コンポーネント
	fall = owner->AddComponent<Component::FallObjectComponent>(0.5f, 2.0f);
	//外側
	out=owner->AddComponent<Component::OutSideControlComponent>();

	// 矢印
	//root = new GameObject();
	//root->AddComponent<Component::EnemyRoot>();
	//SceneManager::Instance()->AddObject(root, false);
	//root->SetExistState(false);

	// 火花の場所
	bulletRPlaceObj= new GameObject();
	bulletLPlaceObj= new GameObject();
	bulletRPlace=bulletRPlaceObj->AddComponent<Component::FireBulletPlace>();
	bulletLPlace =bulletLPlaceObj->AddComponent<Component::FireBulletPlace>();
	SceneManager::Instance()->AddObject(bulletRPlaceObj, false);
	SceneManager::Instance()->AddObject(bulletLPlaceObj, false);


	efemana =
	owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "assets/EfeFile/JetFire.EFE");

}

PlayerObj::~PlayerObj()
{
}

void  PlayerObj::Update()
{
	Vector3 _pos;

	switch (m_status)
	{
	case Status::LIVE:
		if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::AWAKE ||
			MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::START)
		{
			// 移動
			AutoGo(tra->forward());
			SpeedControllerShake();
		}

		if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::NORMAL)
		{
			break;
		}

		//root->SetExistState(true);

		Collision();


		// 外に行った時の処理
		if (out->GetOutFlg())
		{
			// 範囲外
			straightSpeed = 2.5f;
		}
		if (ControllerNum != -1)
		{
			Controller();
		}
		else {
			ControllerNum = Controller::Instance()->GetControllerNum();
			Keybord();
		}

		_pos = owner->transform->position;
		efemana->SetSpawnPos(_pos - tra->forward() * 7);

		// 移動
		fall->SpeedFall(straightSpeed);
		AutoGo(tra->forward());
		SpeedControllerShake();

		// 雲生成
		SmogInstanse();

		// パーティクル生成場所
		splashes->transform->position = tra->position + tra->forward() * 5;
		bulletRPlaceObj->transform->position = tra->position + tra->forward() * 6 + tra->right() * 8 - tra->up() * 1;
		bulletLPlaceObj->transform->position = tra->position + tra->forward() * 6 - tra->right() * 8 - tra->up() * 1;


		break;

	case Status::DEAD:
		DeadUpdate();
		break;
	default:
		break;
	}

	//　一部描画処理
	DrawSpeed();

	//// 縦横の線
	//drawaxis(g_mtx, 200, tra->position);
}

void PlayerObj::DrawObject()
{	
}


void PlayerObj::Start()
{
	smog = SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();

	Vector3 pos = owner->transform->position;
	
	BulletManager::Instance()->SetPlayerObj(owner);
}


void  PlayerObj::Keybord()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE)) {
		BulletManager::Instance()->SetPlayerSpeed(speed);
		BulletManager::Instance()->Fire(tra->qt, tra->position, tra->right(), tra->up(), tra->forward(), &owner->GetTag());
		BulletEffect();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LSHIFT)) {
		return;
	}
	// X軸の左右回転
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		m_stackRot.x += 2;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		m_stackRot.x -= 2;
	}

	// Y軸の左右回転
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		m_stackRot.y -= 2;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		m_stackRot.y += 2;
	}

	// Y軸の左右回転
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_Q)) {
		m_stackRot.z -= 2;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_E)) {
		m_stackRot.z += 2;
	}

	if (!Config::Instance()->GetReverseX())
	{
		tra->RotateLocalXaxis(0.03f * (-m_stackRot.x));
	}
	else {
		tra->RotateLocalXaxis(0.03f * (m_stackRot.x));
	}

	// 各軸の計算
	tra->RotateLocalZaxis(-0.03f * (m_stackRot.z));
	tra->RotateLocalYaxis(0.03f * (m_stackRot.y));

	// 回転に関する計算
	RotUpdate();

	// 速度
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
		if (straightStackSpeed < 0)
		{
			straightStackSpeed += speed * 2;
		}
		straightStackSpeed += speed;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
		if (0 < straightStackSpeed)
		{
			straightStackSpeed -= speed * 2;
		}
		straightStackSpeed -= speed;
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Y)) {
		BulletManager::Instance()->BulletChange();
	}

}

// コントローラー
void PlayerObj::Controller()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	Vector3 inputVector;

	//アナログ方向キー
	if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// 左
		inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLX / 360.0f;
		inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;

		// 真っすぐ移動
		if (inputVector.y > deadZone)
		{
			if (straightStackSpeed < 0)
			{
				straightStackSpeed += speed * (inputVector.y / 50);
			}
			else {
				straightStackSpeed += speed * (inputVector.y / 90);
			}
		}

		if (-deadZone > inputVector.y)
		{
			if (0 < straightStackSpeed)
			{
				straightStackSpeed += speed * (inputVector.y / 50);
			}
			else {
				straightStackSpeed += speed * (inputVector.y / 90);
			}
		}


		if (inputVector.x > deadZone || inputVector.x < -deadZone) {
			m_stackRot.y += (int)(inputVector.x) * 0.025f;
		}

		//----右----//
		if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX / 365.0f;
			inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY / 365.0f;
		}
	}

	// 回転処理
	// 入力増加処理
	if (inputVector.x > deadZone || inputVector.x < -deadZone) {
		m_stackRot.z += (int)(inputVector.x) * 0.025f;
	}
	if (inputVector.y > deadZone || inputVector.y < -deadZone) {
		m_stackRot.x += (int)(inputVector.y) * 0.025f;
	}

	// 回転に関する計算
	RotUpdate();

	// 上下回転
	tra->RotateLocalXaxis(0.03f * (m_stackRot.x));
	// 入力していない時の減衰処理
	tra->RotateLocalZaxis(-0.03f * (m_stackRot.z));
	// Y軸の左右回転
	tra->RotateLocalYaxis(0.03f * (m_stackRot.y));
	
	// R1
	if ((Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {

		//PlaySound(SOUND_LABEL_SE000);
		BulletManager::Instance()->SetPlayerSpeed(speed);
		BulletManager::Instance()->Fire(tra->qt, tra->position, tra->right(), tra->up(), tra->forward(), &owner->GetTag());
		BulletEffect();
	}

	if (Controller::Instance()->GetButtonYTrigger()) {
		BulletManager::Instance()->BulletChange();
	}
}

// 回転の更新
void PlayerObj::RotUpdate()
{
	// 減少処理
	if (m_stackRot.z > 0)
	{
		m_stackRot.z -= m_decay;
	}
	if (m_stackRot.z < 0)
	{
		m_stackRot.z += m_decay;
	}
	if (m_stackRot.x > 0)
	{
		m_stackRot.x -= m_decay;
	}
	if (m_stackRot.x < 0)
	{
		m_stackRot.x += m_decay;
	}
	if (m_stackRot.y > 0)
	{
		m_stackRot.y -= m_decay;
	}
	if (m_stackRot.y < 0)
	{
		m_stackRot.y += m_decay;
	}


	// X軸上限処理
	if (m_stackRot.x > m_maxRot)
	{
		m_stackRot.x = m_maxRot;
	}
	if (m_stackRot.x < -m_maxRot)
	{
		m_stackRot.x = -m_maxRot;
	}

	// Y軸上限処理
	if (m_stackRot.y > m_maxRot)
	{
		m_stackRot.y = m_maxRot;
	}
	if (m_stackRot.y < -m_maxRot)
	{
		m_stackRot.y = -m_maxRot;
	}

	// Z軸上限処理
	if (m_stackRot.z > m_maxRot)
	{
		m_stackRot.z = m_maxRot;
	}
	if (m_stackRot.z < -m_maxRot)
	{
		m_stackRot.z = -m_maxRot;
	}
}



void RotationChange(XMFLOAT4 _rot)
{
	if (_rot.x);
}


// フロートの値が一定いないであるかを判断
bool PlayerObj::floatcheck(float _checkvalue, float _value, float _minimum)
{
	if (fabsf(_checkvalue - _value) < _minimum)
	{
		return true;
	}

	return false;
}

// 自動移動
void PlayerObj::AutoGo(XMFLOAT4 _axisZ)
{
	straightSpeed += straightStackSpeed * 0.02f;

	// 加速度の更新
	if (straightStackSpeed > m_maxstraightStackSpeed)
	{
		straightStackSpeed = m_maxstraightStackSpeed;
	}	
	if (straightStackSpeed < -m_maxstraightStackSpeed)
	{
		straightStackSpeed = -m_maxstraightStackSpeed;
	}
	if (0 > straightStackSpeed)
	{
		straightStackSpeed += speed * 0.5f;
	}
	if (straightStackSpeed > 0)
	{
		straightStackSpeed -= speed * 0.5f;
	}

	// 速度の最大値最小値のによる更新
	if (straightSpeed < m_minSpeed)
	{
		straightSpeed = m_minSpeed;
	}
	else if (straightSpeed > m_maxSpeed) {
		straightSpeed = m_maxSpeed;
	}
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
	
	// 加速が一定より早くなった時
	if (straightSpeed > m_oldspeed + m_compareSpeed)
	{
		m_streatflg = true;
		if (straightSpeed > m_maxSpeed / 2)
		{
			pjet->SetRot(0.1f);
		}
	}
	else {
		m_streatflg = false;
	}

	// 減速が一定より遅くなった時
	if (straightSpeed < m_oldspeed - m_compareSpeed/2)
	{
		m_downflg = true;
		if (straightSpeed > m_maxSpeed / 2) 
		{
			pjet->SetRot(-0.1f);
		}
	}
	else {
		m_downflg = false;
	}

	m_oldspeed = straightSpeed;
}

// 速度に応じた振動
void PlayerObj::SpeedControllerShake()
{
	m_speedShakeMaxCnt = 3;
	m_speedShakeNowCnt += straightSpeed;

	// 一定以上の時の振動
	if (m_speedShakeNowCnt >= m_speedShakeMaxCnt) {
		auto _power = (straightSpeed / m_maxSpeed) / 4;
		m_speedShakeNowCnt = 0;
		Controller::Instance()->SetShakeController(_power, 1, _power, 1);
	}	
}

// 煙の描画
void PlayerObj::SmogInstanse()
{
	// 似た動きが多いからもっとランダムにする 

	float n = RandomClass::GetRandom(-0.1f, 0.1f);
	float n2 = RandomClass::GetRandom(-0.3f, 0.3f);
	float n3 = RandomClass::GetRandom(-0.5f, 0.5f);
	float h = RandomClass::GetRandom(-2.0f, 2.0f);
	float h2 = RandomClass::GetRandom(-2.0f, 2.0f);

	float alpha = RandomClass::GetRandom(0.5f, 0.8f);
	float alpha2 = RandomClass::GetRandom(0.5f, 0.8f);

	float size = RandomClass::GetRandom(1.5f, 2.0f);
	float size2 = RandomClass::GetRandom(1.5f, 2.5f);

	// 雲生成 ジェット
	Vector3 _pos = owner->transform->position;
	if (straightSpeed > 1.5f)
	{
		_pos.x += n;
		smog->SetActive(_pos + n - owner->transform->forward() * 20, size, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
		smog->SetActive(_pos + n2 - owner->transform->forward() * 21.5f, size2, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
		smog->SetActive(_pos + n3 - owner->transform->forward() * 23, size2, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
	}

	// 一定値以上の時に生成
	if (m_stackRot.x > winglInstValue || m_stackRot.x < -winglInstValue) {
		// 右
		smog->SetWingSmog(_pos + h / 2 + tra->right() * 9, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		smog->SetWingSmog(_pos + h2 / 2 + tra->right() * 12, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		// 左
		smog->SetWingSmog(_pos - h - tra->right() * 9, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		smog->SetWingSmog(_pos - h2 - tra->right() * 12, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
	}

	//-----------------
	// 翼の上下
	//-----------------
	if (m_stackRot.x > 0) {
		pwing->SetRot(3);
	}
	if (m_stackRot.x < 0) {
		pwing->SetRot(-3);
	}

	//todo  周辺の雲生成
	if (m_streatflg || m_downflg)
		InstancePerimetrySmog(true);
	else 
		InstancePerimetrySmog(false);
}

// 周辺の雲生成
void PlayerObj::InstancePerimetrySmog(bool _flg)
{
	float n = RandomClass::GetRandom(-10.0f, 10.0f);

	auto w = RandomClass::GetRandom(-50.0f, 50.0f);
	auto h = RandomClass::GetRandom(-50.0f, 50.0f);

	float size = RandomClass::GetRandom(5.0f, 10.0f);
	float size2 = RandomClass::GetRandom(5.0f, 10.0f);

	// 雲生成
	Vector3 _pos = owner->transform->position;
	_pos.x += n;

	if (_flg) {
		auto w2 = RandomClass::GetRandom(-50.0f, 35.0f);
		auto h2 = RandomClass::GetRandom(-50.0f, 50.0f);
		auto w3 = RandomClass::GetRandom(-50.0f, 50.0f);
		auto h3 = RandomClass::GetRandom(-50.0f, 50.0f);
		smog->SetAirSmog(_pos + Vector3(w, h, 0) + tra->forward() * 25 - tra->right() * 15, Vector3(20, 10, 1) + size, XMFLOAT4(1, 1, 1, 0.05f), 0.005f);
		smog->SetAirSmog(_pos + Vector3(w2, h2, 0) + tra->forward() * 25 + tra->right() * 15, Vector3(20, 10, 1) + size2, XMFLOAT4(1, 1, 1, 0.05f), 0.005f);
		smog->SetAirSmog(_pos + Vector3(w3, h3, 0) + tra->forward() * 25 + tra->right() * 15, Vector3(20, 10, 1) + size2, XMFLOAT4(1, 1, 1, 0.05f), 0.005f);

		return;
	}

	if (m_perimetrySmogMaxCnt <= m_perimetrySmogCnt) {
		smog->SetAirSmog(_pos + Vector3(w, h, 0) + tra->forward() * 25 - tra->right() * 15, Vector3(20, 10, 1) + size, XMFLOAT4(1, 1, 1, 0.05f), 0.005f);
		m_perimetrySmogCnt = 0;
	}
	else {
		m_perimetrySmogCnt += straightSpeed;
	}

}


void PlayerObj::DrawSpeed()
{
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::NORMAL)
	{
		return;
	}
	// スピード
	int s = straightSpeed * 100;
	std::string speed(std::to_string(s));

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 140,
		Application::Instance()->CLIENT_HEIGHT / 2 - 45,
		15, 15, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), "Speed");
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 130,
		Application::Instance()->CLIENT_HEIGHT / 2 - 25,
		18, 18, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), speed.c_str());

	// 高さ
	s = tra->position.y;
	std::string alt(std::to_string(s));
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 140,
		Application::Instance()->CLIENT_HEIGHT / 2 - 45,
		15, 15, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), "Alt");
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 130,
		Application::Instance()->CLIENT_HEIGHT / 2 - 25,
		18, 18, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), alt.c_str());

}

// 当たり判定
void PlayerObj::Collision()
{
	Vector3 hoge = Vector3(0, 0, 0);
	
	// 当たり判定の確認　失敗時はリターン
	if (!StageCollider::GetInstans()->CrossPoint(tra->position, hoge))
	{
		return;
	}

	// 当たっているときの位置補正
	if (tra->position.y <= hoge.y)
	{
		tra->position.y = hoge.y;
	}

}

// 死亡時の更新
void PlayerObj::DeadUpdate()
{
	if (m_nowdeadcnt == 0) 
	{
		// 爆発生成
		//bomb->SetExistState(true);
	}
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		m_nowdeadcnt++;
		DeadFly(tra->forward());
		//bomb->transform->position = owner->transform->position;

		// 煙
		const int hoge = 2;
		for (int i = 0; i < hoge; i++)
		{// 大きさ5で煙ぽかった
			smog->SetActive(tra->position + tra->forward() * straightSpeed * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.5f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
		}
	}
	else {
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::END);
		owner->SetExistState(false);
	}
}


void PlayerObj::DeadFly(XMFLOAT4 _axisZ)
{
	// 向いてる向きに落ちる
	straightSpeed = 1;		// 速度は現在のスピード
	tra->position.x += _axisZ.x * straightSpeed;
	tra->position.y += _axisZ.y * straightSpeed;
	tra->position.z += _axisZ.z * straightSpeed;

	tra->RotateXaxis(0.5f);
}


void PlayerObj::BulletEffect() 
{
	auto _bullet = BulletManager::Instance();
	if (_bullet->GetBulletType() == BulletManager::BulletType::Bullet)
	{
		if (_bullet->GetFire()) {
			if (_bullet->GetNormalBulletPos()) {
				bulletRPlace->OnFire();
			}
			else {
				bulletLPlace->OnFire();
			}	
		}
	}
}