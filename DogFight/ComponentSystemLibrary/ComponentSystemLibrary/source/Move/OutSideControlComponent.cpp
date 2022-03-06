#include "OutSideControlComponent.h"
#include "../../GameObjects/MainCamera.h"
// ����
#include "../../DX11System/CDirectInput.h"

using namespace Component;

OutSideControlComponent::OutSideControlComponent(GameObject *_owner) :ComponentBase(_owner)
{
	tra = owner->transform;
}

OutSideControlComponent::~OutSideControlComponent()
{

}

void OutSideControlComponent::Start()
{
}

void OutSideControlComponent::Update()
{
	OutSideTopDown();
	OutSideAround();
}


void OutSideControlComponent::OutSideTopDown()
{
	// ��]�͈̔�
	if (tra->position.y < RotatePosDown || RotatePosUp < tra->position.y)
	{
		if (m_cnt < m_maxcnt) {
			auto hoge = tra->position;
			hoge += tra->forward() * 1000;
			hoge.y = tra->position.y;
			float hoge2 = m_cnt / m_maxcnt;
			tra->LookSlerp(hoge, Vector3(0, 1, 0), hoge2);
			m_cnt++;
		}
	}

	// �ړ�����
	// ��
	if (tra->position.y < LimitPosDown)
	{
		tra->position.y = LimitPosDown;
	}
	else
	// ��
	if (tra->position.y > LimitPosUp)
	{
		tra->position.y = LimitPosUp;
	}
	else {
		m_cnt = 0;
	}
}


void OutSideControlComponent::OutSideAround()
{
	// y�͖�������
	auto hoge = tra->position;
	hoge.y = centerPos.y;

	auto _dis = hoge.Distance(centerPos);

	// �͈͊O�ɍs������
	if (_dis > BeforeLimitLength)
	{
		if (!m_outFlg) {
			m_beforeOutFlg = true;
		}
	}
	else {
		m_beforeOutFlg = false;
	}

	// �͈͊O
	if (_dis > LimitLength)
	{
		if (!m_outFlg) {
			m_outFlg = true; 
			m_beforeOutFlg = false;
			m_firststraight = true;
		}
	}

	// ���i
	if (m_firststraight) {
		if (m_LengthcCnt < m_LengthMaxCnt / 2)
		{
			m_LengthcCnt++;

			hoge += tra->forward() * 1000;
			hoge.y = tra->position.y;
			float hoge2 = m_LengthcCnt / m_LengthMaxCnt;
			tra->LookSlerp(hoge, Vector3(0, 1, 0), hoge2);
		}
		else
		{
			// �͈͊O�̎��ɂ܂��J�E���g������ƍĎn��
			m_LengthcCnt = 0;
			m_firststraight = false;
			m_turnFlg = true;
		}
	}


	// ���S�Ɍ����鏈��
	if (m_turnFlg) {
		if (m_LengthcCnt < m_LengthMaxCnt)
		{
			tra->RotateLocalXaxis(-3);
			tra->position.x += 0.1f;
			m_LengthcCnt++;
		}
		else
		{
			// �͈͊O�̎��ɂ܂��J�E���g������ƍĎn��
			m_LengthcCnt = 0;
			m_turnFlg = false;
			m_straight = true;
		}
	}

	// ���i
	if (m_straight) {
		if (m_LengthcCnt < m_LengthMaxCnt)
		{
			m_LengthcCnt++;
			tra->RotateLocalZaxis(-3);
		}
		else
		{
			// �͈͊O�̎��ɂ܂��J�E���g������ƍĎn��
			m_LengthcCnt = 0;
			m_straight = false;
			m_outFlg = false;
		}
	}
}
