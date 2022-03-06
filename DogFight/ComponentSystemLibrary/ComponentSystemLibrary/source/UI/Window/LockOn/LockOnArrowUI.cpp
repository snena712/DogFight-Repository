#include "LockOnArrowUI.h"

using namespace Component;
#define AroowRate (0.9f)
LockOnArrowUI::LockOnArrowUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("LockOnArrowUI");
}


LockOnArrowUI::~LockOnArrowUI()
{
}

void LockOnArrowUI::Start() { }
void LockOnArrowUI::Update()
{
	LockOnUpdate();

	if (m_init) {
		ren->SetUVState(UV_STATE::DEFAULT);
	}
	else {
		m_init = true;
	}
}
void LockOnArrowUI::DrawObject()
{
	
}


// ロックオンの更新
void LockOnArrowUI::LockOnUpdate()
{
	float _rate;
	auto _target = BulletManager::Instance()->GetTargetObj();
	Vector3 _pos;

	if (_target != nullptr&&BulletManager::Instance()->GetBulletType() == BulletManager::BulletType::Homing)
	{
		_pos = _target->GetComponent<Component::Enemy>()->GetScreenPos();

		if (m_lockonMaxCnt > m_lockoncnt)
		{
			m_lockoncnt++;
		}

		_rate = (float)m_lockoncnt / LOCKONMAX;

		switch (m_status)
		{
		case Component::LockOnArrowUI::Status::UP:
			_pos.y += m_length - m_length * AroowRate * _rate;
			break;

		case Component::LockOnArrowUI::Status::DOWN:
			_pos.y -= m_length - m_length * AroowRate * _rate;
			break;

		case Component::LockOnArrowUI::Status::RIGHT:
			_pos.x -= m_length - m_length * AroowRate * _rate;
			break;

		case Component::LockOnArrowUI::Status::LEFT:
			_pos.x += m_length - m_length * AroowRate * _rate;
			break;

		default:
			break;
		}
		owner->transform->position.x = _pos.x;
		owner->transform->position.y = -_pos.y;
		owner->transform->position.z = _pos.z;
	}

	if (BulletManager::Instance()->GetTargetObj() == nullptr||
		BulletManager::Instance()->GetBulletType() == BulletManager::BulletType::Bullet) {
		owner->transform->position.y = 100;
		m_lockoncnt = 0;
	}
}

void LockOnArrowUI::SetStatus(Status _st)
{
	m_status = _st;

	switch (_st)
	{
	case Component::LockOnArrowUI::Status::UP:
		//画像はロックオンと爆破
		ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Triangle.png");	
		ren->SetUVState(UV_STATE::DEFAULT);
		break;

	case Component::LockOnArrowUI::Status::DOWN:
		//画像はロックオンと爆破
		ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Triangle.png");
		ren->SetUVState(UV_STATE::TURN_Y);
		break;

	case Component::LockOnArrowUI::Status::RIGHT:
		ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Triangle.png");
		owner->transform->RotateZaxis(90);
		break;

	case Component::LockOnArrowUI::Status::LEFT:
		ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Triangle.png");
		owner->transform->RotateZaxis(-90);
		break;

	default:
		break;
	}
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(6);
	ren->SetWidth(6);
	ren->SetVertexColor(XMFLOAT4(1, 1, 0, 1));
}
