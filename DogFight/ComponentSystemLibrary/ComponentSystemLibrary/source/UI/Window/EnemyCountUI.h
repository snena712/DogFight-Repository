#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../String/StringComponent.h"
#include "../../Enemy/EnemyManager.h"

namespace Component {

	class  EnemyCountUI :public ComponentBase
	{
	private:
		Component::StringComponent* strcom;

		Transform* tra;

		void String();			// •¶Žš•`‰æ
	public:
		EnemyCountUI(GameObject* _obj);
		~EnemyCountUI();

		void Start();
		void Update();
	};
}
