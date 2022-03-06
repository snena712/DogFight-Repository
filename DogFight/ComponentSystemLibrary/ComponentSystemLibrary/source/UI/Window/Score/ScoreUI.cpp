#include "ScoreUI.h"

using namespace Component;

ScoreUI::ScoreUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("ScoreUI");
	strcom = owner->AddComponent<Component::StringComponent>(0.95f);
	strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 - 535,
		Application::Instance()->CLIENT_HEIGHT / 2 - 320, 0),
		Vector3(20, 20, 0));
	strcom->SetColor(Vector3(0, 1.0f, 0.0f));
}

ScoreUI::~ScoreUI()
{

}

void ScoreUI::Start()
{
}

void ScoreUI::Update()
{
	ScoreUpdate();
	PosUpdate();

	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
	}
}


void ScoreUI::PosUpdate()
{
	std::string _nowstr(std::to_string(m_score));
	std::string _plusstr(std::to_string(m_plusscore));

	std::string _scorestr;

	if (m_plusscore > 0) {
		_scorestr = "SCORE:" + _nowstr + "+" + _plusstr;
	}
	else {
		_scorestr = "SCORE:" + _nowstr;
	}

	strcom->SetString(_scorestr);
}

// スコアの更新
void ScoreUI::ScoreUpdate()
{
	// スコア上の死亡数とマネージャーの死亡数の比較
	if (m_deadcnt != EnemyManager::Instance()->GetDeadEnemyCnt())
	{
		m_deadcnt++;
		m_plusscore += m_scorepoint;
	}

	// プラスのスコアがあるとき
	if (m_plusscore > 0) 
	{
		m_score+=10;
		m_plusscore-=10;
	}
}
