#include "DestroyUI.h"
using namespace Component;

DestroyUI::DestroyUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("DestroyUI");
	tra = owner->transform;

	// ”š”jƒAƒjƒ[ƒVƒ‡ƒ“
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/DestroyUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(50);
	ren->SetWidth(75);

	float h = Application::CLIENT_HEIGHT;
	tra->position.x = Application::CLIENT_WIDTH / 2;
	tra->position.y = -h / 2 + 220;
	tra->position.z = 1;

	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.0f));
}

DestroyUI::~DestroyUI()
{

}

void DestroyUI::Start()
{
}

void DestroyUI::Update()
{
	StatusUpdate();
}

void DestroyUI::StatusUpdate()
{
	// Ž€–SŽž
	if (EnemyManager::Instance()->GetDeadFlg())
	{
		m_deadflg = true;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
	}

	if (m_deadflg)
	{
		if (m_maxinputcnt > m_nowinputcnt)
		{
			m_nowinputcnt++;
		}

		else {
			m_deadflg = false;
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0));
		}
	}
}
