#include "StartUI.h"
#include "../../../DX11System\CDirectxGraphics.h"

using namespace Component;

StartUI::StartUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("StartUI");

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/BoxFrame.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(50);
	ren->SetWidth(500);
	ren->SetVertexColor(XMFLOAT4(0, 1.0f, 0.2f, 1.0f));

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;
	owner->transform->position = Vector3(wid / 2, -hei / 2 + 80, 1);

	startcom = owner->AddComponent<Component::StartUIColorComponent>(1.0f, StartUIColorComponent::RnderState::TWO);
	startcom->SetColor(Vector3(0, 1.0f, 0.2f));

	strcom = owner->AddComponent<Component::StringComponent>(1.0f);
	strcom->SetPosSize(Vector3(wid / 2 - 18 * 13, hei / 2 - 100, 0),
		Vector3(36, 36, 0));
	strcom->SetColor(Vector3(0, 1.0f, 0.2f));
}

StartUI::~StartUI()
{
}

void  StartUI::Update()
{
	String();

	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::START)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
		startcom->SetFadeStatus(StartUIColorComponent::Fade::FadeIn);
	}
	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeOut);
		startcom->SetFadeStatus(StartUIColorComponent::Fade::FadeOut);
	}
}

void StartUI::String()
{
	strcom->SetString("MISSION START");
}