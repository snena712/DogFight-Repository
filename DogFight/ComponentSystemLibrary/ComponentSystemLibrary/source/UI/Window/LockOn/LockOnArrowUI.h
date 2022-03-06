#pragma once
#include "../../../../Component/Quad2DRenderComponent.h"
#include "../../../Bullet/BulletManager.h"

namespace Component {
	class LockOnArrowUI :public ComponentBase
	{
	public:
		enum class Status
		{
			UP,
			DOWN,
			RIGHT,
			LEFT
		};
	private:
		Component::Quad2DRender* ren;

		void LockOnUpdate();			// ロックオンの更新

		//---ロックオン関連---//
		bool lockonFlg = false;		// ロックオンの状態
		int m_lockoncnt = 0;
		int m_lockonMaxCnt = LOCKONMAX;
		float m_length=200;
		Vector3 m_targetPos;
		Status m_status;
		bool m_init=false;
	public:
		LockOnArrowUI(GameObject *_owner);
		~LockOnArrowUI();

		void Start();
		void Update();
		void DrawObject();

		void SetLoclOnFlg(bool _flg) { lockonFlg = _flg; }
		void SetLockOnCnt(int _cnt) { m_lockoncnt = _cnt; }
		void SetStatus(Status _st);
	};
}

