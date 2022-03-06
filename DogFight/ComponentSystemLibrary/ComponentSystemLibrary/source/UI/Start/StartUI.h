#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../UI/Fade/StartUIColorComponent.h"
#include "../../../DX11System/mystring.h"
#include "../../UI/String/StringComponent.h"

namespace Component {
	class StartUI :public ComponentBase
	{
	private:
		//----�R���|�[�l���g------//
		Transform* tra;
		Component::Quad2DRender *ren;
		Component::StringComponent* strcom;
		Component::StartUIColorComponent* startcom;

		bool uiFinish = false;			// UI�I���ʒm
		int m_nowcnt = 0;				// ���݂̃J�E���g
		int m_maxcnt = 30;				// ���݂̃J�E���g

		void String();

	public:
		StartUI(GameObject *_owner);
		~StartUI();

		bool GetUIFinish() { return uiFinish; }
		//----��{�֐�----//
		void Update();

	};
}


