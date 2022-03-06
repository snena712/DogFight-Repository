#include "CameraShakeComponent.h"

using namespace Component;

CameraShakeComponent::CameraShakeComponent(GameObject* _owner) :ComponentBase(_owner)
{
}

// �J�����̐U���̏��������
// �U�������炩�ɂ���

void CameraShakeComponent::Update()
{
	ShakeUpdate();
}

// ��ʂ̃u��
void CameraShakeComponent::ShakeCamera(float _num)
{
	// �O�ɓ�������
	if (m_maxrandcnt > m_nowrandcnt)
	{
		m_shakeflg = true;
		m_stackrand = _num;
		m_nowrandcnt = 2;
	}
}

void CameraShakeComponent::ShakeUpdate()
{
	// ��������
	if (m_shakeflg)
	{
		// �]�C
		if (m_nowrandcnt != 0)
		{
			m_nowrandcnt--;
			movecnt++;

			// �ړ��̍ő�l
			if (movecnt > moveMacCnt)
			{
				movecnt = 0;
				// �ߋ��ʒu�̕ۑ�
				m_oldstackRandPos = m_stackRandPos;

				// �V�����ʒu�̕ۑ�

				if (!m_reverse) {
					m_stackRandPos.x = RandomClass::GetRandom(-m_stackrand * m_nowrandcnt, 0.0f);
					m_stackRandPos.y = RandomClass::GetRandom(-m_stackrand * m_nowrandcnt / 2, 0.0f);
					m_reverse = !m_reverse;
				}
				else {
					m_stackRandPos.x = RandomClass::GetRandom(0.0f, m_stackrand * m_nowrandcnt);
					m_stackRandPos.y = RandomClass::GetRandom(0.0f, m_stackrand * m_nowrandcnt / 2);
					m_reverse = !m_reverse;
				}
				//m_stackRandPos.y = RandomClass::GetRandom(-m_stackrand * m_nowrandcnt, m_stackrand * m_nowrandcnt);
			}
			float rate = movecnt / moveMacCnt;
			m_randpos.x = (1 - rate) * m_oldstackRandPos.x + rate * m_stackRandPos.x;
			//m_randpos.y = (1 - rate) * m_oldstackRandPos.y + rate * m_stackRandPos.y;	
		}
		else {
			movecnt = 0;
			m_nowrandcnt = 0;
			m_randpos = Vector3(0, 0, 0);
			m_shakeflg = false;
		}
	}
}
