#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../UI/Fade/StartUIColorComponent.h""
#include "../../Bullet/BulletManager.h"
#include "../../UI/Window/WindowBullet.h"
#include "../../UI/Buttun/YButtunUI.h"

namespace Component {
	class HomingBulletUI :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender* ren;
		Component::WindowBullet*window;
		Component::StartUIColorComponent*start;
	public:
		HomingBulletUI(GameObject* _obj);
		~HomingBulletUI();


		void Update();

	};

}