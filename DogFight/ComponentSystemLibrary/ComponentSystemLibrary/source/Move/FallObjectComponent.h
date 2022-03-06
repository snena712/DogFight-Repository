#pragma once
#include "../../Component/ComponentBaseClass.h"


namespace Component {
	class  FallObjectComponent :public ComponentBase
	{
	private:
		float m_speed;		// 落下するスピード
		float m_limitSpeed;	// 落下する制限速度
		float m_stackSpeed;	// 落下する制限速度	

	public:
		FallObjectComponent(GameObject* _obj,float _speed, float _limitspeed);
		~FallObjectComponent();

		void SpeedFall(float _nowspeed);	// スピードに応じた落下
		
	};
}
