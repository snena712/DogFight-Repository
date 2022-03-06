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

		//----�W�F�b�g�̃��X�g
		std::vector<Component::PlayerJatParts*> m_jetlist;	// �W�F�b�g���X�g
		int m_jetnum = 9;		// �W�F�b�g�̐�
		int m_rot;				// ���݂̕ێ��p�x
		int m_maxrot = 10;		// �ő�ێ��p�x
		int m_rotreturn = 1;	// �߂�p�x��
	public:
		PlayerJatPartsParent(GameObject* _obj);
		~PlayerJatPartsParent();

		void Start();
		void Update();

		void SetRot(float _num);	// �p�x�̐ݒ�
	};
}


