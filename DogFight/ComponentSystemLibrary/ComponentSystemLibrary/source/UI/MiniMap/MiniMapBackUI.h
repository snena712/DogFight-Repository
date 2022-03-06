#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../Enemy/EnemyManager.h"

// ミニマップの後ろの物
namespace Component {
	class  MiniMapBackUI :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender* ren;
		Component::StartUIColorComponent* start;

		XMFLOAT4 m_qt;
		float rot = 0;
	
	public:
		MiniMapBackUI(GameObject* _obj);
		~MiniMapBackUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
