#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  FadeManager :public ComponentBase
	{
	private:
		enum class FadeState 
		{
			NONE,
			FADEIN,
			FADEOUT
		};
		FadeState m_state = FadeState::NONE;

		// コンポーネント
		Transform* tra;
		Component::Quad2DRender* ren;


		float m_alpha = 1;

		int m_fadecnt = 0;
		std::string m_scenename;
	
		
		// fadeの更新
		bool StartFade();
		bool EndFade();
	public:
		FadeManager(GameObject* _obj);
		~FadeManager();

		void Start();
		void Update();

		// フェードイン＋シーン読み込み関数
		void SetFadeInScene(float _cnt, std::string _name) { 
			m_fadecnt = _cnt;
			m_scenename = _name;
			m_state = FadeState::FADEIN; 
			m_alpha = 0; 
			owner->SetExistState(true);
		}
		// フェードアウト関数
		void SetOutScene(float _cnt) {
			m_fadecnt = _cnt; 
			m_state = FadeState::FADEOUT; 
			m_alpha = 1; 
			owner->SetExistState(true);
		}
		
		// フェードアウトの取得
		bool GetFadeOut() {
			if (m_alpha > 0.9f)
				return true; else return false;
		}
	};
}
