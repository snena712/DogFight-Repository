#include "MiniMapIconUI.h"

using namespace Component;


MiniMapIconUI::MiniMapIconUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("MiniMapIcon");
}


void MiniMapIconUI::SetObjType(ObjType _objtype)
{
	m_objType = _objtype; 

	switch (m_objType)
	{
	case ObjType::Player:
		ren = owner->AddComponent< Component::Quad2DRender>("assets/Image/UI/HomingBulletUI.png");
		//ren = owner->AddComponent < Component::Quad2DRender > ("assets/Image/UI/Airport.png");
		ren->SetHeight(10);
		ren->SetWidth(15);
		ren->SetVertexColor(XMFLOAT4( 1,0,0,1));
		ren->SetAnchorType(AnchorType::LeftTop);
		
		//‰ñ“]
		owner->transform->RotateZaxis(90);
		break;
	case ObjType::Enemy:
		ren = owner->AddComponent< Component::Quad2DRender>("assets/Image/UI/EnemyUI.png");
		ren->SetHeight(9);
		ren->SetWidth(9);
		ren->SetVertexColor(XMFLOAT4(1, 0.3f, 0.3f, 0.9f));
		ren->SetAnchorType(AnchorType::LeftTop);
		break;
	default:
		break;
	}
	start = owner->AddComponent<Component::StartUIColorComponent>(0.7f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));
}