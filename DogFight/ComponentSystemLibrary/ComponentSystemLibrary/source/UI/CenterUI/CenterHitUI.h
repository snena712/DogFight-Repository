#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  CenterHitUI :public ComponentBase
	{
	public:
		enum class State {
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT,
		};
	private:

		GameObject* m_camera;
		Component::QuadRender* ren;

		int m_nowinputcnt = 0;		 // カウント
		const int m_maxinputcnt = 6; // 最大カウント

		State m_state;			// 状態
		Vector2 m_plus;			// プラスする位置
		Vector3 m_base;			// 基礎位置
		
		void PosUpdate();		// 位置の更新
		bool StatusUpdate();	// 状態の更新
	public:
		CenterHitUI(GameObject* _obj);
		~CenterHitUI();

		void Start();
		void Update();
	
		void SetState(State _state);	// 状態のセット
	};
}
