#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../../Component/SceneManager.h"
#include "../UI/Fade/FadeComponent.h"
namespace Component {
	class  SelectBackUI :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender* ren;

	public:
		SelectBackUI(GameObject* _obj);
		~SelectBackUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
