#pragma once
#include "../../../../Component/Quad2DRenderComponent.h"
#include "../../../../Component/SceneManager.h"
#include "../../../Bullet/BulletManager.h"

namespace Component {
	class  DestroyUI :public ComponentBase
	{
	private:
		Transform* tra;
		Component::Quad2DRender* ren;

		int m_nowinputcnt = 0;			// 死亡カウント
		const int m_maxinputcnt = 20;	// 死亡最大カウント
		bool m_deadflg = false;			// 死亡刺した時のカウント

		void StatusUpdate();
	public:
		DestroyUI(GameObject* _obj);
		~DestroyUI();

		void Start();
		void Update();

		// リセット
		void ResetCnt() { m_nowinputcnt = 0; }
	};
}
