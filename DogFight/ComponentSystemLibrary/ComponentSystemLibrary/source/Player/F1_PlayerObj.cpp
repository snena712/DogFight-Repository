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
		"assets/F2_v2/f2a_game_v3.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/CookTorranceVS.hlsl",				// ���_�V�F�[�_�[
		"shader/CookTorrancePS.hlsl",				// �s�N�Z���V�F�[�_�[
		"assets/F2_v2/f2a_texture/");				// �e�N�X�`���̑��݃t�H���_

	//model = this->AddComponent<Component::ModelComponent>(
	//	"assets/f1/f1.x",		// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
	//	"shader/vs.hlsl",		// ���_�V�F�[�_�[
	//	"shader/ps.hlsl",		// �s�N�Z���V�F�[�_�[
	//	"assets/f1/");			// �e�N�X�`���̑��݃t�H���_

	splashes = new GameObject();
	splashes->AddComponent<Component::SplashesEfect>();
	SceneManager::Instance()->AddObject(splashes, false);

	// ��
	GameObject* wingparent = new GameObject();
	wingparent->transform->SetParent(owner->transform);
	pwing = wingparent->AddComponent<Component::PlayerWingParent>();
	wingparent->transform->position.z -= 2.1f;
	SceneManager::Instance()->AddObject(wingparent, false);

	// �W�F�b�g�̃p�[�c�i����͕ʂ̂��́j
	GameObject* jparts = new GameObject();
	jparts->transform->position.z -= 3.93f;// 0�̂Ƃ�
	jparts->transform->position.y -= 0.05f;
	pjet =jparts->AddComponent<Component::PlayerJatPartsParent>();
	SceneManager::Instance()->AddObject(jparts, false);

	// �����p�R���|�[�l���g
	fall = owner->AddComponent<Component::FallObjectComponent>(0.5f, 2.0f);
	//�O��
	out=owner->AddComponent<Component::OutSideControlComponent>();

	// ���
	//root = new GameObject();
	//root->AddComponent<Component::EnemyRoot>();
	//SceneManager::Instance()->AddObject(root, false);
	//root->SetExistState(false);

	// �ΉԂ̏ꏊ
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
			// �ړ�
			AutoGo(tra->forward());
			SpeedControllerShake();
		}

		if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::NORMAL)
		{
			break;
		}

		//root->SetExistState(true);

		Collision();


		// �O�ɍs�������̏���
		if (out->GetOutFlg())
		{
			// �͈͊O
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

		// �ړ�
		fall->SpeedFall(straightSpeed);
		AutoGo(tra->forward());
		SpeedControllerShake();

		// �_����
		SmogInstanse();

		// �p�[�e�B�N�������ꏊ
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

	//�@�ꕔ�`�揈��
	DrawSpeed();

	//// �c���̐�
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
	// X���̍��E��]
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_W)) {
		m_stackRot.x += 2;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {
		m_stackRot.x -= 2;
	}

	// Y���̍��E��]
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {
		m_stackRot.y -= 2;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_D)) {
		m_stackRot.y += 2;
	}

	// Y���̍��E��]
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

	// �e���̌v�Z
	tra->RotateLocalZaxis(-0.03f * (m_stackRot.z));
	tra->RotateLocalYaxis(0.03f * (m_stackRot.y));

	// ��]�Ɋւ���v�Z
	RotUpdate();

	// ���x
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

// �R���g���[���[
void PlayerObj::Controller()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	Vector3 inputVector;

	//�A�i���O�����L�[
	if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// ��
		inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLX / 360.0f;
		inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;

		// �^�������ړ�
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

		//----�E----//
		if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		{
			inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX / 365.0f;
			inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY / 365.0f;
		}
	}

	// ��]����
	// ���͑�������
	if (inputVector.x > deadZone || inputVector.x < -deadZone) {
		m_stackRot.z += (int)(inputVector.x) * 0.025f;
	}
	if (inputVector.y > deadZone || inputVector.y < -deadZone) {
		m_stackRot.x += (int)(inputVector.y) * 0.025f;
	}

	// ��]�Ɋւ���v�Z
	RotUpdate();

	// �㉺��]
	tra->RotateLocalXaxis(0.03f * (m_stackRot.x));
	// ���͂��Ă��Ȃ����̌�������
	tra->RotateLocalZaxis(-0.03f * (m_stackRot.z));
	// Y���̍��E��]
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

// ��]�̍X�V
void PlayerObj::RotUpdate()
{
	// ��������
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


	// X���������
	if (m_stackRot.x > m_maxRot)
	{
		m_stackRot.x = m_maxRot;
	}
	if (m_stackRot.x < -m_maxRot)
	{
		m_stackRot.x = -m_maxRot;
	}

	// Y���������
	if (m_stackRot.y > m_maxRot)
	{
		m_stackRot.y = m_maxRot;
	}
	if (m_stackRot.y < -m_maxRot)
	{
		m_stackRot.y = -m_maxRot;
	}

	// Z���������
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


// �t���[�g�̒l����肢�Ȃ��ł��邩�𔻒f
bool PlayerObj::floatcheck(float _checkvalue, float _value, float _minimum)
{
	if (fabsf(_checkvalue - _value) < _minimum)
	{
		return true;
	}

	return false;
}

// �����ړ�
void PlayerObj::AutoGo(XMFLOAT4 _axisZ)
{
	straightSpeed += straightStackSpeed * 0.02f;

	// �����x�̍X�V
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

	// ���x�̍ő�l�ŏ��l�̂ɂ��X�V
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
	
	// ����������葁���Ȃ�����
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

	// �����������x���Ȃ�����
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

// ���x�ɉ������U��
void PlayerObj::SpeedControllerShake()
{
	m_speedShakeMaxCnt = 3;
	m_speedShakeNowCnt += straightSpeed;

	// ���ȏ�̎��̐U��
	if (m_speedShakeNowCnt >= m_speedShakeMaxCnt) {
		auto _power = (straightSpeed / m_maxSpeed) / 4;
		m_speedShakeNowCnt = 0;
		Controller::Instance()->SetShakeController(_power, 1, _power, 1);
	}	
}

// ���̕`��
void PlayerObj::SmogInstanse()
{
	// ����������������������ƃ����_���ɂ��� 

	float n = RandomClass::GetRandom(-0.1f, 0.1f);
	float n2 = RandomClass::GetRandom(-0.3f, 0.3f);
	float n3 = RandomClass::GetRandom(-0.5f, 0.5f);
	float h = RandomClass::GetRandom(-2.0f, 2.0f);
	float h2 = RandomClass::GetRandom(-2.0f, 2.0f);

	float alpha = RandomClass::GetRandom(0.5f, 0.8f);
	float alpha2 = RandomClass::GetRandom(0.5f, 0.8f);

	float size = RandomClass::GetRandom(1.5f, 2.0f);
	float size2 = RandomClass::GetRandom(1.5f, 2.5f);

	// �_���� �W�F�b�g
	Vector3 _pos = owner->transform->position;
	if (straightSpeed > 1.5f)
	{
		_pos.x += n;
		smog->SetActive(_pos + n - owner->transform->forward() * 20, size, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
		smog->SetActive(_pos + n2 - owner->transform->forward() * 21.5f, size2, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
		smog->SetActive(_pos + n3 - owner->transform->forward() * 23, size2, XMFLOAT4(1, 1, 1, alpha), Vector3(0.1f, 0.1f, 0.1f), 0.02f);
	}

	// ���l�ȏ�̎��ɐ���
	if (m_stackRot.x > winglInstValue || m_stackRot.x < -winglInstValue) {
		// �E
		smog->SetWingSmog(_pos + h / 2 + tra->right() * 9, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		smog->SetWingSmog(_pos + h2 / 2 + tra->right() * 12, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		// ��
		smog->SetWingSmog(_pos - h - tra->right() * 9, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
		smog->SetWingSmog(_pos - h2 - tra->right() * 12, Vector3(10, 5, 1) + h, XMFLOAT4(1, 1, 1, 0.1f), 0.03f);
	}

	//-----------------
	// ���̏㉺
	//-----------------
	if (m_stackRot.x > 0) {
		pwing->SetRot(3);
	}
	if (m_stackRot.x < 0) {
		pwing->SetRot(-3);
	}

	//todo  ���ӂ̉_����
	if (m_streatflg || m_downflg)
		InstancePerimetrySmog(true);
	else 
		InstancePerimetrySmog(false);
}

// ���ӂ̉_����
void PlayerObj::InstancePerimetrySmog(bool _flg)
{
	float n = RandomClass::GetRandom(-10.0f, 10.0f);

	auto w = RandomClass::GetRandom(-50.0f, 50.0f);
	auto h = RandomClass::GetRandom(-50.0f, 50.0f);

	float size = RandomClass::GetRandom(5.0f, 10.0f);
	float size2 = RandomClass::GetRandom(5.0f, 10.0f);

	// �_����
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
	// �X�s�[�h
	int s = straightSpeed * 100;
	std::string speed(std::to_string(s));

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 140,
		Application::Instance()->CLIENT_HEIGHT / 2 - 45,
		15, 15, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), "Speed");
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 130,
		Application::Instance()->CLIENT_HEIGHT / 2 - 25,
		18, 18, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), speed.c_str());

	// ����
	s = tra->position.y;
	std::string alt(std::to_string(s));
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 140,
		Application::Instance()->CLIENT_HEIGHT / 2 - 45,
		15, 15, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), "Alt");
	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 130,
		Application::Instance()->CLIENT_HEIGHT / 2 - 25,
		18, 18, XMFLOAT4(0, 1.0f, 0.2f, 0.9f), alt.c_str());

}

// �����蔻��
void PlayerObj::Collision()
{
	Vector3 hoge = Vector3(0, 0, 0);
	
	// �����蔻��̊m�F�@���s���̓��^�[��
	if (!StageCollider::GetInstans()->CrossPoint(tra->position, hoge))
	{
		return;
	}

	// �������Ă���Ƃ��̈ʒu�␳
	if (tra->position.y <= hoge.y)
	{
		tra->position.y = hoge.y;
	}

}

// ���S���̍X�V
void PlayerObj::DeadUpdate()
{
	if (m_nowdeadcnt == 0) 
	{
		// ��������
		//bomb->SetExistState(true);
	}
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		m_nowdeadcnt++;
		DeadFly(tra->forward());
		//bomb->transform->position = owner->transform->position;

		// ��
		const int hoge = 2;
		for (int i = 0; i < hoge; i++)
		{// �傫��5�ŉ��ۂ�����
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
	// �����Ă�����ɗ�����
	straightSpeed = 1;		// ���x�͌��݂̃X�s�[�h
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