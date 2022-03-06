#pragma once
#include "../../DX11System/RandomClass.h"
#include "../../Component/ComponentBaseClass.h"

namespace Component {

	class CameraShakeComponent : public ComponentBase
	{
	private:

		float m_stackrand = 0;				// 
		int m_nowrandcnt = 0;				// 
		const int m_maxrandcnt = 25;		// カメラのぶれる感覚
		Vector3 m_randpos;					// ぶれの位置
		Vector3 m_oldstackRandPos;			// ぶれの過去位置保持
		Vector3 m_stackRandPos;				// ぶれの最終位置保持
		bool m_shakeflg = false;			// 振動のフラグ
		bool m_reverse = false;				// falseが左
		float movecnt = 0;					// 移動のカウント
		const int moveMacCnt = 2;			// 移動の最大カウント
		
		void ShakeUpdate();					// 振動の更新
	public:
		CameraShakeComponent(GameObject* _obj);
		~CameraShakeComponent() {};

		void Update();

		void ShakeCamera(float _num);				// 振動のカウントのセット
		Vector3 GetRandPos() { return m_randpos; }	// 振動の位置の取得
	};
}