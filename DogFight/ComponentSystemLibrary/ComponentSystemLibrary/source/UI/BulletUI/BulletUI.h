#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../UI/Fade/StartUIColorComponent.h"
#include "../../Bullet/BulletManager.h"
#include "../../../source/UI/Window/WindowBullet.h"
#include "../../UI/Buttun/YButtunUI.h"

namespace Component {
	class BulletUI :public ComponentBase
	{
	private:
		Component::WindowBullet* window;
		Component::StartUIColorComponent* start;
		Transform* tra;
		Component::Quad2DRender* ren;
	public:
		BulletUI(GameObject* _obj);
		~BulletUI();

		void Update();
	};
}