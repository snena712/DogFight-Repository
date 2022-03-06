#include "AirCloud.h"

using namespace Component;

AirCloud::AirCloud(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("AirCloud");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/RealSmog2.png");
	ren->SetRenderMode(Quad::RenderMode::DEFAULT);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
	ren->SetWidth(1);
	ren->SetHeight(1);

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();
	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(10);
}

AirCloud::~AirCloud()
{
}

void AirCloud::Start()
{

}

void AirCloud::Update()
{
	if (status == Status::LIVE) {

		//AlphaReduce();
		AlphaChanger();

		Collision();
	}
}

void AirCloud::AlphaReduce() {
	if (owner->GetExistState())
	{
		if (status == Status::LIVE) {
			ren->SetVertexColor(m_color);
			ren->SetHeight(m_height);
			ren->SetWidth(m_width);

			m_color.w -= m_reduce;
			m_width += m_plussize.x;
			m_height += m_plussize.y;
		}

		if (m_color.w < 0)
		{
			ren->SetHeight(0);
			ren->SetWidth(0);
			status = Status::DEAD;
			owner->SetExistState(false);
		}
	}
}

void AirCloud::AlphaChanger()
{
	m_alphamaxcnt = 10;


	float limitdown = 0.01f;
	float limitup = 0.4f;
	if (owner->GetExistState())
	{
		if (status == Status::LIVE) {
			if (m_alphanowcnt < m_alphamaxcnt)
			{
				m_alphanowcnt++;

				//m_color.w += m_reduce;
				ren->SetHeight(m_height);
				ren->SetWidth(m_width);
				ren->SetVertexColor(m_color);

				if (m_color.w < limitdown)
				{
					m_color.w = limitdown;
					m_alphanowcnt = m_alphamaxcnt;
				}

				if (m_color.w > limitup)
				{
					m_color.w = limitup;
					m_alphanowcnt = m_alphamaxcnt;
				}
			}
			else
			{
				m_alphanowcnt = 0;

				// 変化量
				m_reduce =  RandomClass::GetRandom(-0.1f, 0.1f) / m_alphamaxcnt;
			}
		}
	}
}

void AirCloud::SetType(ObjType _type)
{
	m_Type = _type;

	switch (m_Type)
	{
	case ObjType::Player:
		ren->ChangeTexture("assets/Image/Smog/RealSmog2.png");
		break;
	case ObjType::Enemy:
		ren->ChangeTexture("assets/Image/Smog/RealSmog2.png");
		break;
	default:
		break;
	}
}

// 当たり判定
void AirCloud::Collision()
{	
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		if (_rigidlist.obj->GetTag() == "Player")
		{
			// アクティブ時のみ
			if (_rigidlist.obj->GetExistState() && m_collisionCnt < m_collisionMaxCnt) {
				owner->transform->position = _rigidlist.obj->transform->position + _rigidlist.obj->transform->forward() * 2;
				m_collisionCnt++;
			}
		}
		else {
			m_collisionCnt = 0;
		}
	}
	
}

// 非アクティブにする
// 雲のインスタンス処理作る
// 端まで行くと消えるようにする