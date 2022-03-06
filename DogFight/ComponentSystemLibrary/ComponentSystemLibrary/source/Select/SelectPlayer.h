#pragma once
#include "../../Component/GameObject.h"
#include "../../Component\RigidbodyComponent.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../DX11System/DX11util.h"
#include "../../source/ObjectShape/Cone.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/RandomClass.h"
#include "../UI/Fade/FadeManager.h"
#include "SelectUnderbarUI.h"
#include "SelectSceneString.h"

namespace Component {

	class SelectPlayer :public ComponentBase
	{
	public :
		enum class Select
		{
			NONE,
			GAMESTART,
			STAGESELECT,
			OPTION,
			SCORE,
			WAIT,
			END
		};

		enum class InputType
		{
			VERTEX,
			WIDOS
		};

		enum ColorMODE
		{
			NONE,
			UP,
			DOWN
		};

	private:
		//----コンポーネント------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::SelectSceneString* string;
		Select m_select= Select::NONE;				// 初めの選択

		const XMFLOAT4 m_maxcolor = XMFLOAT4(0, 1.0f, 0.0f, 0.7f);
		XMFLOAT4 m_color;
		ColorMODE colorMode= ColorMODE::NONE;

		// 位置
		DirectX::XMFLOAT3 g_startpos;	// キー入力された際の開始位置

		// コントローラー
		int ControllerNum = -1;
		bool m_inputflg = false;		
		
		// 入力に対する変数
		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 10;
		bool m_button = false;
		bool m_cancelflg=false;
		int m_inputcnt = 0;				// 入力のカウント
		int m_inputnum = 0;				// 現在の入力
		int m_inputoldnum = 0;			// 過去の入力

		// ＵＩ移動用
		bool m_slideflg = false;
		float m_slidepos = 0;
		int m_nowsldcnt = 0;				//  
		const int m_sldcntmax = 30;			// カウント
		
		// 画面切り替え時
		bool m_chanflg = false;				// 切替フラグ
		int m_nowchancnt = 0;				//  
		const int m_chancntmax = 15;		// カウント

		// シーン読み込み用
		std::string m_loadscnestr;

		// 下線部用
		int m_strlen=0;						// 現在の下線部文字の長さ

		// 各項目の文字列
		const std::string select=("SELECT");
		const std::string start=("START");
		const std::string tutorial=("TUTORIAL");
		const std::string setting=("SETTING");
		const std::string backtitle=("BACK TO TITLE");
		const std::string end=("END");

		const std::string stage=(": STAGE");
		const std::string stage1=("STAGE-1");
		const std::string stage2=("STAGE-2");
		const std::string stage3=("STAGE-3");
		const std::string back=("BACK");

		const std::string option = (": SETTING");
		const std::string con = ("CONTROLER");
		const std::string vib = ("VIB");

		InputType m_type=InputType::VERTEX;

		// ゲームスタート用変数
		int m_gamestartnum = 0;		

		GameObject* fade;
		GameObject* line;

		//-------外部クラスの変数--------//

		//-------関数--------//

		void KeyBord();							// キーボード

		void Controller();
		void InputCnt();

		void SelectUpdate();
		void SettingUpdate();
		void GameStart();
		void GameEnd();
		void StageSelect();

		void String();
		void LinePos();			// 下線の描画
		bool Slider();			// 下線の更新
		bool ColorChange();		// 色更新
		void Sound();			// 音

		void StrLength();
		void InputLimit();		// カーソル管理変数の移動制限
		bool GetChangeFlg() { return m_chanflg; }

		void SetChangeFlg(ColorMODE _mode) { colorMode = _mode; }

	public:
		SelectPlayer(GameObject* _obj);
		~SelectPlayer();

		//----基本関数----//
		void Start();
		void Update();
		void Input();

		bool GetInputFlg() { return m_inputflg; }
	};

}