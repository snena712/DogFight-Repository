#pragma once
#include "../../../../Component/GameObject.h"
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../DX11System\dx11mathutil.h"
#include "../../../../Component/SceneManager.h"
#include "../../String/StringComponent.h"
#include "../../../Enemy/EnemyManager.h"

namespace Component {
	class  ScoreUI :public ComponentBase
	{
	private:
		Component::StringComponent* strcom;

		int m_score = 0;				// ゲーム開始時時間
		const float m_scorepoint = 1000;// 敵のスコア
		int m_plusscore = 0;			// プラスのスコア
		int m_deadcnt = 0;				// 現在の死亡数
		int m_nowscorecnt = 0;			// プラスの表示秒数
		const int m_maxscorecnt = 30;	// プラスの最大表示秒数
		

		void PosUpdate();		// 位置の更新
		void ScoreUpdate();		// シーン切換え時にスコアの更新を行う
	public:
		ScoreUI(GameObject* _obj);
		~ScoreUI();

		void Start();
		void Update();
	};
}
