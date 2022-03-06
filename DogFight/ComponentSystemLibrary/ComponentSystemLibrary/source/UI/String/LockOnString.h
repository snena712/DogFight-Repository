#pragma once
#include "../../../DX11System/mystring.h"
#include "StringWorldScreen.h"

namespace Component {
	class  LockOnString :public ComponentBase
	{
		Vector3 m_lockOnPos;
		bool lockonFlg = false;

	public:
		LockOnString(GameObject* _obj);
		~LockOnString();

		void Update();
		
		void SetLockOnPos(Vector3 _pos) { m_lockOnPos = _pos; }
		void SetlockOnFlg(bool _flg) { lockonFlg = _flg; }
	};
}