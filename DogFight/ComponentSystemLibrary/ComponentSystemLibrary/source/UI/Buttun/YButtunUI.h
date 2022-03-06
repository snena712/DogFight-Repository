#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../UI/Fade/StartUIColorComponent.h""

namespace Component {
	class  YButtunUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		
		Transform* tra;
		Component::Quad2DRender* ren;
		Component::StartUIColorComponent* start;

	public:
		YButtunUI(GameObject* _obj);
		~YButtunUI() {};
	};
}
