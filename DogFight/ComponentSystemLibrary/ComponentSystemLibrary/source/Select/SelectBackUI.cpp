#include "SelectBackUI.h"

using namespace Component;

SelectBackUI::SelectBackUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SelectBackUI");

	tra = owner->transform;

	ren=owner->AddComponent<Component::Quad2DRender>("assets/Image/Select/SelectBackUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	float _wid = Application::CLIENT_WIDTH;
	float _hei = Application::CLIENT_HEIGHT;
	ren->SetHeight(_hei);
	ren->SetWidth(_wid+10);

	tra->position = Vector3(_wid / 2, -_hei / 2, 10);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));

	owner->AddComponent<Component::FadeComponent>();

}

SelectBackUI::~SelectBackUI()
{

}

void SelectBackUI::Start()
{
}

void SelectBackUI::Update(){}
