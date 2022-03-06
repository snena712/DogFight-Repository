#include "SpeedUI.h"

using namespace Component;

SpeedUI::SpeedUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("SpeedUI");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/TriangleFrame.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(40);
	ren->SetWidth(75);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 - 100, -hei / 2 + 15, 1);
}

SpeedUI::~SpeedUI()
{
}

void SpeedUI::Start()
{
}

void SpeedUI::Update()
{
}

