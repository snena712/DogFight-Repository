#pragma once
#include "../../../DX11System/CDirectInput.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../Fade/FadeManager.h"
#include "../../../DX11System/mystring.h"
#include "../Fade/FadeComponent.h"
#include "../../Select/SelectBackUI.h"

namespace Component {

	class ClearManager :public ComponentBase
	{
	private:

		int m_nowinputcnt = 0;			// 待機カウント
		const int m_maxinputcnt = 200;	// 待機カウント最大値
		bool onceLoad = false;			// 一回のみシーンロードする

		Component::FadeManager* m_fade;
		GameObject* m_backui;

		void EndUpdate();
	public:
		ClearManager(GameObject* _obj);
		~ClearManager();

		void Start();
		void Update();
	};

}