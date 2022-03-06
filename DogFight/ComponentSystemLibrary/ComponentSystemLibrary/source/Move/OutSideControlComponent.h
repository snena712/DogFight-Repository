#pragma once
#include "../../Component/ComponentBaseClass.h"


#define LimitPosDown (10)
#define LimitPosUp (2500)
#define RotatePosDown (20)
#define RotatePosUp (LimitPosUp-20)
#define LimitLength (3000)
#define BeforeLimitLength (2500)


namespace Component {
	class OutSideControlComponent :public ComponentBase
	{
	private:
		Transform* tra;

		// �㉺�̃J�E���g
		float m_cnt;
		const float m_maxcnt=60;
		bool m_outFlg = false;			// ��ʊO
		bool m_beforeOutFlg = false;	// ��ʊO
		bool m_turnFlg = false;			// ��]
		bool m_straight = false;		// ���i�{��] 
		bool m_firststraight = false;	// ���i

		// ���̃J�E���g
		float m_LengthcCnt;				// �͈͊O�J�E���g
		const float m_LengthMaxCnt = 60;// �͈͊O�ő�J�E���g
		const Vector3 centerPos = Vector3(0, 500, 0);	// ���S�ʒu

		void OutSideTopDown();			// �㉺�͈̔͊O�̏���
		void OutSideAround();			// �������͈̔͊O�̖߂�����
	public:
		OutSideControlComponent(GameObject* _obj);
		~OutSideControlComponent();

		void Start();
		void Update();

		bool GetOutFlg() { return m_outFlg; }
		bool GetBeforeOutFlg() { return m_beforeOutFlg; }
	};
}
