#pragma once
#include "../../Component/ComputeComponent.h"

namespace Component {

	class FireBulletPlace :public ComponentBase
	{
	private:

		Transform* tra;

		Component::ComputeComponent* efemana;

	public:
		FireBulletPlace(GameObject* _obj);
		~FireBulletPlace();

		void Start();
		void Update();

		void OnFire()
		{
			efemana->ResetElapsedTime();
			efemana->SetStartFlg(true);
		}
	};
}
