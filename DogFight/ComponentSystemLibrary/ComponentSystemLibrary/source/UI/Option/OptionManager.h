#pragma once
#include "../../../DX11System/CDirectInput.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../../UI/Fade/FadeManager.h"
#include "OptionEndUI.h"
#include "OptionRetryUI.h"
#include "OptionSettingUI.h"
#include "OptionStartUI.h"
#include "OptionTitleUI.h"
#include "../../Select/SelectBackUI.h"
#include "../../Select/SelectUnderbarUI.h"
#include "../../../DX11System/mystring.h"
#include "../Fade/FadeComponent.h"
namespace Component {

	class OptionManager :public ComponentBase
	{
	private:
		enum class OptionState
		{
			NONE,
			MAIN,
			RETRY,
			STAGESELECT,
			SETTING,
			TITLE,
			WAIT,
			END
		};

		enum ColorMODE
		{
			NONE,
			UP,
			DOWN
		};

		OptionState m_option;

		const XMFLOAT4 m_maxcolor = XMFLOAT4(0, 1.0f, 0.0f, 0.7f);
		XMFLOAT4 m_color;
		ColorMODE colorMode = ColorMODE::NONE;

		int ControllerNum = -1;
		int m_optionNum;
		const int m_maxoptionNum = 4;
		const int m_deadzone=20;

		bool m_yesflg = false;
		
		bool m_startInputflg = false;
		bool m_startonce = false;

		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 20;



		// 画面切り替え時
		bool m_chanflg = false;				// 切替フラグ
		int m_nowchancnt = 0;				//  
		const int m_chancntmax = 15;		// カウント

		int m_strlen = 0;						// 現在の下線部文字の長さ


		GameObject* m_end;
		GameObject* m_retry;
		GameObject* m_start;
		GameObject* m_title;
		GameObject* m_setting;
		GameObject* m_backui;
		GameObject* m_line;

		Component::FadeManager* fade;

		void InputKeybord();
		void InputController();
		void OptionUpdate();	// 処理の更新
		void ResetCheck();		// 設定の初期化

		bool ColorChange();
		void LinePos();
		void NumLimit();		// 各状態の値管理
		void NormalUpdate();
		void SettingUpdate();

		void String();

		void SetChangeFlg(ColorMODE _mode) { colorMode = _mode; }
		void PressStart();		// スタートボタン入力に対する反応
	public:
		OptionManager(GameObject* _obj);
		~OptionManager();

		void Start();
		void Update();
	};

}