#pragma once
#include "../../../../Component/GameObject.h"
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../DX11System\dx11mathutil.h"
#include "../../../../Component/SceneManager.h"
#include "../../String/StringComponent.h"
#include "../../../Enemy/EnemyManager.h"

namespace Component {
	class  ScoreUI :public ComponentBase
	{
	private:
		Component::StringComponent* strcom;

		int m_score = 0;				// �Q�[���J�n������
		const float m_scorepoint = 1000;// �G�̃X�R�A
		int m_plusscore = 0;			// �v���X�̃X�R�A
		int m_deadcnt = 0;				// ���݂̎��S��
		int m_nowscorecnt = 0;			// �v���X�̕\���b��
		const int m_maxscorecnt = 30;	// �v���X�̍ő�\���b��
		

		void PosUpdate();		// �ʒu�̍X�V
		void ScoreUpdate();		// �V�[���؊������ɃX�R�A�̍X�V���s��
	public:
		ScoreUI(GameObject* _obj);
		~ScoreUI();

		void Start();
		void Update();
	};
}
