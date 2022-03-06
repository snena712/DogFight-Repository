#include "TitlePlayer.h"
#include "../../DX11System\quad.h"
#include "../../DX11System\CDirectxGraphics.h"
#include "../../DX11System\Shader.h"
#include "../../DX11System/CDirectInput.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../DX11System\ImageResourceManager.h"
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
#include "../UI/Fade/FadeManager.h"
#include "../MainGameManager.h"
#include "../../Component/ComputeComponent.h"

using namespace Component;

TitlePlayer::TitlePlayer(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("player");

	tra = owner->transform;

	tra->position = Vector3(0,0, 0);

	// �t�F�[�h�Ɋւ���I�u�W�F�N�g
	fade = new GameObject();
	fade->AddComponent<Component::FadeManager>();
	SceneManager::Instance()->AddObject(fade, false);
	fade->GetComponent<Component::FadeManager>()->SetOutScene(30);
	MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::TITLE);

	// �p�[�e�B�N��
	comCompo =
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "assets/EfeFile/title.EFE");
	
}

TitlePlayer::~TitlePlayer()
{}

void  TitlePlayer::Update()
{
	Start();

	Input();

	String();

	InputCnt();
}


void TitlePlayer::Start()
{
	ControllerNum = Controller::Instance()->GetControllerNum();
}

// ������̕`��
void TitlePlayer::String()
{
	XMFLOAT4 m_col = _base;


	if (m_inputflg)
	{
		if (m_inputcnt % 10 >= 5) {
			m_col = _base;
		}
		else {
			m_col = XMFLOAT4(0, 0, 0, 0);
		}

		if (m_inputmax < m_inputcnt)
		{
			
			m_col2.w -= 0.5f;
			m_col = m_col2;
		}
	}

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 360,
		Application::Instance()->CLIENT_HEIGHT / 2 - 100,
		80, 80, m_col2, "DOG FIGHT");

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 250,
		Application::Instance()->CLIENT_HEIGHT / 2 + 50,
		50, 50, m_col, "PLESS TO A");
}

// ����
void TitlePlayer::Input()
{
	// ��]
	if (ControllerNum != -1) {
		Controller();
	}
	else
	{
		KeyBord();
	}
}

// �L�[�{�[�h
void  TitlePlayer::KeyBord()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {

		m_inputflg = true;
	}
}

// �R���g���[���[
void TitlePlayer::Controller()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	//�A�i���O�����L�[
	// ��
	Vector3 inputVector;
	inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLX / 360.0f;
	inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;


	// �E
	inputVector;
	if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX / 365.0f;
		inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY / 365.0f;
	}

	// ����
	if (Buttons & XINPUT_GAMEPAD_A && !m_inputflg) {
		m_inputflg = true;
	}
}

// �J�E���g�Ǘ�
void TitlePlayer::InputCnt()
{
	// ���͂�������
	if (m_inputflg)
	{
		// ���͏���
		if (m_inputcnt == 0)
		{
			PlaySound(SOUND_OPTION_SE000);
		}

		// �J�E���g�ɉ���������
		if (m_inputmax > m_inputcnt)
		{
			m_inputcnt++;
		}
		if (m_inputmax == m_inputcnt)
		{
			m_inputcnt++;
			fade->GetComponent<Component::FadeManager>()->SetFadeInScene(60, "SelectScene");
		}
		// �t�F�[�h�C������
		if (fade->GetComponent<Component::FadeManager>()->GetFadeOut())
		{
			// shader�̐F������
			comCompo->SetInitColor(XMFLOAT4(0,0,0,0));
			comCompo->SetEndColor(XMFLOAT4(0,0,0,0));
		}

	}
}