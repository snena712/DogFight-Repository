#include "HomingBulletUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

// ミサイルの残段数
HomingBulletUI::HomingBulletUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("HomingBulletUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/HomingBulletUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(30);
	ren->SetWidth(50);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 + 495, -hei / 2 - 280, 1);

	// 背景UI
	GameObject* wind = new GameObject();
	window=wind->AddComponent<Component::WindowBullet>();
	window->SetType(BulletManager::BulletType::Homing);
	SceneManager::Instance()->AddObject(wind, false);
	wind->transform->position = owner->transform->position;

	GameObject* ybuttun = new GameObject();
	ybuttun->AddComponent<Component::YButtunUI>();
	SceneManager::Instance()->AddObject(ybuttun, false);
	ybuttun->transform->position = owner->transform->position;
	ybuttun->transform->position.y -= 40;
}

HomingBulletUI::~HomingBulletUI()
{
}

void HomingBulletUI::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL) {

		if (BulletManager::Instance()->GetBulletType() == BulletManager::BulletType::Homing)
		{
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
		}
		else
		{
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
		}
	}
}

