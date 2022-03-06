#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		float m_rot=0;			// Œ»İ‚Ì•ÛŠp“x
		int m_minrot = 0;		// Å‘å•ÛŠp“x
		int m_maxrot = 10;		// Å‘å•ÛŠp“x

	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(float _num);	// Šp“x‚Ìİ’è

	};
}

