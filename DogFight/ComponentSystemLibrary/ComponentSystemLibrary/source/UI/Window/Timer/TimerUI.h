#pragma once
#include "../../../../Component/SceneManager.h"
#include "../../../UI/String/StringComponent.h"
#include "../../../../DX11System/Time.h"
#include "../../../MainGameManager.h"

namespace Component {
	class  TimerUI :public ComponentBase
	{
	private:
		Component::StringComponent* strcom;

		float m_starttime = 0;		// ÉQÅ[ÉÄäJénéûéûä‘
		float m_limitTime = 5;
		float m_stocktime = 0;
		float m_optionstock=NULL;

		float _time = 0;

		bool m_endflg = false;

		void PosUpdate();
		void StartTimer();

		void ReduceTime();
		void LimitCheck();
	public:
		TimerUI(GameObject* _obj);
		~TimerUI();

		void Start();
		void Update();
		void DrawObject();

		void SetTimer(float _timer) { m_limitTime = _timer; }
	};
}
