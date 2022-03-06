#include "OptionManager.h"
#include "../../../DX11System/Controller.h"


using namespace Component;

OptionManager::OptionManager(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionUI");

	m_start = new GameObject();
	m_start->transform->position = Vector3(0, 0, 0);
	m_start->AddComponent<Component::OptionStartUI>();

	m_retry = new GameObject();
	m_retry->transform->position = Vector3(0, 10, 0);
	m_retry->AddComponent<Component::OptionRetryUI>();

	m_setting = new GameObject();
	m_setting->transform->position = Vector3(0, 20, 0);
	m_setting->AddComponent<Component::OptionSettingUI>();

	m_title= new GameObject();
	m_title->transform->position = Vector3(0, 30, 0);
	m_title->AddComponent<Component::OptionTitleUI>();

	m_end = new GameObject();
	m_end->transform->position = Vector3(0, 40, 0);
	m_end->AddComponent<Component::OptionEndUI>();

	m_backui= new GameObject();
	m_backui->AddComponent<Component::SelectBackUI>();
	m_backui->SetExistState(false);
	m_backui->GetComponent<Component::FadeComponent>()->SetOut(2);

	m_line = new GameObject();
	m_line->AddComponent<Component::SelectUnderbarUI>();
	m_line->SetExistState(false);

	SceneManager::Instance()->AddObject(m_end, false);
	SceneManager::Instance()->AddObject(m_retry, false);
	SceneManager::Instance()->AddObject(m_setting, false);
	SceneManager::Instance()->AddObject(m_start, false);
	SceneManager::Instance()->AddObject(m_title, false);
	SceneManager::Instance()->AddObject(m_backui, false);
	SceneManager::Instance()->AddObject(m_line, false);
}

OptionManager::~OptionManager()
{

}

void OptionManager::Start()
{
	ControllerNum = Controller::Instance()->GetControllerNum();

	fade = SceneManager::Instance()->GameObjectFindName("FadeManager")->GetComponent<Component::FadeManager>();
	fade->SetOutScene(60);
	MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::START);
}

void OptionManager::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::START)
	{
		if (ControllerNum >= 0)
		{
			InputController();
		}
		else {
			InputKeybord();
		}

		if (m_option != OptionState::WAIT) {


			{
				// 通常時

				ResetCheck();

				OptionUpdate();

				// スタートボタン入力に対する反応
				PressStart();

				ColorChange();
				String();

				NumLimit();

				LinePos();
			}
		}
	}
}


void OptionManager::NumLimit()
{
	const int _settingnum = 2;

	switch (m_option)
	{
	case Component::OptionManager::OptionState::MAIN:
		if (m_optionNum > m_maxoptionNum)
		{
			m_optionNum = 0;
		}
		if (m_optionNum < 0)
		{
			m_optionNum = m_maxoptionNum;
		}
		break;
	case Component::OptionManager::OptionState::SETTING:
		if (m_optionNum > _settingnum)
		{
			m_optionNum = 0;
		}
		if (m_optionNum < 0)
		{
			m_optionNum = _settingnum;
		}
		break;

	default:
		break;
	}
	printf("%d:　オプション\n", m_optionNum);
}
void OptionManager::InputKeybord()
{
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_F1))
	{
		m_startInputflg = true;
	}

	// オンの時
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
	{
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {
			m_optionNum--;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {
			m_optionNum++;
		}

		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_A))
		{
			m_yesflg = true;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_B))
		{
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
		}
	}
}

void OptionManager::InputController()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	float y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;
	//Controller::Instance()->GetButtonSTARTTrigger() ||
	if (Controller::Instance()->GetButtonSTARTTrigger()) {
		m_startInputflg = true;
	}

	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
	{
		if (y > m_deadzone) {
			if (!m_yesflg) {
				m_yesflg = true;
				m_optionNum--;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else if (y < -m_deadzone)
		{
			if (!m_yesflg) {
				m_yesflg = true;
				m_optionNum++;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else {
			// 
			m_yesflg = false;
			m_nowinputcnt = 0;
		}

		if (m_nowinputcnt > m_maxinputcnt)
		{
			m_nowinputcnt = 0;
			m_yesflg = false;
		}

		// 決定
		if (Controller::Instance()->GetButtonATrigger()) 
		{
			m_yesflg = true;
		}
	

		if (Buttons & XINPUT_GAMEPAD_B) {
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
		}
		
		printf("%d\n", m_optionNum);
	}
}

// スタートボタン入力に対する反応
void OptionManager::PressStart()
{
	if (m_startInputflg)
	{
		if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::OPTION)
		{
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::OPTION);

			m_option = OptionState::MAIN;
			SetChangeFlg(ColorMODE::UP);
			m_line->SetExistState(true);
			m_backui->GetComponent<Component::FadeComponent>()->SetFade(10);
			m_startInputflg = false;
		}
		else
		{
			ResetCheck();
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
			m_startInputflg = false;
		}
	}
}

// 処理の更新
void OptionManager::OptionUpdate()
{
	switch (m_option)
	{
	case Component::OptionManager::OptionState::MAIN:
		NormalUpdate();
		break;

	case Component::OptionManager::OptionState::SETTING:
		SettingUpdate();
		break;

		break;
	default:
		break;
	}

}

void OptionManager::NormalUpdate()
{
	if (m_yesflg)
	{
		m_yesflg = false;
		switch (m_optionNum)
		{
			// 続ける
		case 0:
			m_start->SetExistState(true);
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
			m_option = OptionState::WAIT;
			ResetCheck();
			break;
			// やり直す
		case 1:
			m_retry->SetExistState(true);
			m_line->SetExistState(false);
			m_option = OptionState::WAIT;
			break;
			// 設定
		case 2:

			m_option = OptionState::SETTING;
			m_optionNum = 0;
			break;

			// タイトルへ
		case 3:
			m_title->SetExistState(true);
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::TITLE);
			m_option = OptionState::WAIT;
			break;

			// 終わる//
		case 4:
			m_end->SetExistState(true);
			m_option = OptionState::WAIT;
			break;

		default:
			break;
		}
	}
}

void OptionManager::SettingUpdate()
{
	// 入力あった時
	if (m_yesflg)
	{
		auto _config = Config::Instance();
		switch (m_optionNum)
		{
		case 0:
			printf("コントローラーオン");
			_config->SetReverseX(!_config->GetReverseX());
			m_yesflg = false;
			break;
		case 1:
			printf("振動オン");
			_config->SetControllerVib(!_config->GetControllerVib());
			m_yesflg = false;
			break;
		case 2:

			if (!m_chanflg)
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (m_chanflg)
			{
				m_option = OptionState::MAIN;
				m_optionNum = 0;
				SetChangeFlg(ColorMODE::UP);
				m_yesflg = false;
			}
			break;
		default:
			break;
		}
	}
}


// 設定の初期化
void OptionManager::ResetCheck()
{
	// 決定後シーン状態が異なるとき
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::OPTION
		&& m_option != OptionState::NONE)
	{
		m_option = OptionState::NONE;

		SetChangeFlg(ColorMODE::DOWN);
		m_backui->GetComponent<Component::FadeComponent>()->SetOut(10);
		m_line->SetExistState(false);
		m_optionNum = 0;
		
		m_yesflg = false;
	}
}

void OptionManager::String()
{
	const std::string option = ("OPTION");
	const std::string start = ("START");
	const std::string retry = ("RETRY");
	const std::string setting = ("SETTING");
	const std::string backtitle = ("BACK TO TITLE");
	const std::string end = ("END");


	const std::string stage = (": STAGE");
	const std::string stage1 = ("STAGE-1");

	const std::string back = ("BACK");

	const std::string _con = ("CONTROLER");
	const std::string _vib = ("VIB");

	std::string _reverseConStr = "OFF";
	std::string _vibConStr = "OFF";
	auto _config = Config::Instance();

	const int size = 25;
	const int brank = 10;

	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
	{
		switch (m_option)
		{
		case Component::OptionManager::OptionState::MAIN:

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 ,
				Application::Instance()->CLIENT_HEIGHT / 2 - 340,
				35, 35, m_color, option.c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 2,
				size, size, m_color, start.c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
				size, size, m_color, retry.c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 4,
				size, size, m_color, setting.c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 5,
				size, size, m_color, backtitle.c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 6,
				size, size, m_color, end.c_str());

			break;


			break;
		case Component::OptionManager::OptionState::SETTING:
			if (_config->GetReverseX())
			{
				_reverseConStr = "ON";
			}
			if (_config->GetControllerVib())
			{
				_vibConStr = "ON";
			}
			
			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 2,
				size, size, m_color, (_con + "   " + _reverseConStr).c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
				size, size, m_color, (_vib + "         " + _vibConStr).c_str());

			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
				Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 4,
				size, size, m_color, back.c_str());

			break;
		case Component::OptionManager::OptionState::END:

			break;
		default:
			break;
		}
	}
}

void OptionManager::LinePos()
{
	const Vector3 base = Vector3(40, -130, 1);
	const int size = 25;
	m_strlen = 5;
	float _len = (m_strlen * size) / 2;
	int m_inputnum = 5;
	//line->GetComponent<Component::SelectUnderbarUI>()->SetPos(Vector3(base.x + _len, base.y - m_inputnum * 0.1f, base.z));

	m_line->GetComponent<Component::SelectUnderbarUI>()->SetPos(Vector3(base.x + _len, base.y - m_optionNum * (size + 10), 2));
	m_line->GetComponent<Component::Quad2DRender>()->SetWidth(m_strlen * size);
}

bool OptionManager::ColorChange()
{
	if (colorMode != ColorMODE::NONE)
	{
		if (m_nowchancnt < m_chancntmax)
		{
			m_nowchancnt++;

			if (colorMode == ColorMODE::UP) {
				m_color.x += m_maxcolor.x / m_chancntmax;
				m_color.y += m_maxcolor.y / m_chancntmax;
				m_color.z += m_maxcolor.z / m_chancntmax;
				m_color.w += m_maxcolor.w / m_chancntmax;
			}
			if (colorMode == ColorMODE::DOWN)
			{
				m_color.x -= m_maxcolor.x / m_chancntmax;
				m_color.y -= m_maxcolor.y / m_chancntmax;
				m_color.z -= m_maxcolor.z / m_chancntmax;
				m_color.w -= m_maxcolor.w / m_chancntmax;
			}

			m_chanflg = false;
			return false;
		}
		else {

			if (colorMode == ColorMODE::UP) {
				m_color.x = m_maxcolor.x;
				m_color.y = m_maxcolor.y;
				m_color.z = m_maxcolor.z;
				m_color.w = m_maxcolor.w;
			}
			if (colorMode == ColorMODE::DOWN)
			{
				m_color.x = 0;
				m_color.y = 0;
				m_color.z = 0;
				m_color.w = 0;
			}

			m_nowchancnt = 0;
			m_chanflg = true;
			colorMode = ColorMODE::NONE;

			return true;
		}
	}
	else {
		m_chanflg = false;
	}
}