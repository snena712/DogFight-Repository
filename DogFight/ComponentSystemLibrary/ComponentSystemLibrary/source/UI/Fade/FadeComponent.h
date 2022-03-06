#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  FadeComponent :public ComponentBase
	{
	private:
		// フェードの状態
		enum class FadeState
		{
			NONE,
			FADEIN,
			FADEOUT
		};

		Component::Quad2DRender* ren;
		FadeState m_state = FadeState::NONE;

		float m_alpha = 1;		// α値
		float m_oldalpha = 1;	// 古いα値
		int m_fadecnt = 0;		// フェードのカウント

		bool StartFade();	// フェードイン
		bool EndFade();		// フェードアウト
	public:
		FadeComponent(GameObject* _obj);
		~FadeComponent();

		void Update();
		void DrawObject();

		// フェードのセット
		void SetFade(float _cnt) {
			m_fadecnt = _cnt;
			m_state = FadeState::FADEIN;
			m_alpha = 0;
			owner->SetExistState(true);
		}
		// フェードのカウントセット
		void SetOut(float _cnt) {
			m_fadecnt = _cnt;
			m_state = FadeState::FADEOUT;
			m_alpha = 1;
			owner->SetExistState(true);
		}

		bool GetFadeOut() {
			if (m_alpha == 0)return true; else return false;
		}
	};
}
