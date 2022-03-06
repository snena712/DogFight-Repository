#include "EnemyCountUI.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

EnemyCountUI::EnemyCountUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("enecnt");

	tra = owner->transform;
	strcom = owner->AddComponent<Component::StringComponent>(0.95f);
	strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 - 535,
		Application::Instance()->CLIENT_HEIGHT / 2 - 340, 0),
		Vector3(20, 20, 0));
	strcom->SetColor(Vector3(0, 1.0f, 0.0f));

}

EnemyCountUI::~EnemyCountUI()
{

}

void EnemyCountUI::Start()
{}

void EnemyCountUI::Update()
{
	String();

	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
	}
}

// •¶Žš•`‰æ
void EnemyCountUI::String()
{
	int s = EnemyManager::Instance()->GetLiveCount();
	std::string ui("ENEMY:"+std::to_string(s));

	strcom->SetString(ui);
}