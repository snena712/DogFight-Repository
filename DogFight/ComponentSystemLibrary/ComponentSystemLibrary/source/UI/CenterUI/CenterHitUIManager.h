#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "CenterHitUI.h"
#include "../../Bullet/BulletManager.h"
namespace Component {
	class  CenterHitUIManager :public ComponentBase
	{
	public:
		// äeéÌèÛë‘
		enum class State {
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT,
		};
	private:
		Component::QuadRender* ren;
		std::vector<GameObject*>m_centerList;

	public:
		CenterHitUIManager(GameObject* _obj);
		~CenterHitUIManager();

		void Start();
		void Update();

	};
}
