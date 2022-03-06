#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../UI/Fade/StartUIColorComponent.h""
#include "../String/StringComponent.h"

namespace Component {
	class  SkipStringUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		
		Transform* tra;
		Component::StringComponent* strcom;

	public:
		SkipStringUI(GameObject* _obj);
		~SkipStringUI() {};

		void Update();
		void String();		// •¶Žš•`‰æ
	};
}
