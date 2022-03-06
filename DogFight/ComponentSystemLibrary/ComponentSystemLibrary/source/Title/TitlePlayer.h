#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../DX11System/DX11util.h"
#include "../Bullet/BulletManager.h"
#include "../../source/ObjectShape/Cone.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../DX11System/mystring.h"

namespace Component {
	class TitlePlayer :public ComponentBase
	{
	private:
		//----コンポーネント------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::ComputeComponent* comCompo;
		GameObject* fade;

		// コントローラー
		int ControllerNum = -1;
		bool m_inputflg = false;		// 連続入力停止

		int m_inputcnt = 0;				// 更新
		const int m_inputmax = 30;		// 上限カウント

		// 文字の色の管理変数
		const  XMFLOAT4 _base= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);
		XMFLOAT4 m_col2= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);

		//-------関数--------//
		void KeyBord();				// キーボード
		void Controller();			// コントローラー	
		void InputCnt();			// 入力に応じるカウント管理

		void String();
	public:
		TitlePlayer(GameObject* _obj);
		~TitlePlayer();

		//----基本関数----//
		void Start();
		void Update();
		void Input();
	};
}