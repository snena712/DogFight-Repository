#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../../UI/Fade/StartUIColorComponent.h"

namespace Component {
	class  AltUI : public ComponentBase
	{
	private:
		Transform* tra;
		Component::Quad2DRender* ren;
		Component::StartUIColorComponent* start;

		XMFLOAT4 m_color;	// êF

	public:
		AltUI(GameObject* _obj);
		~AltUI() {}

	};
}

