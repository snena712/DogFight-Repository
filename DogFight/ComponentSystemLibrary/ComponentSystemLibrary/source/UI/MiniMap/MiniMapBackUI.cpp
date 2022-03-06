#include "MiniMapBackUI.h"

using namespace Component;

MiniMapBackUI::MiniMapBackUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("enecnt");

	tra = owner->transform;

	// ”š”jƒAƒjƒ[ƒVƒ‡ƒ“
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/raderback.png");
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(200);
	ren->SetWidth(200);	
	tra->position = Vector3(100, -623, 1);

	start = owner->AddComponent<Component::StartUIColorComponent>(0.2f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));
}

MiniMapBackUI ::~MiniMapBackUI()
{

}

void MiniMapBackUI::Start()
{
}

void MiniMapBackUI::Update()
{
	//‰ñ“]
	owner->transform->RotateZaxis(2);
}

