#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../UI/Fade/StartUIColorComponent.h"
#include "../../../DX11System/mystring.h"
#include "../../UI/String/StringComponent.h"

namespace Component {
	class StartUI :public ComponentBase
	{
	private:
		//----コンポーネント------//
		Transform* tra;
		Component::Quad2DRender *ren;
		Component::StringComponent* strcom;
		Component::StartUIColorComponent* startcom;

		bool uiFinish = false;			// UI終了通知
		int m_nowcnt = 0;				// 現在のカウント
		int m_maxcnt = 30;				// 現在のカウント

		void String();

	public:
		StartUI(GameObject *_owner);
		~StartUI();

		bool GetUIFinish() { return uiFinish; }
		//----基本関数----//
		void Update();

	};
}


