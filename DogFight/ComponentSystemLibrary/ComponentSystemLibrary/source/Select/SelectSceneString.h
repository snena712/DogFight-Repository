#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../Bullet/BulletManager.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/mystring.h"
#include "../UI/String/StringComponent.h"

namespace Component {
	class  SelectSceneString : public ComponentBase
	{
	public:
		enum class Status
		{
			Press,
			Load
		};

	private:

		Transform* tra;
		Component::Quad2DRender *ren;
		Component::StringComponent* strcom;
		Status m_status = Status::Press;

	public:
		SelectSceneString(GameObject* _obj);
		~SelectSceneString() {}

		void Update();

		void DrawString();		// •¶Žš‚Ì•`‰æ
		void SetStatus(Status _status) { 
			m_status = _status;
			strcom->SetFadeStatus(StringComponent::Fade::FadeOut);
		}
	};
}

