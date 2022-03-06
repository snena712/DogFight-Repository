#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		float m_rot=0;			// ���݂̕ێ��p�x
		int m_minrot = 0;		// �ő�ێ��p�x
		int m_maxrot = 10;		// �ő�ێ��p�x

	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(float _num);	// �p�x�̐ݒ�

	};
}

