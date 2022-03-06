#pragma once
#include "../../Component/ModelComponent.h"
#include "../Player/PlayerJatParts.h"

namespace Component {

	class  PlayerJatPartsParent :public ComponentBase
	{
	private:
		Component::ModelRender* model;

		Transform* tra;
		GameObject* player;

		//----ジェットのリスト
		std::vector<Component::PlayerJatParts*> m_jetlist;	// ジェットリスト
		int m_jetnum = 9;		// ジェットの数
		int m_rot;				// 現在の保持角度
		int m_maxrot = 10;		// 最大保持角度
		int m_rotreturn = 1;	// 戻る角度幅
	public:
		PlayerJatPartsParent(GameObject* _obj);
		~PlayerJatPartsParent();

		void Start();
		void Update();

		void SetRot(float _num);	// 角度の設定
	};
}


