#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"

namespace Component {

	class BulletFlash :public ComponentBase
	{
	private:

		Transform* tra;
		Component::QuadRender* ren;

		int m_life;			// 体力

	public:
		BulletFlash(GameObject* _obj);
		~BulletFlash();

		void Start();
		void Update();

		
		void SetInitialPosition(Vector3 _pos)	// 位置の設定
		{ owner->transform->position = _pos; }

		void SetLife(int _life)					// 体力の設定
		{ m_life = _life; owner->SetExistState(true); }
		
		void LifeReduce() { m_life--; }			// 体力の減少

	};
}
