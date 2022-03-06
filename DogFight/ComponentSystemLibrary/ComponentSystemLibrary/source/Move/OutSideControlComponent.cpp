#include "OutSideControlComponent.h"
#include "../../GameObjects/MainCamera.h"
// 消す
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
	// 回転の範囲
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

	// 移動制限
	// 下
	if (tra->position.y < LimitPosDown)
	{
		tra->position.y = LimitPosDown;
	}
	else
	// 上
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
	// yは無視する
	auto hoge = tra->position;
	hoge.y = centerPos.y;

	auto _dis = hoge.Distance(centerPos);

	// 範囲外に行きかけ
	if (_dis > BeforeLimitLength)
	{
		if (!m_outFlg) {
			m_beforeOutFlg = true;
		}
	}
	else {
		m_beforeOutFlg = false;
	}

	// 範囲外
	if (_dis > LimitLength)
	{
		if (!m_outFlg) {
			m_outFlg = true; 
			m_beforeOutFlg = false;
			m_firststraight = true;
		}
	}

	// 直進
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
			// 範囲外の時にまだカウント超えると再始動
			m_LengthcCnt = 0;
			m_firststraight = false;
			m_turnFlg = true;
		}
	}


	// 中心に向ける処理
	if (m_turnFlg) {
		if (m_LengthcCnt < m_LengthMaxCnt)
		{
			tra->RotateLocalXaxis(-3);
			tra->position.x += 0.1f;
			m_LengthcCnt++;
		}
		else
		{
			// 範囲外の時にまだカウント超えると再始動
			m_LengthcCnt = 0;
			m_turnFlg = false;
			m_straight = true;
		}
	}

	// 直進
	if (m_straight) {
		if (m_LengthcCnt < m_LengthMaxCnt)
		{
			m_LengthcCnt++;
			tra->RotateLocalZaxis(-3);
		}
		else
		{
			// 範囲外の時にまだカウント超えると再始動
			m_LengthcCnt = 0;
			m_straight = false;
			m_outFlg = false;
		}
	}
}
