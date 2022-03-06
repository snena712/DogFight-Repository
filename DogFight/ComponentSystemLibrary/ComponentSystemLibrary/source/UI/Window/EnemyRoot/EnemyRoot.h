#pragma once

#include "../../../../Component/SceneManager.h"
#include "../../../../Component/ModelComponent.h"

namespace Component {
	class  EnemyRoot :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::ModelRender* model;

		void PosUpdate();
	public:
		EnemyRoot(GameObject* _obj);
		~EnemyRoot();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
