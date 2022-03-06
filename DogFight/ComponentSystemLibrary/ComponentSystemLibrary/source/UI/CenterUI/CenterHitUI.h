#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  CenterHitUI :public ComponentBase
	{
	public:
		enum class State {
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT,
		};
	private:

		GameObject* m_camera;
		Component::QuadRender* ren;

		int m_nowinputcnt = 0;		 // �J�E���g
		const int m_maxinputcnt = 6; // �ő�J�E���g

		State m_state;			// ���
		Vector2 m_plus;			// �v���X����ʒu
		Vector3 m_base;			// ��b�ʒu
		
		void PosUpdate();		// �ʒu�̍X�V
		bool StatusUpdate();	// ��Ԃ̍X�V
	public:
		CenterHitUI(GameObject* _obj);
		~CenterHitUI();

		void Start();
		void Update();
	
		void SetState(State _state);	// ��Ԃ̃Z�b�g
	};
}
