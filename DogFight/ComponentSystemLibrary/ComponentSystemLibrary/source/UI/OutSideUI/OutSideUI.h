#pragma once
#include "../../../Component/SceneManager.h"
#include "../String/StringComponent.h"
#include "../../Move/OutSideControlComponent.h"

namespace Component {
	class  OutSideUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::StringComponent* strcom;
		Component::OutSideControlComponent* out;

	public:
		OutSideUI(GameObject* _obj);
		~OutSideUI() {};

		void Start();
		void Update();
	};
}
