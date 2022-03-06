#include "CameraShakeComponent.h"

using namespace Component;

CameraShakeComponent::CameraShakeComponent(GameObject* _owner) :ComponentBase(_owner)
{
}

// カメラの振動の処理入れる
// 振動を滑らかにする

void CameraShakeComponent::Update()
{
	ShakeUpdate();
}

// 画面のブレ
void CameraShakeComponent::ShakeCamera(float _num)
{
	// 前に動いたか
	if (m_maxrandcnt > m_nowrandcnt)
	{
		m_shakeflg = true;
		m_stackrand = _num;
		m_nowrandcnt = 2;
	}
}

void CameraShakeComponent::ShakeUpdate()
{
	// 減衰処理
	if (m_shakeflg)
	{
		// 余韻
		if (m_nowrandcnt != 0)
		{
			m_nowrandcnt--;
			movecnt++;

			// 移動の最大値
			if (movecnt > moveMacCnt)
			{
				movecnt = 0;
				// 過去位置の保存
				m_oldstackRandPos = m_stackRandPos;

				// 新しい位置の保存

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
