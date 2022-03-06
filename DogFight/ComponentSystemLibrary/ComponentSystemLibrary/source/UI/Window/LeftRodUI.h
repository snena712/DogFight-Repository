#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../UI/Fade/StartUIColorComponent.h"

namespace Component {

	class LeftRodUI : public ComponentBase
	{
	private:

		Component::Quad2DRender* ren;
		Component::StartUIColorComponent* start;
	public:
		LeftRodUI(GameObject* _obj);
		~LeftRodUI();

	};

}