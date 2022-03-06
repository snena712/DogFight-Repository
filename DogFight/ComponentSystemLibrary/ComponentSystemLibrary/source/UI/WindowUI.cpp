#include "../UI/WindowUI.h"
#include "../../Component/SceneManager.h"

using namespace Component;

WindowUI::WindowUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "WindowUI");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/CeneterUISub.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(30);
	ren->SetWidth(30);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 , -hei / 2 , 1);
}

WindowUI::~WindowUI()
{

}

void WindowUI::Start()
{
}

void WindowUI::Update()
{
}
