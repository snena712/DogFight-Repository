#pragma once
#include "../../../../Component/Quad2DRenderComponent.h"
#include "../../../../Component/SceneManager.h"
#include "../../../Enemy/EnemyManager.h"
#include "../../../Bullet/BulletManager.h"
namespace Component {
	class  HitUI :public ComponentBase
	{
	private:
		Transform* tra;
		Component::Quad2DRender* ren;

		int m_nowinputcnt = 0;			// 死亡カウント
		const int m_maxinputcnt = 20;	// 死亡最大カウント
		bool m_hitflg = false;;			// 死亡刺した時のカウント

		void PosUpdate();
		void StatusUpdate();
	public:
		HitUI(GameObject* _obj);
		~HitUI();

		void Start();
		void Update();

		// リセット
		void ResetCnt() { m_nowinputcnt = 0; owner->transform->position.x = 10000; }
	};
}
