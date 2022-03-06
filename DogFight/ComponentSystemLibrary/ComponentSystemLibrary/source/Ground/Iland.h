#pragma once

#include "../../Component/SceneManager.h"
#include "../../Component/ModelComponent.h"

namespace Component {
	class  Iland :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::ModelRender* model;

	public:
		Iland(GameObject* _obj);
		~Iland() {};


		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
