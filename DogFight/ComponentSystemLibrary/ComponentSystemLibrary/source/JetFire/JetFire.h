#pragma once
#include <vector>
#include "../../Component\GameObject.h"
#include "../../DX11System\dx11mathutil.h"
#include "../ObjectShape/Cone.h"
#include "../Player/F1_PlayerObj.h"
#include "../../DX11System/RandomClass.h"

namespace Component {

	class JetFire : public ComponentBase
	{
	private:
		Transform* tra;

		Component::PlayerObj* player;
		Component::ComputeComponent* efemana;
		std::vector<Cone*> conelist;

		int m_nowFlickcnt = 0;		// 火のちらつきカウント
		int m_maxFlickcnt = 1;		// 火のちらつき最大カウント
		float m_firesize = 0;		// 火のサイズ
		const int conenum = 3;		// 火の数

		void FlickerFire();			// 火のちらつきをランダムに行う
	public:
		JetFire(GameObject* _obj);
		~JetFire();

		void Start();
		void Update();
		void DrawObject();
	};
}
