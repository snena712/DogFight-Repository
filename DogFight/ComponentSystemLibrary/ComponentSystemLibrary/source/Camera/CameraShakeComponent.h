#pragma once
#include "../../DX11System/RandomClass.h"
#include "../../Component/ComponentBaseClass.h"

namespace Component {

	class CameraShakeComponent : public ComponentBase
	{
	private:

		float m_stackrand = 0;				// 
		int m_nowrandcnt = 0;				// 
		const int m_maxrandcnt = 25;		// �J�����̂Ԃ�銴�o
		Vector3 m_randpos;					// �Ԃ�̈ʒu
		Vector3 m_oldstackRandPos;			// �Ԃ�̉ߋ��ʒu�ێ�
		Vector3 m_stackRandPos;				// �Ԃ�̍ŏI�ʒu�ێ�
		bool m_shakeflg = false;			// �U���̃t���O
		bool m_reverse = false;				// false����
		float movecnt = 0;					// �ړ��̃J�E���g
		const int moveMacCnt = 2;			// �ړ��̍ő�J�E���g
		
		void ShakeUpdate();					// �U���̍X�V
	public:
		CameraShakeComponent(GameObject* _obj);
		~CameraShakeComponent() {};

		void Update();

		void ShakeCamera(float _num);				// �U���̃J�E���g�̃Z�b�g
		Vector3 GetRandPos() { return m_randpos; }	// �U���̈ʒu�̎擾
	};
}