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

	// フェードに関するオブジェクト
	fade = new GameObject();
	fade->AddComponent<Component::FadeManager>();
	SceneManager::Instance()->AddObject(fade, false);
	fade->GetComponent<Component::FadeManager>()->SetOutScene(30);
	MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::TITLE);

	// パーティクル
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

// 文字列の描画
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

// 入力
void TitlePlayer::Input()
{
	// 回転
	if (ControllerNum != -1) {
		Controller();
	}
	else
	{
		KeyBord();
	}
}

// キーボード
void  TitlePlayer::KeyBord()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_A)) {

		m_inputflg = true;
	}
}

// コントローラー
void TitlePlayer::Controller()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	//アナログ方向キー
	// 左
	Vector3 inputVector;
	inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLX / 360.0f;
	inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;


	// 右
	inputVector;
	if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX / 365.0f;
		inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY / 365.0f;
	}

	// 決定
	if (Buttons & XINPUT_GAMEPAD_A && !m_inputflg) {
		m_inputflg = true;
	}
}

// カウント管理
void TitlePlayer::InputCnt()
{
	// 入力あった時
	if (m_inputflg)
	{
		// 入力処理
		if (m_inputcnt == 0)
		{
			PlaySound(SOUND_OPTION_SE000);
		}

		// カウントに応じた処理
		if (m_inputmax > m_inputcnt)
		{
			m_inputcnt++;
		}
		if (m_inputmax == m_inputcnt)
		{
			m_inputcnt++;
			fade->GetComponent<Component::FadeManager>()->SetFadeInScene(60, "SelectScene");
		}
		// フェードイン完了
		if (fade->GetComponent<Component::FadeManager>()->GetFadeOut())
		{
			// shaderの色無効化
			comCompo->SetInitColor(XMFLOAT4(0,0,0,0));
			comCompo->SetEndColor(XMFLOAT4(0,0,0,0));
		}

	}
}