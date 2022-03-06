#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/AnimationComponent.h"

namespace Component {
	class  BulletEffect :public ComponentBase
	{

	private:

		Component::Animation* anim;
		Component::QuadRender* ren;

	public:
		BulletEffect(GameObject* _obj);
		~BulletEffect();

		void Update();

		void SetPosSize(Vector3 _vec, Vector3 _size) {
			owner->transform->position = _vec;
			ren->SetWidth(_size.x);
			ren->SetHeight(_size.y);
		}
	};
}
