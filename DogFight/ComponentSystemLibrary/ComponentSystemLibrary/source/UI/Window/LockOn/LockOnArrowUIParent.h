#pragma once
#include "../../../../Component/Quad2DRenderComponent.h"
#include "../../../Bullet/BulletManager.h"
#include "LockOnArrowUI.h"
namespace Component {
	class LockOnArrowUIParent :public ComponentBase
	{
		//Component::BulletManager* bulletManager;
		Component::Quad2DRender* ren;
		std::vector<Component::LockOnArrowUI*> m_lockArrowList;

		void LockOnUpdate();			// ���b�N�I���̍X�V

		//---���b�N�I���֘A---//
		bool lockonFlg = false;		// ���b�N�I���̏��
		int m_lockoncnt = 0;
		float m_size = 50;				// ���b�N�I���̃T�C�Y
		
	public:
		LockOnArrowUIParent(GameObject *_owner);
		~LockOnArrowUIParent();

		void Start();
		void Update();
		void DrawObject();

		void SetLoclOnFlg(bool _flg) { lockonFlg = _flg; }
		void SetLockOnCnt(int _cnt) { m_lockoncnt = _cnt; }
	};
}

