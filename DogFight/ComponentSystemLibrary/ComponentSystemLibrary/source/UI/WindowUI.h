#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../UI/Fade/StartUIColorComponent.h""
#include "../Bullet/BulletManager.h"


namespace Component {


	class WindowUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		GameObject* m_bullet;

		Transform* tra;
		Component::Quad2DRender* ren;
		Component::StartUIColorComponent *start;

		Vector2 m_size;


	public:
		WindowUI(GameObject* _obj);
		~WindowUI();

		void Start();
		void Update();
	};

}