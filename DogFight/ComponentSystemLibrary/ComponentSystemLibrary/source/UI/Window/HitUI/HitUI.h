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

		int m_nowinputcnt = 0;			// ���S�J�E���g
		const int m_maxinputcnt = 20;	// ���S�ő�J�E���g
		bool m_hitflg = false;;			// ���S�h�������̃J�E���g

		void PosUpdate();
		void StatusUpdate();
	public:
		HitUI(GameObject* _obj);
		~HitUI();

		void Start();
		void Update();

		// ���Z�b�g
		void ResetCnt() { m_nowinputcnt = 0; owner->transform->position.x = 10000; }
	};
}
