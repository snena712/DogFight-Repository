#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"

namespace Component {

	class BulletFlash :public ComponentBase
	{
	private:

		Transform* tra;
		Component::QuadRender* ren;

		int m_life;			// ‘Ì—Í

	public:
		BulletFlash(GameObject* _obj);
		~BulletFlash();

		void Start();
		void Update();

		
		void SetInitialPosition(Vector3 _pos)	// ˆÊ’u‚Ìİ’è
		{ owner->transform->position = _pos; }

		void SetLife(int _life)					// ‘Ì—Í‚Ìİ’è
		{ m_life = _life; owner->SetExistState(true); }
		
		void LifeReduce() { m_life--; }			// ‘Ì—Í‚ÌŒ¸­

	};
}
