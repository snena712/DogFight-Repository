#include "SkipStringUI.h"

using namespace Component;

SkipStringUI::SkipStringUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SkipStringUI");

	tra = owner->transform;

	float hei = Application::CLIENT_HEIGHT;
	float wid = Application::CLIENT_WIDTH;
	tra->position = Vector3(wid / 2+400, -hei / 2 - 300, 1);

	strcom = owner->AddComponent<Component::StringComponent>(1.0f);
	strcom->SetPosSize(Vector3(wid / 2 + 300, hei / 2 + 300, 1),
		Vector3(18, 18, 0));
	strcom->SetColor(Vector3(0, 1.0f, 0.2f));
}

void SkipStringUI::Update()
{
	String();

	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::START)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
	}
	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeOut);
	}
}

// •¶Žš•`‰æ
void SkipStringUI::String()
{
	strcom->SetString("SKIP A");
}


