#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/ComputeComponent.h"

namespace Component {

	class  SplashesEfect :public ComponentBase
	{
	private:
		Transform* tra;
		Component::ComputeComponent* efemana;
	public:
		SplashesEfect(GameObject* _obj);
		~SplashesEfect();

		void Start();
		void Update();

	};
}


