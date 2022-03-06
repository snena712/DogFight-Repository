#include "OutSideUI.h"

using namespace Component;

OutSideUI::OutSideUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("OutSideUI");

	tra = owner->transform;

	float hei = Application::CLIENT_HEIGHT;
	float wid = Application::CLIENT_WIDTH;

	strcom = owner->AddComponent<Component::StringComponent>(1.0f);
	strcom->SetPosSize(Vector3(wid / 2 - 18 * 8, hei / 2 - 100, 0),
		Vector3(36, 36, 0));
	strcom->SetColor(Vector3(1, 0.0f, 0.2f));
	strcom->SetString("Out Side");
}

void OutSideUI::Start()
{
	GameObject* player = SceneManager::Instance()->GameObjectFindName("player");
	out = player->GetComponent<Component::OutSideControlComponent>();
}

void OutSideUI::Update()
{
	if (out->GetOutFlg()) {
		// ”ÍˆÍŠO
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
		strcom->SetString("AutoMove");
	}
	else
	if (out->GetBeforeOutFlg()) 
	{
		// ”ÍˆÍŠO‚És‚«‚©‚¯
		if (strcom->GetFadeStatus() == StringComponent::Fade::None) {
			strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
		}
		if (strcom->GetFadeStatus() == StringComponent::Fade::Normal) {
			strcom->SetFadeStatus(StringComponent::Fade::FadeOut);
		}
		strcom->SetString("Out Side");
	}
	else {
		// ’Êí
		strcom->SetFadeStatus(StringComponent::Fade::FadeOut);
	}

}
