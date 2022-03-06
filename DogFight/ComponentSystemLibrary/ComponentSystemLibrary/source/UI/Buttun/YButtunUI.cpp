#include "YButtunUI.h"

using namespace Component;

YButtunUI::YButtunUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("YButtunUI");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Buttun/YButtun.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(15);
	ren->SetWidth(15);

	start = owner->AddComponent<Component::StartUIColorComponent>(0.7f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));
}



