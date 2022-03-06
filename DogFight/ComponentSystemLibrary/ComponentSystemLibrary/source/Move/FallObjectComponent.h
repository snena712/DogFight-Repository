#pragma once
#include "../../Component/ComponentBaseClass.h"


namespace Component {
	class  FallObjectComponent :public ComponentBase
	{
	private:
		float m_speed;		// ��������X�s�[�h
		float m_limitSpeed;	// �������鐧�����x
		float m_stackSpeed;	// �������鐧�����x	

	public:
		FallObjectComponent(GameObject* _obj,float _speed, float _limitspeed);
		~FallObjectComponent();

		void SpeedFall(float _nowspeed);	// �X�s�[�h�ɉ���������
		
	};
}
