#pragma once
#include "../../Component/ComponentBaseClass.h"
#include "../../Component/GameObject.h"

namespace Component {
	class BulletBase : public ComponentBase 
	{
	public:
		enum class Status
		{
			Live,
			Dead
		};
	protected:
		float m_life;
		Status m_status = Status::Dead;
		bool touchflg = false;
		int atk = 0;

		float speed;
	public:

		BulletBase(GameObject* Owner):ComponentBase(Owner) {}
		~BulletBase() {}
		void SetTouch(bool _flg) { touchflg = _flg; }

		void SetStatus(Status _status) { m_status = _status; }

		Status GetStatus() { 
  			return m_status; }
		
		
		int GetLife() { return  m_life; };
		void SetLife(int _num) { m_life = _num; };

		void SetSpeed(float _num) { speed = _num; }
		float GetSpeed() { return speed; }
	};
}