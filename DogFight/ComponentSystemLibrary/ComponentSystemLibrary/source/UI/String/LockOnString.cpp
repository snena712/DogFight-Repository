#include "LockOnString.h"
#include "../../Bullet/BulletManager.h"

using namespace Component;

LockOnString::LockOnString(GameObject *_owner) :ComponentBase(_owner)
{
}


LockOnString::~LockOnString()
{
}

void LockOnString::Update()
{
	// ���b�N�I�����̕����`��
	if (lockonFlg)
	{
		m_lockOnPos.x -= 40;
		m_lockOnPos.y -= 40;

		MyString::DrawString(m_lockOnPos.x,
			m_lockOnPos.y,
			15, 15, XMFLOAT4(1, 0.0f, 0.0f, 0.7f), "LOCKON");
	}
}

