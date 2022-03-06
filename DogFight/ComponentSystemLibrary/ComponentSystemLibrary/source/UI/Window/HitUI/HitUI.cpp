#include "HitUI.h"
using namespace Component;

HitUI::HitUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("HitUI");

	tra = owner->transform;

	// ���j�A�j���[�V����
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/HitUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(50);
	ren->SetWidth(75);

	float h = Application::CLIENT_HEIGHT;
	tra->position.x = Application::CLIENT_WIDTH / 2;
	tra->position.y = - h / 2 + 220;
	tra->position.z = 1;;


	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.0f));
}

HitUI::~HitUI()
{

}

void HitUI::Start()
{
}

void HitUI::Update()
{
	StatusUpdate();
}

void HitUI::PosUpdate()
{
}

void HitUI::StatusUpdate()
{
	// �q�b�g��
	if (BulletManager::Instance()->GetHitFlg())
	{
		m_nowinputcnt = 0;
		m_hitflg = true;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1.0f));
	}
	// ���S��
	if (EnemyManager::Instance()->GetDeadFlg())
	{
		m_hitflg = false;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0));
	}

	// �q�b�g��
	if (m_hitflg) 
	{
		if (m_maxinputcnt > m_nowinputcnt)
		{
			m_nowinputcnt++;
		}

		else {
			m_hitflg = false;
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0));
		}
	}
}

// �f�X�g���C�ƃq�b�g�}�[�J�[