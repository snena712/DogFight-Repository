#include "FallObjectComponent.h"

using namespace Component;

FallObjectComponent::FallObjectComponent(GameObject *_owner,float _speed,float _limitspeed) : ComponentBase(_owner)
{	
	// スピードの追加
	m_speed = _speed;
	m_limitSpeed = _limitspeed;
}

FallObjectComponent::~FallObjectComponent()
{

}


// 速度に応じた落下
void FallObjectComponent::SpeedFall(float _nowspeed)
{
	if (m_limitSpeed > _nowspeed)
	{
		if (m_stackSpeed < m_speed)
			m_stackSpeed += m_speed/100;
		else
			m_stackSpeed = m_speed;
	}
	else {
		if (m_stackSpeed > 0)
			m_stackSpeed -= 0.005f;
		else
			m_stackSpeed = 0;
	}
	owner->transform->position.y -= m_stackSpeed;
}



