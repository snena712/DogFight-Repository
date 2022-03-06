#include "LockOnUI.h"
using namespace Component;

LockOnUI::LockOnUI(GameObject *_owner) :ComponentBase(_owner)
{
	//�摜�̓��b�N�I���Ɣ��j
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/target.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	m_color = XMFLOAT4(0, 1, 0, 1);
	ren->SetVertexColor(m_color);
}


LockOnUI::~LockOnUI()
{
}

void LockOnUI::Start() {}
void LockOnUI::Update() {}
void LockOnUI::DrawObject()
{
	LockOnUpdate();
}


// ���b�N�I���̍X�V
void LockOnUI::LockOnUpdate()
{
	if (lockonFlg) {
		// ���b�N�I����
		if (m_lockoncnt != 0)
		{
			// �T�C�Y�ύX
			ren->SetHeight(m_size - (float)m_lockoncnt*1.1f);
			ren->SetWidth(m_size - (float)m_lockoncnt*1.1f);
			ren->SetVertexColor(m_color);
		}		
	}
	else {
		ren->SetHeight(0.1f);
		ren->SetWidth(0.1f);
		m_lockoncnt = 0;
	}
}