#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../Bullet/BulletManager.h"
#include "../../UI/String/StringComponent.h"
namespace Component {
	class  WindowBullet : public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender *ren;
		Component::StartUIColorComponent *start;
		Component::StringComponent* strcom;
		BulletManager::BulletType type;

	public:
		WindowBullet(GameObject* _obj);
		~WindowBullet() {}
		
		void Start();
		void Update();

		void DrawString();		// •¶Žš‚Ì•`‰æ
		void SetType(BulletManager::BulletType _type) { type = _type; }
	};
}

