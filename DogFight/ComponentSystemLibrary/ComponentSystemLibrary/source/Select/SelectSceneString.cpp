#include "SelectSceneString.h"

using namespace Component;

SelectSceneString::SelectSceneString(GameObject *_owner) :ComponentBase(_owner)
{
	tra = owner->transform;

	strcom = owner->AddComponent<Component::StringComponent>(0.9f);
	strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 + 350,
		Application::Instance()->CLIENT_HEIGHT / 2 + 250, 0),
		Vector3(15, 15, 0));

	strcom->SetColor(Vector3(0, 1.0f, 0.0f));
}


void SelectSceneString::Update()
{

	DrawString();
}

// •¶Žš•`‰æ
void SelectSceneString::DrawString()
{
	switch (m_status)
	{
	case Status::Load:
		if (strcom->GetFadeStatus() == StringComponent::Fade::None) {
			strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
			strcom->SetString("NOW LOADING");
		}
		break;

	case Status::Press:
		strcom->SetString("A:OK B:BACK");
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
		break;

	default:
		break;
	}
}