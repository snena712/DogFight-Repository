#include "BulletUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

BulletUI::BulletUI(GameObject *_owner) :ComponentBase(_owner)
{
	 owner->SetName( "BulletUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/BulletUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(30);
	ren->SetWidth(40);

	start = owner->AddComponent<Component::StartUIColorComponent>(1.0f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 + 414, -hei / 2 - 280, 1);
	
	GameObject* wind = new GameObject();
	window = wind->AddComponent<Component::WindowBullet>();
	window->SetType(BulletManager::BulletType::Bullet);
	SceneManager::Instance()->AddObject(wind, false);
	wind->transform->position = owner->transform->position;

	GameObject* ybuttun = new GameObject();
	ybuttun->AddComponent<Component::YButtunUI>();
	SceneManager::Instance()->AddObject(ybuttun, false);
	ybuttun->transform->position = owner->transform->position;
	ybuttun->transform->position.y -= 40;
}

BulletUI::~BulletUI()
{

}

void BulletUI::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL) {
		if (BulletManager::Instance()->GetBulletType() == BulletManager::BulletType::Bullet)
		{
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
		}
		else
		{
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
		}
	}
}

