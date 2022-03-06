#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  FadeComponent :public ComponentBase
	{
	private:
		// �t�F�[�h�̏��
		enum class FadeState
		{
			NONE,
			FADEIN,
			FADEOUT
		};

		Component::Quad2DRender* ren;
		FadeState m_state = FadeState::NONE;

		float m_alpha = 1;		// ���l
		float m_oldalpha = 1;	// �Â����l
		int m_fadecnt = 0;		// �t�F�[�h�̃J�E���g

		bool StartFade();	// �t�F�[�h�C��
		bool EndFade();		// �t�F�[�h�A�E�g
	public:
		FadeComponent(GameObject* _obj);
		~FadeComponent();

		void Update();
		void DrawObject();

		// �t�F�[�h�̃Z�b�g
		void SetFade(float _cnt) {
			m_fadecnt = _cnt;
			m_state = FadeState::FADEIN;
			m_alpha = 0;
			owner->SetExistState(true);
		}
		// �t�F�[�h�̃J�E���g�Z�b�g
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
