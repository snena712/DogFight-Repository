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

	camtype = CameraType::DELAY;	// �J�����̎��

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

	// �v���C���[�����݂��邩�H
	if (player != nullptr)
	{
		// �v���C���[�X�V
		playertra = player->transform;
	}

	// ��ԕω�
	Input();

	// �ʒu�̍X�V
	CameraPos();

}

// ������
void MainCamera::Start()
{
	// �J�����̈ʒu���v���C���[����擾
	player = SceneManager::Instance()->GameObjectFindName("player");
	playertra = player->transform;
	playercompo = player->GetComponent<Component::PlayerObj>();
	out= player->GetComponent<Component::OutSideControlComponent>();

}

// �ʒu�̍X�V
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

// �x��Ēǔ�����J����
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
		// ���S���̃J����
		DeadCamera();
		return;
	}
	
	// �O�̈Ӑ}����ۑ�
	SetOldPos();

	// ����
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
		// ��ʊO�ɂ���������
		cam->GetOwner()->transform->LookAt(player->transform->position, Vector3(0, 1, 0));
		tra->position.y = player->transform->position.y;
		outflg = true;
		stackOutPos = tra->position;

		return;
	}

	// ��ʓ��ŏo�����肪����A���J�E���g�ȓ�
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

// ����
void MainCamera::Input()
{
	if (ControllerNum == -1 && Controller::Instance()->CheckController())
	{
		ControllerNum = Controller::Instance()->GetControllerNum();
		Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	}	
}

// �p�x�����W�A���ɕύX
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}

// ���ʑ����J����(�y����]�Ȃ��o�[�W����)
void MainCamera::UpdateSphereCamera(
	float radius,						// ���a(����)
	float elevation,					// �p�i���́j
	const Vector3 lookat,
	const int _pluscnt)	// �����_�i���́j
{
	Vector3 eye;							// ���_
	Vector3 _pos = Vector3(0, 5, 18);
	float _azimuth;							// ���ʊp�i���́j

	m_startcnt += _pluscnt;
	azimuth -= (float)_pluscnt;

	// �p�A���ʊp�����W�A���ɕϊ�
	elevation = ToRad(elevation);
	_azimuth = ToRad(azimuth);

	// �p�ɉ��������a���v�Z
	float r = radius * sinf(elevation);

	// ���_���W���v�Z
	eye.x = r * cosf(_azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(_azimuth);

	// ���_�x�N�g���𒍎��_�����s�ړ�
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	tra->position = eye + player->transform->up() * _pos;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());

	XMFLOAT4 _qt = tra->qt;

	tra->qt.x = 0;
}

// �x��ĒǏ]����J����
void MainCamera::DelayCamera()
{

	// ��{�̏ꏊ
	float length = 30;


	Vector3 inputVector;
	Vector3 _pos;

	// 
	inputVector.x = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX) / 362.0f;
	inputVector.y = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 362.0f;
	float yinputVector = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 182.0f;

	//inputVector.x���C���Ƃɒ����Ċ|�����芄�����肵�Ă���
	float backx = inputVector.x;
	if (inputVector.x < 5 && -5 < inputVector.x) { inputVector.x = 0; }
	int xxx = inputVector.x;
	double radi = xxx * 3.141592653589793f / 360; _pos.x = length * sin(radi);

	if (inputVector.y < 5 && -5 < inputVector.y) { inputVector.y = 0; }
	radi = inputVector.y * 3.141592653589793f / 360;

	_pos.y = length * sin(radi);

	float firstleng = 1;			// ��������
	const float confirstleng = 18;	// �Œ�̏�������

	if (inputVector.x == 0 && inputVector.y == 0)
	{
		firstleng = confirstleng;
	}

	float z = 0;
	float w = 0;

	// �N�I�[�^�j�I��������@
	cam->GetOwner()->transform->qt = qt[useidx];

	if (ControllerNum != -1 && (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		// ���E���͂ɑ΂��邚���W�̕␳�@
		if (_pos.x < 0)
		{
			z = _pos.x + 30;
		}
		else {
			z = 30 - _pos.x;
		}

		// ���͂��Ȃ��Ƃ��v�Z�̃o�O���Ȃ���
		if (_pos.x == 0)
		{
			_pos.x = 0.01f;
		}
		if (_pos.y == 0)
		{
			_pos.y = 0.01f;
		}

		//======== �J�����̉�]=========== //
		cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

		// ���͂Ȃ��̏����l�̎�
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
		// �U��
		if (playercompo->GetStreatFlg())
		{
			//shake->ShakeCamera(0.05f);
		}
		if (playercompo->GetDownFlg())
		{
			//shake->ShakeCamera(0.05f);
		}

		auto _shakePos = shake->GetRandPos();

		// ���x�ɉ������{��
		auto s = playercompo->GetSpeed();
		auto d = playercompo->GetMaxSpeed();
		float rate = s / d;

		// �J�����̂̐U�����������J�����ʒu
		Vector3 _pos = m_mainPos + shake->GetRandPos();

		_pos.z = _pos.z * (1.0f - rate);

		tra->position = stackpos[useidx]
			- forward[useidxQt] * _pos.z - forward[useidxQt] * 7
			+ up[useidxQt] * (_pos.y) + right[useidxQt] * _pos.x;

		// ����p
		auto _fov = DirectX::XM_PI * 0.3f + DirectX::XM_PI  * (rate) * 0.2f;
		cam->SetFov(_fov);

		// �����_
		cam->GetOwner()->transform->LookAt(player->transform->position + forward[useidx] * m_mainPos.z + up[useidx] * (m_mainPos.y), tra->up());
	}
}

// �G�𒍎�����
void MainCamera::LockOn()
{
	if (EnemyManager::Instance()->GetLookObj() == nullptr) {
		return;
	}

	Vector3 lookat = EnemyManager::Instance()->GetLookObj()->transform->position;	// �����_�i���́j

	// ���_�x�N�g���𒍎��_�����s�ړ�
	tra->position = player->transform->position + player->transform->up() * 3;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());
}

// �X�^�[�g�̉��o�p
void MainCamera::StartCamera(int posecnt, int _cnt, int _pluscnt)
{
	Vector3 lookat = player->transform->position;	// �����_�i���́j
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


	// �ҋ@
	if (posecnt > m_posecnt)
	{
		StartLookCamera(Vector3(11, 5, _ppos.z + 210), m_posecnt);
		m_posecnt += _pluscnt;
	}
	// ��]
	else {
		UpdateSphereCamera(18, 90, lookat, _pluscnt);
	}


	// �J�n���o�X�L�b�v
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A) ||
		ControllerNum != -1 && Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons&XINPUT_GAMEPAD_A)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
	}
}

// �J�n���ɋ@�̂����鉉�o�p
void MainCamera::StartLookCamera(Vector3 _pos, int time)
{
	auto hoge = player->transform->position;
	owner->transform->position = Vector3(_pos.x, hoge.y + _pos.y, _pos.z);
	cam->GetOwner()->transform->LookAt(player->transform->position, player->transform->up());
}

// �ʒu���ƃJ�E���g�̍X�V
void MainCamera::SetOldPos()
{
	qt[svidxQt] = player->transform->qt;
	forward[svidxQt] = player->transform->forward();
	up[svidxQt] = player->transform->up();
	right[svidxQt] = player->transform->right();
	stackpos[svidx] = player->transform->position;
	svidx++;
	svidxQt++;

	// ������
	if (svidx > arraySiza - 1)
	{
		ResetSivCnt();
		firstflag = true;
	}
	// 30�t���[���O�̏����g�p���ăJ�������쐬
	useidx = svidx - _delay;
	if (useidx < 0)
	{
		// ������
		if (!firstflag)
		{
			useidx = 0;
		}// �ʏ�
		else
		{
			useidx = svidx + arraySiza - _delay;
		}
	}

	// ������
	if (svidxQt > arraySiza - 1)
	{
		ResetSivQtCnt();
		firstflag = true;
	}
	// 30�t���[���O�̏����g�p���ăJ�������쐬
	useidxQt = svidxQt - _delayQt;
	if (useidxQt < 0)
	{
		// ������
		if (!firstflag)
		{
			useidxQt = 0;
		}// �ʏ�
		else
		{
			useidxQt = svidxQt + arraySiza - _delayQt;
		}
	}
}

// ���S���p
void  MainCamera::DeadCamera()
{
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());
}

// �N���A���̃J����
void  MainCamera::ClearCamera()
{
	auto _ene = EnemyManager::Instance()->GetDeadObj();
	if (_ene == nullptr) {
		return;
	}

	// �J�����̈ʒu�ƒ����_�̐ݒ�
	m_clearEnemyPos = _ene->transform->position;
	owner->transform->position = m_clearEnemyPos + Vector3(100, 0, 0);
	cam->GetOwner()->transform->LookAt(m_clearEnemyPos, owner->transform->up());
}
