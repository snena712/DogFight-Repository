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

		void LockOnUpdate();			// ロックオンの更新

		//---ロックオン関連---//
		bool lockonFlg = false;		// ロックオンの状態
		int m_lockoncnt = 0;
		float m_size = 50;				// ロックオンのサイズ
		
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

