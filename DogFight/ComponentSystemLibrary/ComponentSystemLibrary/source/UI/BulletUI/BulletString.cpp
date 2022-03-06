#include "BulletString.h"


using namespace Component;

BulletString::BulletString(GameObject *_owner) :ComponentBase(_owner)
{
	tra = owner->transform;

	strcom = owner->AddComponent<Component::StringComponent>(0.9f);
	strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 + 430,
		Application::Instance()->CLIENT_HEIGHT / 2 + 200, 0),
		Vector3(30, 30, 0));
	strcom->SetColor(Vector3(0, 1.0f, 0.0f));
}


void BulletString::Update()
{
	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
	}
	DrawString();
}


// •¶Žš•`‰æ
void BulletString::DrawString()
{
	std::string num("1/" + std::to_string(BulletManager::Instance()->GetHomigNum()));

	switch (BulletManager::Instance()->GetBulletType())
	{
	case BulletManager::BulletType::Bullet:
		strcom->SetString("-/- ");
		break;

	case BulletManager::BulletType::Homing:
		strcom->SetString(num);
		break;

	default:
		break;
	}		
}

