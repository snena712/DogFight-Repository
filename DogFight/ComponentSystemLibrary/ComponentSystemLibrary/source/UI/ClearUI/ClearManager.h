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

		int m_nowinputcnt = 0;			// �ҋ@�J�E���g
		const int m_maxinputcnt = 200;	// �ҋ@�J�E���g�ő�l
		bool onceLoad = false;			// ���̂݃V�[�����[�h����

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