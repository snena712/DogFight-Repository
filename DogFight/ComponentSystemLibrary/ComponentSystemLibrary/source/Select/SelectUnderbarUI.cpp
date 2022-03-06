#include "SelectUnderbarUI.h"

using namespace Component;

SelectUnderbarUI::SelectUnderbarUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SelectUnderbarUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/Select/Underbar.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(10);
	ren->SetWidth(200);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
}

SelectUnderbarUI::~SelectUnderbarUI()
{

}

void SelectUnderbarUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void SelectUnderbarUI::Update()
{
	PosUpdate();
}

void SelectUnderbarUI::PosUpdate()
{
	tra->position = m_pos;
}

