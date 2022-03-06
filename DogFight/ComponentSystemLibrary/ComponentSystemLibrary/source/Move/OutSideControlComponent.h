#pragma once
#include "../../Component/ComponentBaseClass.h"


#define LimitPosDown (10)
#define LimitPosUp (2500)
#define RotatePosDown (20)
#define RotatePosUp (LimitPosUp-20)
#define LimitLength (3000)
#define BeforeLimitLength (2500)


namespace Component {
	class OutSideControlComponent :public ComponentBase
	{
	private:
		Transform* tra;

		// 上下のカウント
		float m_cnt;
		const float m_maxcnt=60;
		bool m_outFlg = false;			// 画面外
		bool m_beforeOutFlg = false;	// 画面外
		bool m_turnFlg = false;			// 回転
		bool m_straight = false;		// 直進＋回転 
		bool m_firststraight = false;	// 直進

		// 横のカウント
		float m_LengthcCnt;				// 範囲外カウント
		const float m_LengthMaxCnt = 60;// 範囲外最大カウント
		const Vector3 centerPos = Vector3(0, 500, 0);	// 中心位置

		void OutSideTopDown();			// 上下の範囲外の処理
		void OutSideAround();			// 横方向の範囲外の戻す処理
	public:
		OutSideControlComponent(GameObject* _obj);
		~OutSideControlComponent();

		void Start();
		void Update();

		bool GetOutFlg() { return m_outFlg; }
		bool GetBeforeOutFlg() { return m_beforeOutFlg; }
	};
}
