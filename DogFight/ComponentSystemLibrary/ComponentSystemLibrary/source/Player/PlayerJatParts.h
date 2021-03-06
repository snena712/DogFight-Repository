#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		float m_rot=0;			// 現在の保持角度
		int m_minrot = 0;		// 最大保持角度
		int m_maxrot = 10;		// 最大保持角度

	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(float _num);	// 角度の設定

	};
}

