#include "WindowBullet.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

WindowBullet::WindowBullet(GameObject *_owner) :ComponentBase(_owner)
{
	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/WindowBullet.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(60);
	ren->SetWidth(80);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));

	strcom = owner->AddComponent<Component::StringComponent>(1.0f);
}

void WindowBullet::Start()
{
	const int _size = 15;
	const int _h = 295;

	strcom->SetColor(Vector3(0, 1.0f, 0.2f));

	// アイコンの小さい文字
	switch (type)
	{
	case BulletManager::BulletType::Bullet:
		strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 + 439,
			Application::Instance()->CLIENT_HEIGHT / 2 + _h, 0),
			Vector3(_size, _size, 0));
		break;

	case BulletManager::BulletType::Homing:
		strcom->SetPosSize(Vector3(Application::Instance()->CLIENT_WIDTH / 2 + 525,
			Application::Instance()->CLIENT_HEIGHT / 2 + _h, 0),
			Vector3(_size, _size, 0));
		break;
	default:
		break;
	}

	if (type == BulletManager::Instance()->GetBulletType())
	{
		start->SetBaseAlpha(0.8f);
	}
	else {
		start->SetBaseAlpha(0.3f);
	}
}

void WindowBullet::Update()
{
	if (MainGameManager::GetInstance()->m_state == MainGameManager::SceneState::NORMAL)
	{
		strcom->SetFadeStatus(StringComponent::Fade::FadeIn);
	}
	DrawString();
}

// 文字描画
void WindowBullet::DrawString()
{

	// 描画するものの選択
	switch (type)
	{
	case BulletManager::BulletType::Bullet:
		strcom->SetString("-");
		break;

	case BulletManager::BulletType::Homing:
		strcom->SetString(std::to_string(BulletManager::Instance()->GetHomigNum()));
		break;
	default:
		break;
	}

	// 弾選択時と非選択時の色
	if (start->GetFadeStatus() == StartUIColorComponent::Fade::Normal) {
		if (type == BulletManager::Instance()->GetBulletType())
		{
			start->SetBaseAlpha(0.8f);
			start->SetColor(Vector3(1, 1, 1));
		}
		else {
			start->SetBaseAlpha(0.3f);
			start->SetColor(Vector3(0.5f, 0.5f, 0.5f));
		}
	}
}