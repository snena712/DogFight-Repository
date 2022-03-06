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

		int m_nowinputcnt = 0;			// ���S�J�E���g
		const int m_maxinputcnt = 20;	// ���S�ő�J�E���g
		bool m_deadflg = false;			// ���S�h�������̃J�E���g

		void StatusUpdate();
	public:
		DestroyUI(GameObject* _obj);
		~DestroyUI();

		void Start();
		void Update();

		// ���Z�b�g
		void ResetCnt() { m_nowinputcnt = 0; }
	};
}
