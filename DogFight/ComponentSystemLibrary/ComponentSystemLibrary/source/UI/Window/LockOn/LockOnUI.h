#pragma once
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../Component\RigidbodyComponent.h"

namespace Component {
	class LockOnUI :public ComponentBase
	{
		//Component::BulletManager* bulletManager;
		Component::QuadRender* ren;

		void LockOnUpdate();			// ���b�N�I���̍X�V

		bool  lockonFlg = false;		// ���b�N�I���̏��
		int	  m_lockoncnt = 0;
		float m_size = 50;				// ���b�N�I���̃T�C�Y
		XMFLOAT4 m_color;

	public:
		LockOnUI(GameObject *_owner);
		~LockOnUI();

		void Start();
		void Update();
		void DrawObject();

		void SetLoclOnFlg(bool _flg) { lockonFlg = _flg; }
		void SetLockOnCnt(int _cnt) { m_lockoncnt = _cnt; }

		void SetAlpha(float _alpha) { m_color.w = _alpha; }
	};
}

